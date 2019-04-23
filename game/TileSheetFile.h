#pragma once
#include<Windows.h>
#include<string>
#include<list>


#include"ConCTN.h"

//Tile Sheet/Palette Data
//-----------------------------------
struct FSTTileSheet
{
	FSTTileSheet():SheetName("_UnKnown_"), Row(0), Column(0){}
	FSTTileSheet(LPCSTR Name, WORD _Row, WORD _Column)
		:SheetName(Name), Row(_Row), Column(_Column) {}
	

	std::string SheetName;
	std::list<std::string> FilePath;

	WORD Row;
	WORD Column;
};

struct FSTTileSheetGroup
{
	CDataContainer<FSTTileSheet> m_DataCon;

	bool Parsing(LPCSTR FilePath);
};


