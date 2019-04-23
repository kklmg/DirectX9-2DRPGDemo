#pragma once
#include"MyLexer.h"
#include"Convert.h"

std::string g_CurStr;
WORD g_CurToken;
//-------------------------------------
//Class My Lexer
//-------------------------------------
CMYLexer::CMYLexer() :m_pDef(NULL)
{
}
CMYLexer::CMYLexer(CTokenDefine* def) : m_pDef(def)
{
}

CMYLexer::~CMYLexer()
{
	Close();
}

void CMYLexer::SetTokenDef(CTokenDefine* def)
{
	m_pDef = def;
}

bool CMYLexer::GetNextChar()
{
	if (m_File.peek() == EOF) return FALSE;

	m_File.seekg(0, std::ios::cur);
	m_File.read(&m_CurChar, 1);
	return TRUE;
}

BOOL CMYLexer::Open(LPCSTR p_strFileName)
{
	//try open file
	m_File.open(p_strFileName);

	if (!m_File.is_open())return FALSE;

	//Init seek position
	//m_File.seekg(std::ios::beg);
}

void CMYLexer::Close()
{
	if (m_File.is_open())
	{
		m_File.close();
	}
}

BOOL CMYLexer::Reset()
{
	m_File.seekg(std::ios::beg);
	return true;
}

DWORD CMYLexer::GetNextToken(std::string& GetString)
{
	GetString.clear();
	//get next Char
	if (!GetNextChar())	return g_CurToken = TOKEND_END;

	while (m_CurChar == ' ' || m_CurChar == '\t' || m_CurChar == '\r' || m_CurChar == '\n')
	{
		if (!GetNextChar())	return g_CurToken = TOKEND_END;
	}

	// ------------------------------------------------------------
	// 2. BLOCK 확인.
	// ------------------------------------------------------------
	// 1) '{'인가를 확인한다.
	if (m_CurChar == '{')
	{
		GetString = m_CurChar;
		return	g_CurToken = TOKEND_BLOCK_START;
	}

	// 2) '}'인가를 확인한다.
	else if (m_CurChar == '}')
	{
		GetString = m_CurChar;
		return	g_CurToken = TOKEND_BLOCK_END;
	}

	// ------------------------------------------------------------
	// 3. Reserved Word
	// ------------------------------------------------------------
	// - '*'인가를 확인한다.
	//   만약 '*'로 시작하는 것은 Reserved word이므로 검색한다!
	else if (m_CurChar == '*')
	{
		//no defination seted
		if (m_pDef == NULL) return g_CurToken = TOKEND_NOTDEFINED;

		int value;
		int	p_StringPoint = 0;

		while (m_CurChar != ' ' && m_CurChar != '\t' && m_CurChar != '\r'
			&& m_CurChar != '\n' && m_CurChar != '"' && m_CurChar != '{' && m_CurChar != '}')
		{
			m_strBuff[p_StringPoint] = m_CurChar;

			if (!GetNextChar())	return g_CurToken = TOKEND_END;

			++p_StringPoint;
		}
		m_strBuff[p_StringPoint] = NULL;

		GetString = m_strBuff;

		if (m_pDef->SearchData(m_strBuff, value))
			return g_CurToken = value;
		// 맞는 것은 전혀 찾지 못했다. 정의되지 않은 TOKEN이라고 돌려준다.
		else return	g_CurToken = TOKEND_NOTDEFINED;
	}


	// ------------------------------------------------------------
	// 4. 숫자인지를 확인한다.
	// ------------------------------------------------------------
	// - 만약에 '0'에서 '9'사이이거나 '-'부하고 붙어 있으면 숫자라고 판단을 한다.
		else if (m_CurChar >= '0' && m_CurChar <= '9' || m_CurChar == '-')
	{
		int		p_StringPoint = 0;

		while ((m_CurChar >= '0' && m_CurChar <= '9') || m_CurChar == '-' || m_CurChar == '.')
		{
			m_strBuff[p_StringPoint] = m_CurChar;

			if (!GetNextChar())	return g_CurToken = TOKEND_END;

			++p_StringPoint;
		}
		m_strBuff[p_StringPoint] = NULL;

		GetString = m_strBuff;
		return	g_CurToken = TOKEND_NUMBER;
	}


	// ------------------------------------------------------------
	// 5. String인지를 검사한다.
	// ------------------------------------------------------------
	// - '"' 따음표로 시작하면 이것은 String인다!
	else if (m_CurChar == '"')
	{
		int		p_StringPoint = 0;

		if (!GetNextChar())	return g_CurToken = TOKEND_END;


		while (m_CurChar != '"')
		{
			m_strBuff[p_StringPoint] = m_CurChar;

			if (!GetNextChar())	return g_CurToken = TOKEND_END;

			++p_StringPoint;
		}
		m_strBuff[p_StringPoint] = NULL;

		GetString = m_strBuff;
		return	g_CurToken = TOKEND_STRING;
	}
	//GetString = m_CurChar;
	// Error) 
	return	g_CurToken = TOKEND_NOTDEFINED;
}

void CMYLexer::NextBlockStart()
{
	do
	{
		if (g_CurToken == TOKEND_END)return;
		GetNextToken();
	} while (g_CurToken != TOKEND_BLOCK_START);
}
void CMYLexer::NextBlockEnd()
{
	do
	{
		if (g_CurToken == TOKEND_END)return;
		GetNextToken();
	} while (g_CurToken != TOKEND_BLOCK_END);
}

bool CMYLexer::FindToken(int FindToken)
{
	do
	{
		GetNextToken();
		if (g_CurToken == FindToken)return TRUE;
	} while (g_CurToken != TOKEND_END);
	return FALSE;
}
bool CMYLexer::FindToken_Until(int FindToken, int breaktoken)
{
	do
	{
		GetNextToken();
		if (g_CurToken == FindToken)return TRUE;
		if (g_CurToken == breaktoken)return FALSE;
	} while (g_CurToken != TOKEND_END);
	return FALSE;
}
bool CMYLexer::FindToken_InBlock(int FindToken)
{
	return FindToken_Until(FindToken, TOKEND_BLOCK_END);
}

bool CMYLexer::GetInt(int &Get) 
{
	GetNextToken();
	if (g_CurToken == TOKEND_END)return FALSE;
	int Data = Convert::StrToInt(g_CurStr.data());
	Get = Data;

	return TRUE;
}

bool CMYLexer::GetWORD(WORD &Get)
{
	GetNextToken();
	if (g_CurToken == TOKEND_END)return FALSE;
	WORD Data=Convert::StrToWord(g_CurStr.data());
	Get = Data;
	return TRUE;
}
bool CMYLexer::GetFloat(float &Get) 
{
	GetNextToken();
	if (g_CurToken == TOKEND_END)return FALSE;
	float Data= Convert::StrToFloat(g_CurStr.data());	
	Get = Data;

	return TRUE;
}