#include<fstream>
#include<iostream>

#include"TilePalette.h"
#include"Global_data.h"
#include"blit.h"
#include"Convert.h"



CTilePalette::CTilePalette(WORD SlotSize)
	:m_wSlotLength(SlotSize),m_lpDDSPalette(NULL),
	CursleRow(0), CursleColumn(0), ClickFrame({ 0,0,0,0 }), m_wTileCount(0),
	m_wWidth(PltColumn*SlotSize), m_wHeight(PltRow*SlotSize), m_wSlotVolume(PltRow*PltColumn)
{
	//-----------------------
	//Init Palette 
	//-----------------------
	m_pLastSlot = &m_Slot[0][0];

	//Init SLot
	InitUIDestRect();
	InitSlot();
	

	//Init DDSurface
	InitDDSurFace();
}

CTilePalette::~CTilePalette()
{
	if (m_lpDDSPalette != NULL)
	{
		m_lpDDSPalette->Release();
		m_lpDDSPalette = NULL;
	}

	for (int i = 0; i < m_wTileCount; ++i)
	{
		--m_pLastSlot;
		if (m_pLastSlot->pTile != NULL) 
		{
			delete m_pLastSlot->pTile;
			m_pLastSlot->pTile = NULL;	
		}
	}



}

void CTilePalette::SetTile(std::list<FSTTileData*>& __list) 
{
	std::list<FSTTileData*>::iterator iter = __list.begin();

	//Set tile
	for (int i = 0; i < m_wSlotVolume; ++i, ++m_pLastSlot, ++m_wTileCount)
	{
		if (__list.size() == 0)break;

		m_pLastSlot->m_TileData = (*iter);	
		m_pLastSlot->pTile = __CreateNewTile({0,0}, m_pLastSlot->m_SlotPos, (*iter)->TileName.data());

		m_pLastSlot->pTile->DrawTile(m_lpDDSPalette, 0);
		__list.erase(iter++);
	}
}


void CTilePalette::Update(CTile** GetSelect)
{
	POINT pt = g_Mouse.GetMousePos();
	//Get Mouse Event
	if (g_Mouse.IsLeftDown())
	{
		CursleRow = CalculateRow(pt.y);
		CursleColumn = CalculateColumn(pt.x);

		if (m_Slot[CursleRow][CursleColumn].pTile != NULL)
			*GetSelect = m_Slot[CursleRow][CursleColumn].pTile;
	}
}

HRESULT CTilePalette::Draw(LPDIRECTDRAWSURFACE7 DestlpDDS)
{
	for (STPaSlot *pCurSlot = &m_Slot[0][0]; pCurSlot != m_pLastSlot; ++pCurSlot)
	{
		if(pCurSlot->pTile!=NULL)
			pCurSlot->pTile->DrawTile(DestlpDDS, 0);
	}
	return DD_OK;
}

HRESULT CTilePalette::DrawTileInfo(LPDIRECTDRAWSURFACE7 DestlpDDS)
{
	HRESULT hrlt;
	//Draw Seleted Tile
//	DrawSlectedTile(DestlpDDS, m_DrawSelectTile);
//	hrlt = DestlpDDS->Blt(&m_DrawSelectTile, m_lpDDSPalette, &m_Slot[CursleRow][CursleColumn].m_SlotPos, DDBLT_WAIT, NULL);

	//Draw Tile Name
	std::string TileName("TileName: ");
	if(m_Slot[CursleRow][CursleColumn].m_TileData!=NULL)
	TileName += m_Slot[CursleRow][CursleColumn].m_TileData->TileName;
	g_pUIMng->__DrawText(DestlpDDS, TileName.data(), m_DrawTileName);

	//Draw Tile Sheet Name
	std::string TileSheetName("TileSheet: ");
	if (m_Slot[CursleRow][CursleColumn].m_TileData != NULL)
	TileSheetName += m_Slot[CursleRow][CursleColumn].m_TileData->RefTileSheet;
	g_pUIMng->__DrawText(DestlpDDS, TileSheetName.data(), m_DrawSheetName);


	//Draw Tile Type
	std::string strType("Type: ");
	if (m_Slot[CursleRow][CursleColumn].m_TileData != NULL)
	strType += Convert::WordToStr(m_Slot[CursleRow][CursleColumn].m_TileData->TileType);
	g_pUIMng->__DrawText(DestlpDDS, strType.data(), m_DrawTileType);

	//Draw Tile Obstacle
	std::string strObstacle("Obstacle: ");
	if (m_Slot[CursleRow][CursleColumn].m_TileData != NULL)
	strObstacle += Convert::WordToStr(m_Slot[CursleRow][CursleColumn].m_TileData->TileObsLevel);
	g_pUIMng->__DrawText(DestlpDDS, strObstacle.data(), m_DrawTileObstacle);


	return DD_OK;
}

HRESULT CTilePalette::DrawSlectedTile(LPDIRECTDRAWSURFACE7 DestlpDDS, RECT rect)
{
	m_Slot[CursleRow][CursleColumn].pTile->DrawTile(DestlpDDS,rect,0);
	return DD_OK;
}

void CTilePalette::InitSlot()
{
	int i, j;
	for (i = 0; i < PltRow; ++i) //i: row
	{
		for (j = 0; j < PltColumn; ++j) //j: column
		{
			m_Slot[i][j].m_SlotPos.left = j*m_wSlotLength + m_DrawRect.left;
			m_Slot[i][j].m_SlotPos.top = i*m_wSlotLength + m_DrawRect.top;
			m_Slot[i][j].m_SlotPos.right = (j + 1)*m_wSlotLength + m_DrawRect.left;
			m_Slot[i][j].m_SlotPos.bottom = (i + 1)*m_wSlotLength + m_DrawRect.top;
		}
	}
}


void CTilePalette::InitDDSurFace()
{
	m_lpDDSPalette = CreateSurfaceEX(m_wWidth, m_wHeight, TRUE);

	_FillSurface(m_lpDDSPalette, BLUE);
}

void CTilePalette::InitUIDestRect()
{
	m_DrawRect = CreateRect({ HRES / 2, VRES / 2 }, m_wWidth, m_wHeight);

	m_DrawSelectTile = { 0,0,(long)(HRES*0.1),(long)(VRES*0.1) };

	m_DrawTileName = { 0,(long)(VRES*0.9),HRES,(long)(VRES*0.95) };

	m_DrawSheetName = { 0,(long)(VRES*0.95),HRES,VRES };

	m_DrawUserInput = { 0,(long)(VRES*0.85),HRES,(long)(VRES*0.9) };

	m_DrawTileType = { 0,(long)(VRES*0.1),300,(long)(VRES*0.15) };

	m_DrawTileObstacle = { 0,(long)(VRES*0.15),300,(long)(VRES*0.2) };
}


WORD CTilePalette::CalculateRow(int y)
{
	WORD Row = (y - m_DrawRect.top) / m_wSlotLength;
	if (Row >= PltRow)Row = PltRow - 1;
	return Row;
}
WORD CTilePalette::CalculateColumn(int x)
{
	WORD Column = (x - m_DrawRect.left) / m_wSlotLength;
	if (Column >= PltColumn)Column = PltColumn - 1;
	return Column;
}


