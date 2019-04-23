#pragma once
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")  
#pragma comment(lib, "dxguid.lib")
#include<string>

extern bool g_bLockInput;

//static char g_cKeyStateBuffer[256];
extern char g_cKeyStateBuffer[256];
extern char g_cPreKeyStateBuffer[256];

extern DIMOUSESTATE g_diMouseState;
extern DIMOUSESTATE g_PrediMouseState;


std::string __GetInputFromDIKeyBoard();

std::string __GetInputFromDIKeyBoard_Once();

class CDirectxInput
{
public:
	CDirectxInput();
	~CDirectxInput();

	bool Init(HINSTANCE hInstance, HWND hwnd);	

	void UpdateInputState();
	bool mousedown();

private:
	bool ReadDevice(IDirectInputDevice8*pDIDevice, void* pBuffer, long lSize);

private:
	LPDIRECTINPUT8			m_pDirectInput;
	LPDIRECTINPUTDEVICE8    m_pMouseDevice;
	LPDIRECTINPUTDEVICE8    m_pKeyboardDevice;
};

#define DIKEYDOWN(n) ((g_cKeyStateBuffer[n] & 0x80)&&(!g_bLockInput))
#define DIPREKEYDOWN(n) ((g_cPreKeyStateBuffer[n] & 0x80)&&(!g_bLockInput))
#define DIKEYDOWNONCE(n)((DIKEYDOWN(n)) && (!(DIPREKEYDOWN(n))))

#define GETDICHAR __GetInputFromDIKeyBoard();
#define GETDICHARONCE(n) 10