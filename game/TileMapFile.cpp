#include"TileMapFile.h"
#include"MyLexer.h"
#include"Global_data.h"

//Tile Reference
FSTTileRef::FSTTileRef() :Row(0), Column(0) {}


FSTTileMapData::FSTTileMapData() 
{


}

//Tile Map Data
FSTTileMapData::~FSTTileMapData()
{
	std::list<FSTTileRef*>::iterator iter = TileDataList.begin();

	while (iter != TileDataList.end())
	{
		delete (*iter);
		(*iter) = NULL;
		++iter;
	}
}

bool FSTTileMapData::Parsing(LPCSTR FilePath)
{
	CToken_TileMap *DefinedToken = new CToken_TileMap;

	std::string GetString;
	WORD TokenType;
	WORD TokenFigure;

	CMYLexer* lexer = new CMYLexer(DefinedToken);

	//CMYLexer lexer(DefinedToken);
	if (!lexer->Open(FilePath))return FALSE;

	//parse Map Name
	lexer->FindToken(tenTM_MapName);
	lexer->GetNextToken(MapName);

	lexer->NextBlockStart();

	//Parse Cell Size
	lexer->FindToken(tenTM_CellWidth);
	lexer->GetWORD(Cell_Width);

	lexer->FindToken(tenTM_CellHeight);
	lexer->GetWORD(Cell_Height);

	//Parse Map Size
	lexer->FindToken(tenTM_Row);
	lexer->GetWORD(MapRow);

	lexer->FindToken(tenTM_Column);
	lexer->GetWORD(MapColumn);

	//Parse Tile Index
	lexer->FindToken(tenTM_TileData);

	lexer->NextBlockStart();

	while (lexer->FindToken_InBlock(tenTM_TileName))
	{
		FSTTileRef* tileRef = new FSTTileRef;
		//parse tile name		
		lexer->GetNextToken(GetString);
		tileRef->TileName = GetString;
		
		//parse Tile position
		lexer->FindToken(tenTM_TilePos);

		lexer->GetWORD(TokenFigure);
		tileRef->Row = TokenFigure;

		lexer->GetWORD(TokenFigure);
		tileRef->Column = TokenFigure;


		TileDataList.push_back(tileRef);
	}
	delete lexer;
	delete DefinedToken;
}

CTileMap* FSTTileMapData::CreateTileMap()
{
	CTileMap* NewMap = NULL;
	STTileSrc *TileSrc = NULL;
	NewMap = new CTileMap(MapRow, MapColumn, Cell_Width);

	std::list<FSTTileRef*>::iterator listiter;

	//int a=TileDataList.size();
	for (listiter = TileDataList.begin(); listiter != TileDataList.end(); ++listiter)
	{
		//Get Tile Data
		TileSrc = g_pDataBase->GetTile((*listiter)->TileName.data());

		//Set Tile
		NewMap->SetTile({ (*listiter)->Row, (*listiter)->Column }, TileSrc);
	}
	return NewMap;
}
