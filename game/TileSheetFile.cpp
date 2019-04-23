#include"TileSheetFile.h"
#include"MyLexer.h"

//Tile Sheet Parsing
bool FSTTileSheetGroup::Parsing(LPCSTR FilePath)
{
	CToken_TileSheet DefinedToken;
	std::string GetStrKey;
	std::string GetPath;
	WORD TokenType;

	CMYLexer lexer(&DefinedToken);
	if (!lexer.Open(FilePath))return FALSE;

	//parse Tile Sheet
	lexer.FindToken(tenTS_Data);
	lexer.NextBlockStart();

	while (lexer.FindToken_InBlock(tenTS_Name))
	{
		lexer.GetNextToken(GetStrKey);

		FSTTileSheet* newData = new FSTTileSheet;
		newData->SheetName = GetStrKey;
		lexer.FindToken(tenTS_PathGroup);
		lexer.NextBlockStart();

		//Parse File Path
		while (lexer.FindToken_InBlock(tenTS_Path))
		{
			lexer.GetNextToken(GetPath);
			newData->FilePath.push_back(GetPath);
		}

		//Parse Sheet Size
		lexer.FindToken(tenTS_Size);

		lexer.GetWORD(newData->Row);
		lexer.GetWORD(newData->Column);

		m_DataCon.AddData(GetStrKey.data(), newData);
	}
	int ct=m_DataCon.GetSize();
	return TRUE;
}