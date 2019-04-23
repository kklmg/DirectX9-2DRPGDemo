#include<fstream>
#include<iostream>

#include"TilePalette.h"
#include"Global_data.h"
#include"blit.h"
#include"Convert.h"



STTileSlot::STTileSlot()
	:m_SlotPos({ 0,0,0,0 }), m_pTile(NULL), TileInfo(NULL) {}

STTileSlot::~STTileSlot()
{
	if (TileInfo != NULL)
	{
		delete TileInfo;
		TileInfo = NULL;
	}
	if (m_pTile != NULL)
	{
		delete m_pTile;
		m_pTile = NULL;
	}

}


bool STTileSlot::DrawTile(LPDIRECTDRAWSURFACE7 Destlpdds)
{
	if (m_pTile == NULL)return FALSE;
	m_pTile->DrawTile(Destlpdds, m_SlotPos, 0);
	return TRUE;
}

bool STTileSlot::DrawTile(LPDIRECTDRAWSURFACE7 Destlpdds, RECT rect) 
{
	if (m_pTile == NULL)return FALSE;
	m_pTile->DrawTile(Destlpdds, rect, 0);
	return TRUE;
}


CTile* STTileSlot::CreateNewTile(STTileSrc* __TileInfo)
{
	TileInfo = __TileInfo;
	return m_pTile = __CreateNewTile({ 0,0 }, m_SlotPos, TileInfo, TileInfo->TileName.data());
}







CTileEditor::CTileEditor(WORD SlotSize, WORD bmprow, WORD bmpcolumn,LPCSTR SheetName, WORD pathCount...)
	:m_TSInfo(SheetName, bmprow, bmpcolumn), m_wSlotLength(SlotSize), 
	m_pCurSheet(NULL), m_lpDDSPalette(NULL), m_wMaxPage(0), m_wCurPage(0),
	m_wCursleRow(0), m_wCursleColumn(0), ClickFrame({ 0,0,0,0 }), 
	m_wWidth(PltColumn*SlotSize), m_wHeight(PltRow*SlotSize),
	m_bGetInput(FALSE), m_enSetMode(eSetNone),m_wCurslePage(0)
{
	//-----------------------------------
	//Save Path Data
	//------------------------------------
	va_list ap;
	va_start(ap, pathCount);

	if (pathCount == 0) return;

	for (int i = 0; i < pathCount; ++i)
	{
		m_TSInfo.FilePath.push_back(va_arg(ap, LPCSTR));
	}
	va_end(ap);

	//-------------------------------------------
	//Create TileSheet 
	//-------------------------------------------
	m_Iter = m_TSInfo.FilePath.begin();

	if (pathCount == 1) //Create No ani TileSheet
	{
		m_pCurSheet = new CTileSheet_Single;
		m_pCurSheet->LoadBitMapFile((*m_Iter).data(), bmprow, bmpcolumn);
	}

	else //Create Ani TileSheet
	{
		m_pCurSheet = new CTileSheet_Multi;
		while (m_Iter != m_TSInfo.FilePath.end())
		{
			m_pCurSheet->LoadBitMapFile((*m_Iter).data(), bmprow, bmpcolumn);
			++m_Iter;
		}
	}
	m_pCurSheet->SetName(SheetName);
	//---------------------------------------------
	//Init Palette 
	//---------------------------------------------	
	//Calculate tile Count
	WORD MaxCount = bmprow*bmpcolumn;



	m_wMaxPage = MaxCount / PltSlotCount;

	InitUIDestRect();
	//Init SLot
	InitSlot();


	

	//Set Data to Slot
	WORD r;	// Row
	WORD c;	// Column
	int p;	// Page

	int ct = 0;	//loop count
	STTileSlot* pCurSlot = &m_pSlot[0][0][0];

	for (p = 0; p <= m_wMaxPage; ++p)	//Travesal page
	{
		for (r = 0; r < bmprow; ++r)	//Travesal row
		{
			for (c = 0; c < bmpcolumn; ++c)	//Travesal column
			{
				if (ct >= MaxCount)break;//------ Temp!!!!

				//create default Name auto
				std::string TileName = m_pCurSheet->GetName() + Convert::IntToStr(ct);

				//create Tile Data
				STTileSrc* tilesrc = new STTileSrc(TileName.data(),{ r,c }, m_pCurSheet);

				//create tile
				pCurSlot->CreateNewTile(tilesrc);

				++pCurSlot;
				++ct;
			}
		}
	}
	
}

