#include<fstream>

#include"TileMap.h"
#include"blit.h"
#include"Text.h"
#include"InitDD.h"
#include"collide.h"


RECT __GetCoord(GRID _grid, WORD nGridWidth, WORD nGridHeight)
{
	RECT _rect;

	_rect.left = nGridWidth*_grid.wColumn;
	_rect.top = nGridHeight*_grid.wRow;
	_rect.right = nGridWidth*_grid.wColumn + nGridWidth;
	_rect.bottom = nGridHeight*_grid.wRow + nGridHeight;

	return _rect;
}

CTileMap::CTileMap() :
	m_dwRow(0), 
	m_dwColumn(0),
	m_dwWidth(0),
	m_dwHeight(0),
	m_pppTile(NULL),
	m_wCellLength(0)
{
}

CTileMap::CTileMap(DWORD Row, DWORD Column, WORD CellLength,LPCSTR MapName):m_strMapName(MapName)
{
	InitMap(Row, Column, CellLength);
}

CTileMap::~CTileMap()
{
	for (int i = 0; i < m_dwRow; i++)
	{
		for (int j = 0; j < m_dwColumn; j++)
		{
			SAFE_RELEASE(m_pppTile[i][j]);
		}
	}

	for (int i = 0; i < m_dwRow; i++)
	{
		if (m_pppTile[i] != NULL)
		{
			delete[] m_pppTile[i];
			m_pppTile[i] = NULL;
		}
	}
	if (m_pppTile != NULL)
	{
		delete[] m_pppTile;
		m_pppTile = NULL;
	}



}

bool CTileMap::InitMap(DWORD Row, DWORD column, WORD CellLength)
{
	if (column == 0 || Row == 0)return FALSE;
	if (CellLength == 0) return FALSE;

	m_wCellLength = CellLength;		//per cell size

	m_dwRow = Row;			//map row
	m_dwColumn = column;	//map column
	m_dwTileCount = Row*column;

	m_dwWidth = m_dwColumn * CellLength;	//map width
	m_dwHeight = m_dwRow * CellLength;		//map height


	m_rtMapRect = {0,0,(long)m_dwWidth,(long)m_dwHeight};	//map rect



	//int **p1 = new int *[x];
	//*p1 = new int[x*y];
	//for (int i = 0; i < y; i++)
	//	p1[i] = *p1 + i*y;
	
	//m_pppTile = new CTile**[m_dwRow];	//row
	//*m_pppTile = new CTile*[m_dwRow*m_dwColumn];//all

	//for (int i = 0; i <m_dwColumn; ++i) //column
	//{
	//	m_pppTile[i] = *m_pppTile + i*m_dwColumn;
	//}


	////Create Cells
	m_pppTile = new CTile**[m_dwRow];//row
	for (int i = 0; i <m_dwRow; i++) //column
	{
		m_pppTile[i] = new CTile*[m_dwColumn];
	}

	//Init Cell Data
	for (int i = 0; i < m_dwRow; i++)
	{
		for (int j = 0; j < m_dwColumn; j++)
		{
			m_pppTile[i][j] = NULL;
		}
	}

	//Set direct draw Clipper
	//__DDCSetClip(m_pMapSuf, m_ptCornerLT.x, m_ptCornerLT.y, m_ptCornerRB.x, m_ptCornerRB.y);

	return TRUE;
}

////////////
//Draw Map//
////////////
void  CTileMap::DrawMap(LPDIRECTDRAWSURFACE7 destSuf, RECT &Screen)
{
	STMapPos CoverTile = GetRectPos(Screen);
	
	bool update= m_Timer.LoopTimer(0.5);
	
	int i, j;
	//Draw Cell 
	for (i = CoverTile.nTile_Top; i < CoverTile.nTile_Bottom+1; ++i)
	{
		for (j = CoverTile.nTile_Left; j < CoverTile.nTile_Right+1; ++j)
		{
			if (m_pppTile[i][j] != NULL)
			{
				if(update)
					m_pppTile[i][j]->DrawTile(destSuf,1);
				else
					m_pppTile[i][j]->DrawTile(destSuf, 0);
			}
		}
	}
}


//get cell coord
POINT CTileMap::GetGridCoord(GRID &_grid)
{
	return m_pppTile[_grid.wRow][_grid.wColumn]->GetTileCenter();
}

