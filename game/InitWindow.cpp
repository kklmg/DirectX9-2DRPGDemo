#include"InitWindow.h"
#include"Global_data.h"
BOOL InitWindow(HINSTANCE hInstance, int nCmdShow) 
{
	WNDCLASS wc;
	HRESULT hRet;
	
	// Set up and register window class
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NAME;
	wc.lpszClassName = NAME;
	RegisterClass(&wc);

	DWORD ExWinStyle, WinStyle;

#ifdef _DEBUG
	ExWinStyle = NULL;
	WinStyle = WS_POPUPWINDOW | WS_CAPTION;
#else
	ExWinStyle = WS_EX_TOPMOST;
	WinStyle = WS_POPUP;
#endif

	// Create a window
	g_hWnd = CreateWindowEx(ExWinStyle,
		NAME,
		TITLE,
		WinStyle,
		0,
		0,
		HRES, //GetSystemMetrics(SM_CXSCREEN),
		VRES, //GetSystemMetrics(SM_CYSCREEN),
		NULL,
		NULL,
		hInstance,
		NULL);
	if (!g_hWnd)
		return FALSE;
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	HANDLE hProc = ::GetCurrentProcess();
#ifdef _DEBUG
	::SetPriorityClass(hProc, NORMAL_PRIORITY_CLASS);
#else
	::SetPriorityClass(hProc, HIGH_PRIORITY_CLASS);
#endif

	SetFocus(g_hWnd);

	//ShowCursor(TRUE);
	return TRUE;
}