CTileEditor::~CTileEditor()
{
	if (m_pCurSheet != NULL) 
	{
		delete m_pCurSheet;
		m_pCurSheet = NULL;
	}
	
	if (m_lpDDSPalette != NULL)
	{
		m_lpDDSPalette->Release();
		m_lpDDSPalette = NULL;
	}
	if (m_pSlot != NULL)
	{
		delete[] m_pSlot;
		//m_pSlot = NULL;
	}
}

void CTileEditor::Update()
{
	_FillSurface(g_pDDSBack, 0x00000000);
	POINT pt = g_Mouse.GetMousePos();
	//Get Mouse Event
	if (g_Mouse.IsLeftDown()) 
	{
		int row= CalculateRow(pt.y);
		int column= CalculateColumn(pt.x);
		int page = m_wCurPage;
		if (m_pSlot[page][row][column].m_pTile != NULL) 
		{
			m_wCursleRow = row;
			m_wCursleColumn = column;
			m_wCurslePage = page;
		
		}	
	}


	

	if (DIKEYDOWNONCE(DIK_TAB))
	{
		ReturnToDefaultMode();
		if (m_wCurPage == m_wMaxPage)
			m_wCurPage = 0;
		else
			++m_wCurPage;		
	}

	//Change Setting Mode
	if (DIKEYDOWNONCE(DIK_F1)) 
	{
		m_enSetMode = enSettingMode::eSetNone;	
		m_GuideStr.clear();	
		m_Inputbuff.clear();
	}
	if (DIKEYDOWNONCE(DIK_F2))
	{
		m_enSetMode = enSettingMode::eSetTileType;	
		m_GuideStr = "Set: Type";
		m_Inputbuff.clear();
	}
	if (DIKEYDOWNONCE(DIK_F3))
	{
		m_enSetMode = enSettingMode::eSetTileObastacle;
		m_GuideStr = "Set: Obstacle";
		m_Inputbuff.clear();
	}
	if (DIKEYDOWNONCE(DIK_F4))
	{
		m_enSetMode = enSettingMode::eSetTileName;
		m_GuideStr = "Set: TileName";
		m_Inputbuff.clear();
	}
	if (DIKEYDOWNONCE(DIK_F5))
	{
		m_enSetMode = enSettingMode::eSetTileSheetName;
		m_GuideStr = "Set: TileSheet";
		m_Inputbuff.clear();
	}

	if (DIKEYDOWNONCE(DIK_F10))
	{
		ReturnToDefaultMode();
		if (SaveTileSheet("Data/TileSheet00.txt")) 	
			m_GuideStr = "Save TileSheet Suceed";
	}
	if (DIKEYDOWNONCE(DIK_F11))
	{
		ReturnToDefaultMode();
		if(SaveToFile("Data/TileSetting00.txt"))
			m_GuideStr = "Save Tiles Suceed";
	}
	


	//get input
	switch (m_enSetMode)
	{
		case CTileEditor::eSetNone:
			break;
		case CTileEditor::eSetTileName:
		{
			if (m_Inputbuff.size() < 20)
				m_Inputbuff.append(g_keyBuff);
			if (DIKEYDOWNONCE(DIK_RETURN))
			{
				m_pSlot[m_wCurPage][m_wCursleRow][m_wCursleColumn].TileInfo->TileName = m_Inputbuff;
			//	ReturnToDefaultMode();
			}
		}break;
		case CTileEditor::eSetTileSheetName:
		{
			if (m_Inputbuff.size() < 20)
				m_Inputbuff.append(g_keyBuff);
			if (DIKEYDOWNONCE(DIK_RETURN))
			{
				m_pCurSheet->SetName(m_Inputbuff.data());
				//m_pSlot[m_wCurPage][CursleRow][CursleColumn].m_TileData.RefTileSheet = m_Inputbuff;
			//	ReturnToDefaultMode();
			}
		}break;


		case CTileEditor::eSetTileType:
		{
			if (m_Inputbuff.size() < 3)
				m_Inputbuff.append(g_keyBuff_figure);
			if (DIKEYDOWNONCE(DIK_RETURN))
			{		
				int TIleType;
				sscanf(m_Inputbuff.data(),"%d", &TIleType);
				m_pSlot[m_wCurPage][m_wCursleRow][m_wCursleColumn].TileInfo->TileType = TIleType;
			//	ReturnToDefaultMode();

			}
		}break;
		case CTileEditor::eSetTileObastacle:
		{
			if (m_Inputbuff.size() < 3)
				m_Inputbuff.append(g_keyBuff_figure);
			if (DIKEYDOWNONCE(DIK_RETURN))
			{
				int TIleObs;
				sscanf(m_Inputbuff.data(), "%d", &TIleObs);
				m_pSlot[m_wCurPage][m_wCursleRow][m_wCursleColumn].TileInfo->TileObstacle = TIleObs;
			//
			}
		}break;

		default:
			break;
	}

	//Clear input
	if (DIKEYDOWNONCE(DIK_BACKSPACE)) 
	{
		if (m_Inputbuff.size() != 0)
		{
			m_Inputbuff.erase(m_Inputbuff.end() - 1);
		}
	}

}

