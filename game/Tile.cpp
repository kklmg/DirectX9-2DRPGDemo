//---------------------------------------------
//Head File
//---------------------------------------------


//---------------------------------------------
//My Head File
//---------------------------------------------
#include"Tile.h"
#include"blit.h"
#include"Global_data.h"

//---------------------------------------------
//Class Tile Constructor / Destroyer
//---------------------------------------------
CTile::CTile(GRID grid, RECT rect, STTileSrc* TileSource)
	:m_gdPos(grid), m_rtPos(rect),
	m_wTileType(TileSource->TileType), m_bIncObstacle(TileSource->TileObstacle),
	m_wObstacle_level(TileSource->TileObstacle), m_pDesc(TileSource),
	m_dwID((grid.wRow << 16) | grid.wColumn) {}


CTile::~CTile()
{
}


//Get Tile Data
//---------------------------------------------
DWORD CTile::GetTileID()const
{
	return m_dwID;
}

std::string CTile::GetTileName() const
{
	return m_pDesc->TileName;
}
std::string CTile::GetTileSheetName() const 
{
	return m_pDesc->_TileSheet->GetName();
}

GRID CTile::GetGrid()const
{
	return m_gdPos;
}

RECT CTile::GetPostion() const
{
	return m_rtPos;
}

POINT CTile::GetTileCenter()const
{
	RECT rect= m_rtPos;
	return GetRectCenter(rect);
}

WORD CTile::GetObstacleLevel() const
{
	return m_wObstacle_level;
}

bool CTile::ifIncludeObstacle() const
{
	return m_bIncObstacle;
}

void CTile::MarkIncludeObstacle()
{
	m_bIncObstacle = TRUE;
}

STTileSrc* CTile::GetTileDesc()const 
{
	return m_pDesc;
}
//---------------------------------------------
//Set Tile ATT
//---------------------------------------------
void CTile::SetTileType(WORD type)
{
	m_wTileType = type;
}

void CTile::SetTileObstacleLevel(WORD level)
{
	m_wObstacle_level = level;
}



//---------------------------------------------
//Class Static Tile
//---------------------------------------------
CTile_Static::CTile_Static(GRID grid, RECT rect, STTileSrc *TileSource)
	: CTile(grid, rect, TileSource)
{
}
CTile_Static::CTile_Static(GRID grid, RECT rect, STTileSrc* TileSource, WORD TileType, WORD ObsLevel)
	: CTile(grid, rect, TileSource)
{
	CTile::m_wTileType = TileType;
	CTile::m_wObstacle_level = ObsLevel;
	CTile::m_bIncObstacle = ObsLevel;
}
CTile_Static::~CTile_Static()
{
}

void CTile_Static::DrawTile(LPDIRECTDRAWSURFACE7 DestSuf, DWORD DrawSetting)
{	
	m_pDesc->DrawTile(DestSuf, m_rtPos,0);
}
void CTile_Static::DrawTile(LPDIRECTDRAWSURFACE7 DestSuf, RECT& DestRect, DWORD DrawSetting)
{
	m_pDesc->DrawTile(DestSuf, DestRect,0);
}


//---------------------------------------------
//Class Dynamic Tile
//---------------------------------------------
CTile_Dynamic::CTile_Dynamic(GRID grid, RECT rect, STTileSrc *TileSource, float rate)
	: CTile(grid, rect, TileSource), m_dwIndex(0),
	m_fRate(rate), m_dwMaxIndex(TileSource->_TileSheet->GetSheetCount()) {}

CTile_Dynamic::CTile_Dynamic(GRID grid, RECT rect, STTileSrc *TileSource, WORD TileType, WORD ObsLevel, float rate)
	: CTile(grid, rect, TileSource), m_dwIndex(0),
	m_fRate(rate), m_dwMaxIndex(TileSource->_TileSheet->GetSheetCount())
{
	CTile::m_wTileType = TileType;
	CTile::m_wObstacle_level = ObsLevel;
	CTile::m_bIncObstacle = ObsLevel;
}
CTile_Dynamic::~CTile_Dynamic()
{
}

void CTile_Dynamic::UpdateTile() 
{	
	if (m_Timer.LoopTimer(m_fRate))
	{
		++m_dwIndex;
		if (m_dwIndex >= m_dwMaxIndex)m_dwIndex = 0;
	}
}

void CTile_Dynamic::DrawTile(LPDIRECTDRAWSURFACE7 DestSuf, DWORD DrawSetting)
{
	UpdateTile();
	m_pDesc->DrawTile(DestSuf, m_rtPos, m_dwIndex);
}

void CTile_Dynamic::DrawTile(LPDIRECTDRAWSURFACE7 DestSuf, RECT& DestRect, DWORD DrawSetting)
{
	UpdateTile();
	m_pDesc->DrawTile(DestSuf, DestRect, m_dwIndex);
}

CTile* __CreateNewTile(GRID grid, RECT rect, LPCSTR TileName) 
{
	CTile* newTile = NULL;
	STTileSrc*Tsc= g_pDataBase->GetTile(TileName);
	if (Tsc == NULL)return NULL;

	if (Tsc->bMulti) 
	{
		newTile = new CTile_Dynamic(grid, rect, Tsc, Rate_TileUpade);
	}
	else 
	{
		newTile = new CTile_Static(grid, rect, Tsc);
	}
	Tsc->TileName = TileName;

	return newTile;
}

CTile* __CreateNewTile(GRID grid, RECT rect, STTileSrc*TileInfo, LPCSTR TileName)
{
	CTile* newTile = NULL;
	if (TileInfo == NULL)return NULL;

	if (TileInfo->bMulti)
	{
		newTile = new CTile_Dynamic(grid, rect, TileInfo, Rate_TileUpade);
	}
	else
	{
		newTile = new CTile_Static(grid, rect, TileInfo);
	}
	TileInfo->TileName = TileName;

	return newTile;
}