#pragma once
#include<ddraw.h>
#include<string>
#include"TileSheet.h"
#include"MyStruct.h"
#include"Tile.h"
#include"ObjectClass.h"

//obstacle=0 is not aobstacle
//obstacle=1
//obstacle=2 
//obstacle=3 

struct STMapPos
{
	int nTile_Left = 0;
	int nTile_Top = 0;
	int nTile_Right = 0;
	int nTile_Bottom = 0;
};



RECT __GetCoord(GRID _grid, WORD nGridWidth, WORD nGridHeight);

class CTileMap
{
public:
	//----------------------------------------------
	//Creator & Destroyer
	//----------------------------------------------
	CTileMap();
	//Create New Tile Map
	CTileMap(DWORD Row, DWORD column, WORD CellLength,LPCSTR MapName="UnKnown");
	//Load Tile Map
	CTileMap(LPCSTR FilePath);
	
	~CTileMap();

	//----------------------------------------------
	//Init map
	//----------------------------------------------
	bool InitMap(DWORD Row,DWORD column,WORD CellLength);


	//----------------------------------------------
	//Get Data
	//----------------------------------------------
	DWORD GetMapWidth() const;
	DWORD GetMapHeight() const;

	CTile* GetTile(DWORD Row, DWORD column) const;

	WORD GetCelllength()const;


	//Get Map Position
	bool CTileMap::GetTileGrid(int x, int y, GRID &GetGrid);

	POINT GetGridCoord(GRID &_grid);

	RECT GetGridRect(GRID &_grid);

	RECT GetGridRect(GRID &_grid1, GRID &_grid2);

	RECT GetMapRect() const;

	STMapPos GetRectPos(RECT &rect);

	STMapPos GetCirClePos(STCircle &circle);

	CTile* GetPointTilePos(POINTFLOAT &pos);

	//
	DWORD GetTilecount();

	CTile* GetTile(DWORD row, DWORD column);


	//----------------------------------------------
	//Save Map Data to file
	//----------------------------------------------
	bool SaveToTxtFile(LPCSTR FilePath);

	//----------------------------------------------
	//Update & Draw
	//----------------------------------------------
	void UpdateTile();

	void DrawMap(LPDIRECTDRAWSURFACE7 suf,RECT &SourceRect);


	//----------------------------------------------
	//Tile Setting
	//----------------------------------------------
	void SetTile(GRID DestGrid, STTileSrc *TileSrc);
	void SetTile(GRID DestGrid, STTileSrc *TileSrc, WORD TileType, WORD ObstacleLevel);

	void SetTileH(WORD rowMap, STTileSrc *TileSrc, WORD TileType, WORD ObstacleLevel);
	void SetTileV(WORD columnMap, STTileSrc *TileSrc, WORD TileType, WORD ObstacleLevel);

	void SetTileAll(STTileSrc *TileSrc, WORD TileType, WORD ObstacleLevel);

	void SetTileEdge(STTileSrc *TileSrc, WORD TileType, WORD ObstacleLevel);

	//void SetTileAllEX(GRID desTile, CTileSheet *_TS, WORD TileType, WORD ObstacleLevel);
	//void SetTileAllRandomly(CTileSheet *_TS, WORD TileType, WORD ObstacleLevel);
	//void SetTileRandomEX(CTileSheet *_TS, WORD TileType, WORD ObstacleLevel);
	//void SpillTile(WORD count, GRID desTile, CTileSheet *_TS, WORD TileType, WORD ObstacleLevel);
	//void SpillTileRandomly(WORD count, CTileSheet *_TS, WORD TileType, WORD ObstacleLevel);

	//
	void MarkObstacle(RECT &rect);

	//used in Test
	void DisplayMapData();
	void DisplayMapDataEX(LPDIRECTDRAWSURFACE7 suf);

private:
	std::string m_strMapName;

	RECT m_rtMapRect;

	DWORD m_dwRow;
	DWORD m_dwColumn;
	DWORD m_dwTileCount;

	DWORD m_dwWidth;
	DWORD m_dwHeight;

	CTile ***m_pppTile;
	WORD m_wCellLength;

	CTimer m_Timer;
};

