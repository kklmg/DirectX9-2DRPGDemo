#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

#endif
#pragma comment (lib, "dxguid")
#pragma comment (lib, "ddraw")

//---------------------------
// include head file
//---------------------------
#include<Windows.h>
#include<iostream>
#include <crtdbg.h>

//---------------------------
// include my head file
//---------------------------
#include"Global_data.h"
#include"ddutil.h"
#include"GameMng.h"

using namespace std;

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)

{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(8921);

	//Set Random Seed________________________________
	srand((int)time(0));

	//Init GameSystem________________________________
	g_pGameSystem = new CGameSystem;
	g_pGameSystem->Init(hInstance, nCmdShow);
	g_pGameSystem->TestInit();

	//perform Game Main Loop
	g_pGameSystem->UpdateFrame();

	//Shut down Game;
	SAFE_RELEASE(g_pGameSystem);
}
 

long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT  hRet;
	POINT PtMouse;
	
	//Update Mouse
	GetCursorPos(&PtMouse);
	ScreenToClient(hWnd, &PtMouse);
	g_Mouse.UpdataMouse(PtMouse);

	
	

	switch (message)
	{
	//--------------------------------------------
	//Message: windwos Create
	//--------------------------------------------
	case WM_CREATE:
		SetCapture(hWnd);
		return 0;

	case WM_ACTIVATE:
		g_pGameSystem->UpdateActive(!((BOOL)HIWORD(wParam)));
		break;
	//------------------------------------------
	//Mouse Event
	//------------------------------------------
	case WM_LBUTTONDOWN:
		g_Mouse.LeftKeyDown();
		break;
	case WM_LBUTTONUP:
		g_Mouse.LeftKeyUp();
		break;
	case WM_RBUTTONDOWN:
		g_Mouse.RightKeyDown();
		break;
	case WM_RBUTTONUP:
		g_Mouse.RightKeyUp();
		break;
	case WM_MOUSEMOVE:
		break;
		
		


	//---------------------------------------------||

	case WM_DROPFILES:		
	break;

	//------------------------------------------
	//KeyBoard Event
	//------------------------------------------
	case WM_KEYDOWN:
	{		
		//if(wParam>VK_0)
		if (wParam >= 0x30 && wParam <= 0x39)
		{
			g_keyBuff += char(wParam);
			g_keyBuff_figure += wParam;
		}
		if (wParam >= 0x41 && wParam <= 0x5A)
		{
			g_keyBuff += wParam;
		}
		// Handle any non-accelerated key commands
		switch (wParam)
		{
			case VK_SPACE:
			{
				bShowMapInfo = !bShowMapInfo;
			}break;

			case VK_ESCAPE:
				PostMessage(hWnd, WM_CLOSE, 0, 0);
				return 0L;
		}break;		
	}
	case WM_KEYUP:
	{
		/*
		* VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
		* 0x40 : unassigned
		* VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
		*/


	
	}break;


	case WM_SETCURSOR:
	{
		//Turn off the cursor since this is a full-screen app
		//HCURSOR
		//SetCursor();
		//ShowCursor(TRUE);
		return TRUE;
		break;
	}
	case WM_TIMER:
		break;



//------------------------------------------
//Close Program
//------------------------------------------
	case WM_DESTROY:
	{
		//ReleaseAllObjects();
		PostQuitMessage(0);
		return 0L;//long 0
	}

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