RECT CTileMap::GetGridRect(GRID &_grid)
{
	RECT TempRect;
	if (_grid.wColumn > m_dwColumn || _grid.wRow > m_dwRow)
	{
		TRACE0("this cell is not in the map");
		return{ 0,0,0,0 };
	}
	TempRect.left = m_wCellLength*_grid.wColumn;
	TempRect.top = m_wCellLength*_grid.wRow;
	TempRect.right = m_wCellLength*_grid.wColumn + m_wCellLength;
	TempRect.bottom = m_wCellLength*_grid.wRow + m_wCellLength;

	return TempRect;
}

RECT CTileMap::GetGridRect(GRID &_gridLT, GRID &_gridRB)
{
	RECT rect =
	{
		_gridLT.wColumn * m_wCellLength,
		_gridLT.wRow * m_wCellLength,
		(_gridRB.wColumn + 1) * m_wCellLength,
		(_gridRB.wRow + 1) * m_wCellLength,
	};

	return rect;
}

RECT CTileMap::GetMapRect() const 
{
	return m_rtMapRect;
}

WORD CTileMap::GetCelllength()const
{
	return m_wCellLength;
}

////////////////
//Tile Setting//
////////////////
CTile* CTileMap::GetTile(DWORD Row, DWORD column) const 
{
	if (Row >= m_dwRow || column >= m_dwColumn)return NULL;
	return m_pppTile[Row][column];
}


//set tile
void CTileMap::SetTile(GRID DestGrid, STTileSrc *TileSrc)
{
	if (DestGrid.wColumn >= m_dwColumn
		|| DestGrid.wRow >= m_dwRow)
	{
		TRACE0("this cell is not in the map");
		return;
	}
	RECT rcDst = GetGridRect(DestGrid);

	if (m_pppTile[DestGrid.wRow][DestGrid.wColumn] != NULL)
	{
		if (m_pppTile[DestGrid.wRow][DestGrid.wColumn]->GetTileDesc() == TileSrc)
			return;

		SAFE_RELEASE(m_pppTile[DestGrid.wRow][DestGrid.wColumn]);
	}

	if (TileSrc->bMulti)	//create static file
		m_pppTile[DestGrid.wRow][DestGrid.wColumn] = new CTile_Dynamic(DestGrid, rcDst, TileSrc, Rate_TileUpade);
	else				//create dynamic tile
		m_pppTile[DestGrid.wRow][DestGrid.wColumn] = new CTile_Static(DestGrid, rcDst, TileSrc);

}

void CTileMap::SetTile(GRID DestGrid, STTileSrc *TileSrc, WORD TileType, WORD ObstacleLevel)
{
	if (DestGrid.wColumn > m_dwColumn
		|| DestGrid.wRow > m_dwRow)
	{
		TRACE0("this cell is not in the map");
		return;
	}
	RECT rcDst = GetGridRect(DestGrid);

	if (m_pppTile[DestGrid.wRow][DestGrid.wColumn] != NULL) 
	{
		SAFE_RELEASE(m_pppTile[DestGrid.wRow][DestGrid.wColumn]);
	}


	if (TileSrc->bMulti)	//create static file
		m_pppTile[DestGrid.wRow][DestGrid.wColumn] 
		= new CTile_Dynamic(DestGrid, rcDst, TileSrc,TileType, ObstacleLevel, Rate_TileUpade);
	else				//create dynamic tile
		m_pppTile[DestGrid.wRow][DestGrid.wColumn] 
		= new CTile_Static(DestGrid, rcDst, TileSrc, TileType, ObstacleLevel);
}

//set tile horizontal
void CTileMap::SetTileH(WORD rowMap, STTileSrc *TileSrc, WORD TileType, WORD ObstacleLevel)
{
	for (WORD i = 0; i <m_dwRow; i++)
		for (WORD j = 0; j <m_dwColumn; j++)
		{
			if (i == rowMap)
			{
				SetTile({ i, j }, TileSrc, TileType, ObstacleLevel);
			}
		}
}

//set tile vertical
void CTileMap::SetTileV(WORD columnMap, STTileSrc *TileSrc, WORD TileType, WORD ObstacleLevel)
{
	if (columnMap >m_dwColumn)
	{
		TRACE0("tile out range");
		return;
	}

	for (WORD i = 0; i < m_dwRow; i++)
		for (WORD j = 0; j <m_dwColumn; j++)
		{	
			if (j == columnMap)
			{
				SetTile({ i, j }, TileSrc, TileType, ObstacleLevel);
			}
		}
}

