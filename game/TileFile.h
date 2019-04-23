#pragma once
#include<Windows.h>
#include<string>
#include"Tile.h"
#include"ConCTN.h"

//Tile Data
//-----------------------------------
struct FSTTileData
{
	FSTTileData() :TileName("_UnKnown_"), RefTileSheet("_UnKnown_"),
		RefRow(0), RefColumn(0), TileType(0), TileObsLevel(0) {}

	FSTTileData(LPCSTR TileName,LPCSTR TilesheetName) :TileName(TileName), RefTileSheet(TilesheetName),
		RefRow(0), RefColumn(0), TileType(0), TileObsLevel(0) {}
	
	std::string TileName;
	std::string RefTileSheet;

	WORD RefRow;
	WORD RefColumn;

	WORD TileType;
	WORD TileObsLevel;

	CTile* CreateTile();
};

struct FSTTileDataGroup
{
	//Parse Data
	bool Parsing(LPCSTR FilePath);

	//Data Container
	CDataContainer<FSTTileData> m_DataCon;
};
