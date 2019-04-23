#include"TokenDefine.h"

//-------------------------------------------
//Class Token
//-------------------------------------------
bool CTokenDefine::SearchData(LPSTR SearchStr, int &GetValue)
{
	m_iter = m_mapTokenDefine.find(SearchStr);

	if (m_iter == m_mapTokenDefine.end())return FALSE;
	else
	{
		GetValue = (*m_iter).second;
		return TRUE;
	}
}

void CTokenDefine::DefineToken(LPCSTR str, int en)
{
	m_mapTokenDefine.insert(std::pair<std::string, int>(str, en));
}


//-------------------------------------------
//Scene Token
//-------------------------------------------
CToken_Scene::CToken_Scene() { Init(); }

void CToken_Scene::CToken_Scene::Init()
{
	//DefineToken("*SCENE", eScene);
	//DefineToken("asd", b);
	//DefineToken("asd", c);
}

//-------------------------------------------
//TileMap Token
//-------------------------------------------
CToken_TileMap::CToken_TileMap() { Init(); }

void CToken_TileMap::Init()
{
	DefineToken("*MAPNAME", tenTM_MapName);
	DefineToken("*MAPROW", tenTM_Row);
	DefineToken("*MAPCOLUMN", tenTM_Column);
	DefineToken("*CELLWIDTH", tenTM_CellWidth);
	DefineToken("*CELLHEIGHT", tenTM_CellHeight);
	DefineToken("*TILEDATA", tenTM_TileData);
	DefineToken("*TILEPOS", tenTM_TilePos);
	DefineToken("*TILENAME", tenTM_TileName);
}


//-------------------------------------------
//TileSheet Token
//-------------------------------------------
CToken_TileSheet::CToken_TileSheet() { Init(); }

void CToken_TileSheet::Init()
{
	DefineToken("*TILESHEETDATA", tenTS_Data);
	DefineToken("*TILESHEET", tenTS_Name);
	DefineToken("*FILEPATHGROUP", tenTS_PathGroup);
	DefineToken("*FILEPATH", tenTS_Path);	
	DefineToken("*SHEETSIZE", tenTS_Size);
}

//-------------------------------------------
//Tile Token
//-------------------------------------------
CToken_Tile::CToken_Tile() { Init(); }

void CToken_Tile::Init()
{
	DefineToken("*TILEDATA", tenT_TileData);
	DefineToken("*TILENAME", tenT_TileName);
	DefineToken("*REFTILESHEET", tenT_REFSheet);
	DefineToken("*REFPOSITION", tenT_REFPosition);
	DefineToken("*TILETYPE", tenT_TileType);
	DefineToken("*TILEOBSTACLE", tenT_TileObstacle);
}