void CTileMap::SetTileEdge(STTileSrc *TileSrc, WORD TileType, WORD ObstacleLevel)
{
	SetTileH(0, TileSrc,TileType,ObstacleLevel);
	SetTileH(m_dwRow-1, TileSrc, TileType, ObstacleLevel);
	SetTileV(0, TileSrc, TileType, ObstacleLevel);
	SetTileV(m_dwColumn-1, TileSrc ,TileType, ObstacleLevel);
}




//set tile all
void CTileMap::SetTileAll(STTileSrc *TileSrc, WORD TileType, WORD ObstacleLevel)
{
	for (WORD i = 0; i < m_dwRow; i++)
		for (WORD j = 0; j <m_dwColumn; j++)
		{
			SetTile({i,j}, TileSrc, TileType, ObstacleLevel);
		}
}


void CTileMap::MarkObstacle(RECT &rect) 
{
	STMapPos CoverTile = GetRectPos(rect);

	int i, j;
	for (i = CoverTile.nTile_Top; i < CoverTile.nTile_Bottom + 1; ++i)
	{
		for (j = CoverTile.nTile_Left; j < CoverTile.nTile_Right + 1; ++j)
		{
			if (m_pppTile[i][j] != NULL)
			{
				m_pppTile[i][j]->MarkIncludeObstacle();
			}
		}
	}
}

//Save Data To Text File
bool CTileMap::SaveToTxtFile(LPCSTR FilePath) 
{
	std::ofstream WtFile;
	int r, c;
	//try open file
	WtFile.open(FilePath);
	if (!WtFile.is_open())return FALSE;

	//write Tile Map Data
	WtFile << "*MAPNAME\n" << "\t" << '"' <<m_strMapName.data() << '"' << std::endl;

	WtFile << "{\n";	//Tile Map Data block start
	WtFile << "\t*CELLWIDTH" << "\t" << m_wCellLength << std::endl;//save cell width
	WtFile << "\t*CELLHEIGHT" << "\t" << m_wCellLength << std::endl;//save cell Height
	WtFile << "\t*MAPROW" << "\t" << m_dwRow << std::endl;	//save row
	WtFile << "\t*MAPCOLUMN" << "\t" << m_dwColumn << std::endl;//save column

	WtFile << "\t*TILEDATA" << std::endl;//save column

	//Save Tile Data
	//------------------------------------------------------------
	WtFile << "{"<<std::endl;	//Tile Data block start
	for (r = 0; r < m_dwRow; ++r)
	{
		for (c = 0; c < m_dwColumn; ++c)
		{
			if (m_pppTile[r][c] != NULL)
			{
				WtFile << "\t*TILENAME" << "\t" << '"' << m_pppTile[r][c]->GetTileName().data() << '"' << std::endl;
				WtFile << "\t*TILEPOS" << "\t" << r << "\t" << c << std::endl << std::endl;
			}
		}
	}
	WtFile << "}\n";	//Tile Data block end


	WtFile << "}\n";	//Tile Map Data block end

	WtFile.close();

	return TRUE;
}










//random set
//void CTileMap::SetTileAllRandomly(CTileSheet *_TS, WORD TileType, WORD ObstacleLevel)
//{
//	for (WORD i = 0; i < m_dwRow; i++)
//		for (WORD j = 0; j <m_dwColumn; j++)
//		{
//			SetTile({ i, j },
//			{ (WORD)(rand() % _TS->m_wRow), (WORD)(rand() % _TS->m_wColumn) },
//				_TS, TileType, ObstacleLevel);
//		}
//}

//void CTileMap::SpillTile(WORD count, GRID desTile, CTileSheet *_TS, WORD TileType, WORD ObstacleLevel)
//{
//	for (WORD i = 0; i < count; i++)
//	{
//		SetTile({ (WORD)(rand() % m_dwRow),(WORD)(rand() %m_dwColumn) },
//		{ desTile.wRow, desTile.wColumn },
//			_TS, TileType, ObstacleLevel);
//	}
//}
//
//void CTileMap::SpillTileRandomly(WORD count, CTileSheet *_TS, WORD TileType, WORD ObstacleLevel)
//{
//	for (WORD i = 0; i < count; i++)
//	{
//		SetTile({ (WORD)(rand() % m_dwRow), (WORD)(rand() %m_dwColumn) },
//		{ (WORD)(rand() % _TS->m_wRow), (WORD)(rand() % _TS->m_wColumn) },
//			_TS, TileType, ObstacleLevel);
//	}
//}



