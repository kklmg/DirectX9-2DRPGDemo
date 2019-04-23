#pragma once
#include<Windows.h>
//-----------------------------------------------------------------------------
//  Common Lexer
//  
//  1) Common Lexer란...
//     - 기본적인 File의 Open/Clse/Reset기능과 다음 Characater를 얻을 수 있는
//       기능만 제공하는 Class임.
//     - Buffering을 수행하므로 일반 fgetc()를 통한 것보다 빠른 속도를 가진다.
//     - 넘져진 p_TokenString에는 그엥 해당하는 String을 돌려준다.
//
//  2) GetToken()함수.
//     - 가장 핵심함수로 이 Lexer를 상속받아 반드시 재정의 해주어야한다.
//     - Return value는 Token의 종류에 해당하는 숫자를 돌려주며..
//     - p_TokenString에는 해당 Token의 문자열을 복사해서 넣어주는 기능을 하도록
//       제작해야한다.
//  
//-----------------------------------------------------------------------------
// Include) 
#include "CGDefinitions.h"
#include "TokenDefine.h"

class ILexer
{
	// ****************************************************************************
	// Constructor/Destructor)
	// ----------------------------------------------------------------------------
public:
	ILexer();
	virtual ~ILexer();


	// ****************************************************************************
	// Public)
	// ----------------------------------------------------------------------------
public:
	// 1) File Open/Close/Reset함수.
	BOOL				Open(LPCSTR p_strFileName, DWORD p_dwBufferSize = MAX_SIZE_OF_BUFFER);	// 파일을 연다.
	void				Close();			// 닫아버린다.
	BOOL				Reset();			// 처음부터 다시 시작한다.

											// 2) Token을 얻는 함수.(반드시 재정의 해줘야 한다.)
	virtual	int					GetToken(LPSTR p_TokenString) = 0;


	// ****************************************************************************
	// Implementation)
	// ----------------------------------------------------------------------------
private:
	// 1) 파일을 읽어서 작업하기 위한 변수들
	HANDLE				m_File;				// 파일의 핸들

											// 2) Buffer Infos~~
	DWORD				m_Pointer;			// 현재 Buffer상에 읽고 있는 문자열의 Pointer
	DWORD				m_NowLoaded;		// 현재 읽어 들여놓은 크기

	DWORD				m_dwBufferSize;		// Buffer의 크기
	char*				m_pBuffer;			// 파일의 내용을 읽어 들인 Buffer

	DWORD				m_dwFileSize;		// 파일의 전체 크기
	DWORD				m_NowProgress;		// 현재 진행.

protected:
	// 1) 다음 Character를 얻어오는 함수.(Lexer 내부적으로만 사용됨.)
	char				GetNextChar();
};