HRESULT CTileEditor::Draw(LPDIRECTDRAWSURFACE7 DestlpDDS)
{
	STTileSlot* pCurSlot = &m_pSlot[m_wCurPage][0][0];
	for (int i = 0; i < PltSlotCount; ++i, ++pCurSlot) 
	{
		if (!pCurSlot->DrawTile(DestlpDDS))break;
	}

	return DD_OK;
	//return DestlpDDS->Blt(&m_DrawRect, m_lpDDSPalette, NULL, DDBLT_WAIT, NULL);
}

HRESULT CTileEditor::DrawTileInfo(LPDIRECTDRAWSURFACE7 DestlpDDS)
{
	HRESULT hrlt;
	//Draw Seleted File
	m_pSlot[m_wCurslePage][m_wCursleRow][m_wCursleColumn].DrawTile(DestlpDDS, m_DrawSelectTile);

	//hrlt = DestlpDDS->Blt(&m_DrawSelectTile, m_lpDDSPalette, &m_pSlot[m_wCurPage][CursleRow][CursleColumn].m_SlotPos, DDBLT_WAIT, NULL);
	
	//Draw Tile Name
	std::string TileName("TileName: ");
	TileName += m_pSlot[m_wCurslePage][m_wCursleRow][m_wCursleColumn].TileInfo->TileName;
	g_pUIMng->__DrawText(DestlpDDS, TileName.data(), m_DrawTileName);

	//Draw Tile Sheet Name
	std::string TileSheetName("TileSheet: ");
	TileSheetName += m_pCurSheet->GetName();
	g_pUIMng->__DrawText(DestlpDDS, TileSheetName.data(), m_DrawSheetName);

	//Draw User Input
	std::string strInput("Input: ");
	strInput += m_Inputbuff;
	g_pUIMng->__DrawText(DestlpDDS, strInput.data(), m_DrawUserInput);

	//Draw Tile Type
	std::string strType("Type: ");
	strType += Convert::WordToStr(m_pSlot[m_wCurslePage][m_wCursleRow][m_wCursleColumn].TileInfo->TileType);
	g_pUIMng->__DrawText(DestlpDDS, strType.data(), m_DrawTileType);

	//Draw Tile Obstacle
	std::string strObstacle("Obstacle: ");
	strObstacle += Convert::WordToStr(m_pSlot[m_wCurslePage][m_wCursleRow][m_wCursleColumn].TileInfo->TileObstacle);
	g_pUIMng->__DrawText(DestlpDDS, strObstacle.data(), m_DrawTileObstacle);

	//Draw Guide		
	g_pUIMng->__DrawText(DestlpDDS, m_GuideStr.data(), m_DrawGuide);
	

	return DD_OK;
	//return hrlt;
}

void CTileEditor::InitSlot()
{
	
	m_pSlot = new STTileSlot[m_wMaxPage + 1][PltRow][PltColumn];

	int p, r, c;
	for (p = 0; p <= m_wMaxPage; ++p)// p:page
	{
		for (r = 0; r < PltRow; ++r) //  r: row
		{
			for (c = 0; c < PltColumn; ++c) //c: column
			{
				m_pSlot[p][r][c].m_SlotPos.left = c*m_wSlotLength + m_DrawRect.left;
				m_pSlot[p][r][c].m_SlotPos.top = r*m_wSlotLength + m_DrawRect.top;
				m_pSlot[p][r][c].m_SlotPos.right = (c + 1)*m_wSlotLength + m_DrawRect.left;
				m_pSlot[p][r][c].m_SlotPos.bottom = (r + 1)*m_wSlotLength + m_DrawRect.top;
			}
		}
	}
}