//////////////////
//Get Infomation//
//////////////////
DWORD CTileMap::GetMapWidth() const 
{
	return m_dwWidth;
}

DWORD CTileMap::GetMapHeight() const 
{
	return m_dwHeight;
}

/////////////////////
//Display map info//
/////////////////////
void CTileMap::DisplayMapData()
{	
	for (WORD i = 0; i < m_dwRow; i++)
		for (WORD j = 0; j <m_dwColumn; j++)
		{
			__PutTextf
			(j*m_wCellLength,
				i*m_wCellLength,
				RED, "(%d , %d)", i, j);

			if (m_pppTile[i][j]->ifIncludeObstacle()) {
				__PutTextf
				(j*m_wCellLength,
					i*m_wCellLength + m_wCellLength / 2,
					BLUE, "TRUE");
			}
			else
			{
				__PutTextf
				(j*m_wCellLength,
					i*m_wCellLength + m_wCellLength / 2,
					WHITE, "FALSE");
			}
		}
}

void CTileMap::DisplayMapDataEX(LPDIRECTDRAWSURFACE7 suf)
{
	suf->GetDC(&gHdc);
	for (WORD i = 0; i < m_dwRow; i++)
		for (WORD j = 0; j <m_dwColumn; j++)
		{
			__PutTextf
			(j*m_wCellLength,
				i*m_wCellLength,
				RED, "(%d , %d)", i, j);

			if (m_pppTile[i][j]->GetObstacleLevel() >= 1) {
				__PutTextf
				(j*m_wCellLength,
					i*m_wCellLength + m_wCellLength / 2,
					BLUE, "TRUE");
			}
			else
			{
				__PutTextf
				(j*m_wCellLength,
					i*m_wCellLength + m_wCellLength / 2,
					WHITE, "FALSE");
			}
		}
	suf->ReleaseDC(gHdc);
}

STMapPos CTileMap::GetRectPos(RECT &rect)
{
	STMapPos pos;

	//figure out 
	pos.nTile_Left  = rect.left / m_wCellLength;
	pos.nTile_Top = rect.top / m_wCellLength;
	pos.nTile_Right= rect.right / m_wCellLength;
	pos.nTile_Bottom = rect.bottom / m_wCellLength;

	if (pos.nTile_Left < 0) pos.nTile_Left = 0;	
	if (pos.nTile_Right >m_dwColumn - 1) pos.nTile_Right =m_dwColumn - 1;
	if (pos.nTile_Top < 0) pos.nTile_Top = 0;
	if (pos.nTile_Bottom > m_dwRow - 1)	pos.nTile_Bottom = m_dwRow - 1;
	return pos;
}

STMapPos CTileMap::GetCirClePos(STCircle &Circle) 
{
	RECT rect;
	rect.left = Circle.point.x- Circle.radius;
	rect.top = Circle.point.y - Circle.radius;
	rect.right = Circle.point.x + Circle.radius;
	rect.bottom = Circle.point.y + Circle.radius;

	STMapPos pos=GetRectPos(rect);
	return pos;
}

CTile* CTileMap::GetPointTilePos(POINTFLOAT &pos)
{
	DWORD column = pos.x / m_wCellLength;
	if (column < 0 || column >=m_dwColumn )return NULL;

	DWORD row = pos.y / m_wCellLength;
	if(row<0 || row >=m_dwRow)return NULL;

	return m_pppTile[row][column];
}

CTile* CTileMap::GetTile(DWORD row, DWORD column)
{
	if (row >= m_dwRow || column >= m_dwColumn)return NULL;
	else return m_pppTile[row][column];
}

DWORD CTileMap::GetTilecount() 
{
	return m_dwColumn*m_dwRow;
}

bool CTileMap::GetTileGrid(int x,int y, GRID &GetGrid)
{
	GetGrid.wRow = y / m_wCellLength;
	GetGrid.wColumn = x / m_wCellLength;

	if (GetGrid.wRow > m_dwRow || GetGrid.wColumn > m_dwColumn)return FALSE;
	else return TRUE;
}