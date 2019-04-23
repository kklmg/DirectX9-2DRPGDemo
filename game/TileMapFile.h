#pragma once
#include<string>
#include<windows.h>
#include<list>

#include"TileMap.h"

//Tile Reference
struct FSTTileRef
{
	FSTTileRef();
	
	std::string TileName;

	WORD Row;
	WORD Column;
};

//Tile Map Data
struct FSTTileMapData
{
	FSTTileMapData();
	~FSTTileMapData();

	bool Parsing(LPCSTR FilePath);

	CTileMap* CreateTileMap();


	std::string MapName;

	WORD Cell_Width; 
	WORD Cell_Height;
	
	WORD MapRow;
	WORD MapColumn;

	std::list<FSTTileRef*> TileDataList;
	std::list<FSTTileRef*>::iterator m_iter;
};



