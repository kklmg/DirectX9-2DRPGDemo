#include"TileFile.h"
#include"MyLexer.h"
#include"Global_data.h"



bool FSTTileDataGroup::Parsing(LPCSTR FilePath)
{
	CToken_Tile DefinedToken;
	std::string GetStrKey;
	WORD TokenType;

	CMYLexer lexer(&DefinedToken);
	if (!lexer.Open(FilePath))return FALSE;

	//parse Tile Sheet
	while (lexer.FindToken_Until(tenT_TileData, TOKEND_END))
	{
		FSTTileData* newData = new FSTTileData;

		lexer.NextBlockStart();
		//Parse Tile Name
		lexer.FindToken_InBlock(tenTS_Name);
		lexer.GetNextToken(GetStrKey);
		newData->TileName = GetStrKey;

		//Parse Reference TileSheet
		lexer.FindToken_InBlock(tenT_REFSheet);
		lexer.GetNextToken(newData->RefTileSheet);

		//Parse RefPosition 
		lexer.FindToken_InBlock(tenT_REFPosition);
		lexer.GetWORD(newData->RefRow);
		lexer.GetWORD(newData->RefColumn);

		//Parse Tile Type
		lexer.FindToken_InBlock(tenT_TileType);
		lexer.GetWORD(newData->TileType);

		//Parse Tile Obstacle
		lexer.FindToken_InBlock(tenT_TileObstacle);
		lexer.GetWORD(newData->TileObsLevel);

		//Insert Data
		m_DataCon.AddData(GetStrKey.data(), newData);

		//End
		lexer.NextBlockEnd();
	}

	//test
	int test = m_DataCon.GetSize();

	return TRUE;
}


