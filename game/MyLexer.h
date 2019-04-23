#pragma once
#include<Windows.h>
#include<fstream>
#include<map>
#include<string>

#include"TokenDefine.h"


extern std::string g_CurStr;
extern WORD g_CurToken;
//-------------------------------
//Class My Lexer
//-------------------------------
class CMYLexer
{
public:
	CMYLexer();
	CMYLexer(CTokenDefine*);
	virtual ~CMYLexer();

public:
	BOOL Open(LPCSTR p_strFileName);
	void Close();
	BOOL Reset();

	void SetTokenDef(CTokenDefine* def);

	DWORD GetNextToken(std::string& GetString= g_CurStr);
	
	void NextBlockStart();
	void NextBlockEnd();

	bool FindToken(int FindToken);
	bool FindToken_Until(int FindToken, int breaktoken);
	bool FindToken_InBlock(int FindToken);

	bool CMYLexer::GetInt(int &Get);
	bool CMYLexer::GetWORD(WORD &Get);
	bool CMYLexer::GetFloat(float &Get);

public:
	std::string m_CurStr;
	WORD m_wCurToken;

private:
	bool GetNextChar();

	std::ifstream m_File;

	char m_strBuff[255];

	char m_CurChar;

	

	CTokenDefine* m_pDef;
};


