#pragma once
#ifndef __DXMNG_H__
#define __DXMNG_H__

class _DXMNG_H_ {};

#define DIRECTINPUT_VERSION 0x800

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <ddraw.h>
#include <dsound.h>
#include <dinput.h>

#include <crtdbg.h>

///// ���� �Լ�
extern void ___Trace0(char *str);
extern void ___Trace(char *format, ...);

extern BOOL ___Error(char    *str, char *file, int line);
extern BOOL ___DDError(HRESULT hErr, char *file, int line);
extern BOOL ___DSError(HRESULT hErr, char *file, int line);



///// ����
#define DDERRCHK( hr )				___DDError( hr, __FILE__, __LINE__ )
#define DSERRCHK( hr )				___DSError( hr, __FILE__, __LINE__ )
#define MSGERROR( s )				___Error  ( s,  __FILE__, __LINE__ )


///// ����� ���
#ifdef _DEBUG
#include <assert.h>

//#define new DEBUG_NEW
#define malloc( size_t )	_malloc_dbg( size_t , _NORMAL_BLOCK, __FILE__, __LINE__ )

#define THIS_FILE                   __FILE__
#define ASSERT(e)                   assert(e)
#define ASSERT_VALID(o)             if (!o) assert(FALSE)

#define TRACE(s)                    ___Trace0(s)
#define TRACE0(s)                   ___Trace0(s)
#define TRACE1(f, a1)               ___Trace (f, a1)
#define TRACE2(f, a1, a2)           ___Trace (f, a1, a2)
#define TRACE3(f, a1, a2, a3)       ___Trace (f, a1, a2, a3)
#define TRACE4(f, a1, a2, a3, a4)   ___Trace (f, a1, a2, a3, a4)

#else // RELEASE == #ifdef !(_DEBUG) 

///// ������ ���

#define ASSERT(e)                   ((void)0)
#define ASSERT_VALID(o)             ((void)0)

#define TRACE(s)                    ((void)0)
#define TRACE0(s)                   ((void)0)   
#define TRACE1(f, a1)               ((void)0)
#define TRACE2(f, a1, a2)           ((void)0)
#define TRACE3(f, a1, a2, a3)       ((void)0)
#define TRACE4(f, a1, a2, a3, a4)   ((void)0)

#endif // _DEBUG

// #include "template.h"

#endif // __DXMNG_H__