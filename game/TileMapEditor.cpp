#include"TileMapEditor.h"
#include"blit.h"
#include"InitDD.h"
#include"Global_data.h"
#include<fstream>
#include"TileMapFile.h"

CTileMapEditor::CTileMapEditor()
	: m_pTileMap(NULL), m_pCamera(NULL), m_pDDScene(NULL)
	,m_wSleRow(0), m_wSleColumn(0),m_enCurView(eTileMap), m_Timer(eTmEnd),
	m_pltIndex(0), m_pltSleID(0), m_pCurSleTile(NULL), m_emEditMode(emEditMode::emCreateNew)
{
	//m_pTilepalette = new CTilePalette(50,"Tile.txt");
	InitPalette();
	
	int halfH = HRES / 2;
	int setV = VRES*0.1;
	m_rtDrawSlect = CreateRect({ halfH,setV }, 40, 40);

	//tile buff
	m_pCurSleTile = new CTile*;
	*m_pCurSleTile = NULL;

	//Init Guide
	InitGuide();
	

}

CTileMapEditor::~CTileMapEditor()
{
	if (m_pTileMap != NULL)
	{
		delete m_pTileMap;
		m_pTileMap = NULL;
	}
	if (m_pCamera != NULL) 
	{
		delete m_pCamera;
		m_pCamera = NULL;		
	}
	if (m_pDDScene != NULL)
	{
		m_pDDScene->Release();
		m_pDDScene = NULL;
	}
	if (m_pCurSleTile != NULL)
	{
		delete m_pCurSleTile;
		m_pCurSleTile = NULL;
	}
	


	std::vector<CTilePalette*>::iterator iter = m_vectorplt.begin();

	for (;iter != m_vectorplt.end();++iter) 
	{
		delete (*iter);
		(*iter) = NULL;
	}
		
}

void CTileMapEditor::CreateTileMap(WORD CellWidth, WORD CellHeight, WORD Row, WORD Column, LPCSTR SavePath)
{
	m_emEditMode = emEditMode::emCreateNew;

	m_szSavePath = SavePath;

	m_pTileMap = new CTileMap(Row, Column, CellWidth);
	//Get Tile Data
	STTileSrc* TileSrc = g_pDataBase->GetTile("basic4");

	//Set Tile
	if (TileSrc != NULL) m_pTileMap->SetTileAll(TileSrc, 1, 1);
	

	m_pCamera = new CCamera(m_pTileMap);

	m_pDDScene = CreateSurfaceEX(m_pTileMap->GetMapWidth(), m_pTileMap->GetMapHeight(), TRUE);

	if (m_pTileMap->GetMapWidth() >= HRES && m_pTileMap->GetMapHeight() >= VRES)
	{
		m_DrawTileMap = { 0,0,HRES,VRES };
	}
	else
	{
		POINT ScreenCenter = { HRES >> 1,VRES >> 1 };
		m_DrawTileMap = CreateRect(ScreenCenter, m_pCamera->GetWidth(), m_pCamera->GetHeight());
	}
}

bool CTileMapEditor::LoadTileMap(LPCSTR FilePath) 
{
	m_szSavePath = FilePath;	

	//parsing file and create
	FSTTileMapData MapData;
	MapData.Parsing(FilePath);
	m_pTileMap = MapData.CreateTileMap();

	if (!m_pTileMap)return FALSE;
	m_pCamera = new CCamera(m_pTileMap);

	m_pDDScene = CreateSurfaceEX(m_pTileMap->GetMapWidth(), m_pTileMap->GetMapHeight(), TRUE);

	if (m_pTileMap->GetMapWidth() >= HRES && m_pTileMap->GetMapHeight() >= VRES)
	{
		m_DrawTileMap = { 0,0,HRES,VRES };
	}
	else
	{
		POINT ScreenCenter = { HRES >> 1,VRES >> 1 };
		m_DrawTileMap = CreateRect(ScreenCenter, m_pCamera->GetWidth(), m_pCamera->GetHeight());
	}



	return TRUE;
}

void CTileMapEditor::InitPalette()
{
	std::list<FSTTileData*> GetList;
	g_pDataBase->GetAllTileData(GetList);

	while (GetList.size()!=0)
	{
		CTilePalette *newpt = new CTilePalette(50);
		newpt->SetTile(GetList);
		m_vectorplt.push_back(newpt);
	}

}

