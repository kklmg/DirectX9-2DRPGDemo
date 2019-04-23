
#include "DialogLexer.h"


DIALOG::CLexer::CLexer(LPCSTR p_FileName, DWORD p_SizeOfBuffer)
{
	// 1) Open한다.
	if (p_FileName != NULL)
	{
		Open(p_FileName, p_SizeOfBuffer);
	}
}

DIALOG::CLexer::~CLexer()
{
}

//Lexical Analysis
int DIALOG::CLexer::GetToken(LPSTR p_TokenString)
{
	// ------------------------------------------------------------
	// 1. 앞쪽 쓸데없는 문자찾기
	// ------------------------------------------------------------
	// - 앞쪽에 공백이나 Tab이나 개행이나 캐리지리턴 같은 문자열이 있으면 무시한다.
	CHAR	Char = GetNextChar();
	while (Char == ' ' || Char == '\t' || Char == '\r' || Char == '\n')
	{
		Char = GetNextChar();

		// Check) 끝인지를 확인한다.
		RETURN_IF(!Char, TOKEND_END);
	}


	// ------------------------------------------------------------
	// 2. BLOCK 확인.
	// ------------------------------------------------------------
	// 1) '{'인가를 확인한다.
	if (Char == '{')
	{
		p_TokenString[0] = '{';
		p_TokenString[1] = NULL;

		return	TOKEND_BLOCK_START;
	}

	// 2) '}'인가를 확인한다.
	else if (Char == '}')
	{
		p_TokenString[0] = '}';
		p_TokenString[1] = NULL;

		return	TOKEND_BLOCK_END;
	}


	// ------------------------------------------------------------
	// 3. Reserved Word
	// ------------------------------------------------------------
	// - '*'인가를 확인한다.
	//   만약 '*'로 시작하는 것은 Reserved word이므로 검색한다!
	else if (Char == '*')
	{
		int		p_StringPoint = 0;
		while (Char != ' ' && Char != '\t' && Char != '\r' && Char != '\n' && Char != '"' && Char != '{' && Char != '}')
		{
			p_TokenString[p_StringPoint] = Char;

			Char = GetNextChar();

			// Check) 끝인지를 확인한다.
			RETURN_IF(!Char, TOKEND_END);

			++p_StringPoint;
		}
		p_TokenString[p_StringPoint] = NULL;

		for (int i = 0; i<eDigToken_MAX; ++i)
		{
			if (!strcmp(p_TokenString, DigToken[i]))
			{
				// 맞는 Token을 찾았다! 그 번호를 return시켜준다.
				return	i;
			}
		}

		// 맞는 것은 전혀 찾지 못했다. 정의되지 않은 TOKEN이라고 돌려준다.
		return	TOKEND_NOTDEFINED;
	}


	// ------------------------------------------------------------
	// 4. 숫자인지를 확인한다.
	// ------------------------------------------------------------
	// - 만약에 '0'에서 '9'사이이거나 '-'부하고 붙어 있으면 숫자라고 판단을 한다.
	else if (Char >= '0' && Char <= '9' || Char == '-')
	{
		int		p_StringPoint = 0;
		while ((Char >= '0' && Char <= '9') || Char == '-' || Char == '.')
		{
			p_TokenString[p_StringPoint] = Char;

			Char = GetNextChar();

			// Check) 끝인지를 확인한다.
			RETURN_IF(!Char, TOKEND_END);

			++p_StringPoint;
		}
		p_TokenString[p_StringPoint] = NULL;


		return	TOKEND_NUMBER;
	}


	// ------------------------------------------------------------
	// 5. String인지를 검사한다.
	// ------------------------------------------------------------
	// - '"' 따음표로 시작하면 이것은 String인다!
	else if (Char == '"')
	{
		Char = GetNextChar();

		// Check) 끝인지를 확인한다.
		RETURN_IF(!Char, TOKEND_END);

		int		p_StringPoint = 0;
		while (Char != '"')
		{
			p_TokenString[p_StringPoint] = Char;

			Char = GetNextChar();

			// Check) 끝인지를 확인한다.
			RETURN_IF(!Char, TOKEND_END);

			++p_StringPoint;
		}
		p_TokenString[p_StringPoint] = NULL;

		return	TOKEND_STRING;
	}


	// Error) 
	return	TOKEND_NOTDEFINED;
}