void CTileEditor::InitUIDestRect()
{
	m_DrawRect = CreateRect({ HRES / 2, VRES / 2 }, m_wWidth, m_wHeight);

	m_DrawSelectTile = { 0,0,(long)(HRES*0.1),(long)(VRES*0.1) };

	m_DrawTileName = { 0,(long)(VRES*0.9),HRES,(long)(VRES*0.95) };

	m_DrawSheetName = { 0,(long)(VRES*0.95),HRES,VRES };

	m_DrawUserInput = { 0,(long)(VRES*0.85),HRES,(long)(VRES*0.9) };

	m_DrawTileType = { 0,(long)(VRES*0.1),300,(long)(VRES*0.15) };

	m_DrawTileObstacle = { 0,(long)(VRES*0.15),300,(long)(VRES*0.2) };

	m_DrawGuide = { 0,(long)(VRES*0.8),HRES,(long)(VRES*0.85) };
}




WORD CTileEditor::CalculateRow(int y)
{
	WORD Row = (y - m_DrawRect.top) / m_wSlotLength;
	if (Row >= PltRow)Row = PltRow-1;
	return Row;
}
WORD CTileEditor::CalculateColumn(int x)
{
	WORD Column = (x - m_DrawRect.left) / m_wSlotLength;
	if (Column >= PltColumn)Column = PltColumn-1;
	return Column;
}

void CTileEditor::ReturnToDefaultMode()
{
	m_enSetMode = enSettingMode::eSetNone;
	m_GuideStr.clear();
	m_Inputbuff.clear();
}

bool CTileEditor::SaveToFile(LPCSTR filePath) 
{
	std::ofstream WtFile;

	//try open file
	WtFile.open(filePath);
	if (!WtFile.is_open())return FALSE;

	//Seek File end
	WtFile.seekp(0,std::ios::end);

	//write Data
	STTileSlot* pCurSlot = &m_pSlot[0][0][0];
	int MaxCount = m_pCurSheet->GetTileCount();
	int p, ct = 0;

	for (int p = 0; p <= m_wMaxPage; ++p)
	{
		for (int i = 0; i <= PltSlotCount; ++i, ++pCurSlot,++ct)
		{
			if (ct > MaxCount)break;
			if (pCurSlot->m_pTile == NULL)break;
			WtFile << "*TILEDATA\n";
			WtFile << "{\n";
			WtFile << "\t*TILENAME\t" << '"' << pCurSlot->TileInfo->TileName.data() << '"' << '\n';
			WtFile << "\t*REFTILESHEET\t" << '"' << m_pCurSheet->GetName().data() << '"' << '\n';
			WtFile << "\t*REFPOSITION\t" << pCurSlot->TileInfo->SrcTile.wRow << '\t' << pCurSlot->TileInfo->SrcTile.wColumn << '\n';
			WtFile << "\t*TILETYPE\t" << pCurSlot->TileInfo->TileType << '\n';
			WtFile << "\t*TILEOBSTACLE\t" << pCurSlot->TileInfo->TileObstacle << '\n';
			WtFile << "}\n";
		}
	}
	//Clos file
	WtFile.close();
}

bool CTileEditor::SaveTileSheet(LPCSTR filePath) 
{
	std::ofstream WtFile;

	//try open file
	WtFile.open(filePath);
	if (!WtFile.is_open())return FALSE;

	//Seek File end
	WtFile.seekp(0,std::ios::end);
	
	//write Data
	WtFile << "*TILESHEETDATA\n";
	WtFile << "{\n";
	WtFile << "\t*TILESHEET" << "\t" << '"' << m_pCurSheet->GetName().data() << '"' << '\n';

	WtFile << "\t*FILEPATHGROUP" << '\n';
	WtFile << "\t{\n";
	for (m_Iter = m_TSInfo.FilePath.begin(); m_Iter != m_TSInfo.FilePath.end(); ++m_Iter) 
	{
		WtFile << "\t\t*FILEPATH"<<"\t"<< '"'<< (*m_Iter).data() <<'"' << '\n';
	}
	WtFile << "\t}\n";

	WtFile<<"\t*SHEETSIZE"<<"\t"<< m_TSInfo.Row<< "\t" << m_TSInfo.Column << '\n';
	WtFile << "}\n\n";

	//Clos file
	WtFile.close();


}