void CTileMapEditor::Update()
{
	_FillSurface(g_pDDSBack, 0x00000000);

	//Update Tilemap
	if (m_enCurView == eTileMap)
	{
		if (DIKEYDOWNONCE(DIK_F2)) 
		{
			m_strKeyguide = "F1:TimeMap LeftRight:Change Page";
			m_enCurView = ePalette;
		}
	
		//----------------------------------------------------
		//Update clipper
		//----------------------------------------------------
		m_pCamera->Update();

		POINT pt = g_Mouse.GetMousePos();
		POINT Mappt = GetMapCoord(pt);
		GRID grid;

		//Mouse press
		if (g_Mouse.IsLeftPress())
		{
			if (m_pTileMap->GetTileGrid(Mappt.x, Mappt.y, grid))
			{
				//get coord
				m_wSleRow = grid.wRow;
				m_wSleColumn = grid.wColumn;

				//set tile
				if(*m_pCurSleTile != NULL)
				m_pTileMap->SetTile(grid, (*m_pCurSleTile)->GetTileDesc());
			}
		}
		
		if (DIKEYDOWNONCE(DIK_F11)) 
		{
			if (SaveMapInfoToFile()) 
			{
				m_strActguide = "Save Map suceed";
				m_Timer.ResetTimer();
			}
		}


	}
	else if (m_enCurView==ePalette)
	{
		if (DIKEYDOWNONCE(DIK_F1))
		{
			m_strKeyguide = "F2:Palette F11::Save";
			m_enCurView = eTileMap;
		}

		if (m_vectorplt.size() == 0)return;

		if (DIKEYDOWNONCE(DIK_LEFT))
		{
			--m_pltIndex;
			if (m_pltIndex < 0)
				m_pltIndex = m_vectorplt.size() - 1;
		}

		if (DIKEYDOWNONCE(DIK_RIGHT)) 
		{
			++m_pltIndex;
			if (m_pltIndex >= m_vectorplt.size())
				m_pltIndex = 0;
		}
	}
	

}

POINT CTileMapEditor::GetMapCoord(POINT &ScreenCoord) 
{	
	
	POINT pt =
	{
		ScreenCoord.x + m_pCamera->GetCameraRect().left - m_DrawTileMap.left,
		ScreenCoord.y + m_pCamera->GetCameraRect().top - m_DrawTileMap.top
	};

	return pt;
}

void CTileMapEditor::Draw(LPDIRECTDRAWSURFACE7 DestlpDDS)
{
	if (m_enCurView == eTileMap)
	{
		m_pTileMap->DrawMap(m_pDDScene, m_pCamera->GetCameraRect());


		g_pDDSBack->Blt(&m_DrawTileMap, m_pDDScene, &m_pCamera->GetCameraRect(), DDBLT_WAIT, NULL);
	}

	else if (m_enCurView == ePalette) 
	{	
		DrawPalette(DestlpDDS);
	}

	DrawSlectedTile(g_pDDSBack);
	DrawGuide(g_pDDSBack);
}

void CTileMapEditor::InitGuide()
{
	m_DrawKeyGuide = { 0, 0, (long)(HRES), (long)(VRES*0.05) };
	m_DrawActGuide = { 0,(long)(VRES*0.95),HRES,VRES };

	m_strKeyguide = "F1:TimeMap F2:Palette F11::Save";
}

void CTileMapEditor::DrawGuide(LPDIRECTDRAWSURFACE7 lpDDS)
{
	//Draw Guide		
	g_pUIMng->__DrawText(lpDDS, m_strKeyguide.data(), m_DrawKeyGuide);

	if (m_Timer.OnceTimer(2))m_strActguide.clear();
	
	if (m_strActguide.size() != 0)
		g_pUIMng->__DrawText(lpDDS, m_strActguide.data(), m_DrawActGuide);

}

void CTileMapEditor::DrawSlectedTile(LPDIRECTDRAWSURFACE7 lpDDS)
{	
	if(*m_pCurSleTile!=NULL)
	(*m_pCurSleTile)->DrawTile(lpDDS, m_rtDrawSlect,0);
}

void CTileMapEditor::DrawPalette(LPDIRECTDRAWSURFACE7 lpDDS) 
{
	if (m_vectorplt.size() == 0)return;
	if (m_pltIndex >= m_vectorplt.size())return;
	m_vectorplt[m_pltIndex]->Update(m_pCurSleTile);
	m_vectorplt[m_pltIndex]->DrawTileInfo(lpDDS);
	m_vectorplt[m_pltIndex]->Draw(lpDDS);
}

bool CTileMapEditor::SaveMapInfoToFile()
{
	return m_pTileMap->SaveToTxtFile(m_szSavePath);
}
