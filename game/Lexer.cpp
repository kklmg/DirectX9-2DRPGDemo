#include "Lexer.h"

ILexer::ILexer() :
	m_File(INVALID_HANDLE_VALUE),
	m_dwFileSize(0),
	m_NowProgress(0),
	m_Pointer(0),
	m_NowLoaded(0),
	m_dwBufferSize(0),
	m_pBuffer(NULL)
{
}

ILexer::~ILexer()
{
	Close();
}

char ILexer::GetNextChar()
{
	// ------------------------------------------------------------
	// CASE) Buffer의 끝에 다았으면 다시 File로 부터 Buffer만큼을 읽어 들인다.
	// ------------------------------------------------------------
	if (m_Pointer == m_NowLoaded)
	{
		// Declare) 
		BOOL	result;

		// 1) Read한다.
		result = ReadFile(m_File, m_pBuffer, m_dwBufferSize, &m_NowLoaded, NULL);

		// Check) 
		RETURN_IF(result == FALSE, 0);

		// Check) 
		RETURN_IF(m_NowLoaded == 0, 0);

		// 2) Reset한다.
		m_Pointer = 0;
	}


	// ------------------------------------------------------------
	// Buffer가 차지 않았을 경우 그냥 다음 글자를 돌려보낸다.
	// ------------------------------------------------------------
	// 진행...계산...
	++m_NowProgress;


	// Return) 다음 글자를 돌려준다.
	return	m_pBuffer[m_Pointer++];
}

BOOL ILexer::Open(LPCSTR p_FileName, DWORD p_dwBufferSize)
{
	// ------------------------------------------------------------
	// Check) 
	// ------------------------------------------------------------
	// 1) FileName String이 NULL이면 그냥 돌려보낸다.
	RETURN_IF(p_FileName == NULL, FALSE);

	// 2) Buffer의 Size가 0면 돌려보낸다.
	RETURN_IF(p_dwBufferSize == 0, FALSE);


	// ------------------------------------------------------------
	// Declare) 
	// ------------------------------------------------------------
	// 1) Buffer
	HANDLE	hFile = INVALID_HANDLE_VALUE;
	void*	pBuffer = NULL;
	DWORD	dwFileSize;

	try
	{
		// ------------------------------------------------------------
		// 1. File Open하기.
		// ------------------------------------------------------------
		// 1) File을 Open한다.
		hFile = CreateFile(p_FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		// Check) 파일을 열지 못했다.
		RETURN_IF(hFile == INVALID_HANDLE_VALUE, FALSE);

		// 2) 기본적인 변수들을 설정한다.
		dwFileSize = GetFileSize(m_File, NULL);


		// ------------------------------------------------------------
		// 2. 초기화 및 준비
		// ------------------------------------------------------------
		// 1) Buffer Size 생성.
		pBuffer = (char*)malloc(p_dwBufferSize);


		// ------------------------------------------------------------
		// 2. 초기화 및 준비
		// ------------------------------------------------------------
		// 1) File 정보를  설정한다.
		m_File = hFile;
		m_dwFileSize = dwFileSize;

		// 2) Buffer정보를 설정한다.
		m_pBuffer = (char*)pBuffer;
		m_dwBufferSize = p_dwBufferSize;

		// 3) 초기화
		Reset();
	}

	// ------------------------------------------------------------
	// Exception)
	// ------------------------------------------------------------
	catch (...)
	{
		// 1) File닫기.
		SAFE_CLOSEHANDLE(hFile);

		// 2) Buffer 지우기.
		SAFE_FREE(pBuffer);

		// Return) 실패....
		return	FALSE;
	}


	// Return) 성공!!!
	return	TRUE;
}

void ILexer::Close()
{
	// ------------------------------------------------------------
	// Check) 
	// ------------------------------------------------------------
	// 1) 파일이 열린 상태인지를 검사하여 열린상태가 아니라면 그냥 돌려보낸다.
	RETURN_IF(m_File == INVALID_HANDLE_VALUE, );

	// ------------------------------------------------------------
	// 1. 모두 닫는다.
	// ------------------------------------------------------------
	// 1) Buffer를 지워분진다.
	SAFE_FREE(m_pBuffer);
	m_dwBufferSize = 0;

	// 2) File을 닫는다.
	SAFE_CLOSEHANDLE(m_File);
}

BOOL ILexer::Reset()
{
	// ------------------------------------------------------------
	// Check) 
	// ------------------------------------------------------------
	// 1) 파일이 열린 상태인지를 검사한다.
	RETURN_IF(m_File == INVALID_HANDLE_VALUE, FALSE);


	// ------------------------------------------------------------
	// 1. 값들을 초기상태로 돌린다.
	// ------------------------------------------------------------
	// 1) 기본 변수를 Reset한다.
	m_NowProgress = 0;
	m_Pointer = 0;
	m_NowLoaded = 0;


	// ------------------------------------------------------------
	// 2. 값들을 초기상태로 돌린다.
	// ------------------------------------------------------------
	// Declare) 
	DWORD	dwResult;

	// 1) File의 Pointer를 제일 처음으로 되돌린다.
	dwResult = SetFilePointer(m_File, 0, 0, FILE_BEGIN);

	// Check) 읽기에 실패했으면 FALSE를 Return한다.
	RETURN_IF(dwResult == INVALID_SET_FILE_POINTER, FALSE);


	// Return) 성공!!
	return	TRUE;
}
