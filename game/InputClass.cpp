#include"InputClass.h"

bool g_bLockInput = FALSE;

char g_cKeyStateBuffer[256] = { 0 };
char g_cPreKeyStateBuffer[256] = { 0 };

 DIMOUSESTATE g_diMouseState;
 DIMOUSESTATE g_PrediMouseState;


 std::string __GetInputFromDIKeyBoard()
 {
	std::string GetChar;
	int i;
	for (i = 0; i < 256; ++i)
	{
		if (g_cPreKeyStateBuffer[i] & 0x80)
		{
			GetChar += g_cKeyStateBuffer[i];
		}
	}
	return GetChar;
 }

 std::string __GetInputFromDIKeyBoard_Once()
 {
	std::string GetChar;
	int i;
	for (i = 0; i < 256; ++i)
	{
		if ((g_cKeyStateBuffer[i] & 0x80) && (!(g_cPreKeyStateBuffer[i] & 0x80)))
		{
			GetChar += g_cKeyStateBuffer[i];
		}
	}
	return GetChar;
 }


CDirectxInput::CDirectxInput()
	:m_pDirectInput(NULL),
	m_pMouseDevice(NULL),
	m_pKeyboardDevice(NULL)
{}

CDirectxInput::~CDirectxInput()
{
	if (m_pDirectInput != NULL)
	{
		m_pDirectInput->Release();
		m_pDirectInput = NULL;
	}
	if (m_pMouseDevice != NULL)
	{
		m_pMouseDevice->Release();
		m_pMouseDevice = NULL;
	}
	if (m_pKeyboardDevice != NULL)
	{
		m_pKeyboardDevice->Release();
		m_pKeyboardDevice = NULL;
	}
	if (m_pDirectInput != NULL)
	{
		m_pDirectInput->Release();
		m_pDirectInput = NULL;
	}
}

bool CDirectxInput::Init(HINSTANCE hInstance, HWND hwnd)
{
	//Init IDirectInput8 interface
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL)))
		return E_FAIL;
	
	

	//Init Keybord_____________________________________________________________________________
	if (FAILED(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboardDevice, NULL)))
		return E_FAIL;

	//Set DataFormat and CooperateLel	
	m_pKeyboardDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	
	//Get Control Right
	m_pKeyboardDevice->Acquire();
	m_pKeyboardDevice->Poll();


	//Init Mouse__________________________________________________________________________________
//	if (FAILED(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouseDevice, NULL)))
	//	return E_FAIL;

	//Set DataFormat and CooperateLel
	//m_pMouseDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	//m_pMouseDevice->SetDataFormat(&c_dfDIMouse);

	
	//Get Control Right
	//m_pMouseDevice->Acquire();
	//m_pMouseDevice->Poll();
	
	//----------------------------------------------------------------------------------------------
	return S_OK;
}

bool CDirectxInput::ReadDevice(IDirectInputDevice8*pDIDevice, void* pBuffer, long lSize)
{
	HRESULT hr;
	while (true)
	{
		pDIDevice->Poll();             //poll device
		pDIDevice->Acquire();          //Get Control Right
		if (SUCCEEDED(hr = pDIDevice->GetDeviceState(lSize, pBuffer))) break;
		if (hr != DIERR_INPUTLOST || hr != DIERR_NOTACQUIRED) return FALSE;
		if (FAILED(pDIDevice->Acquire())) return FALSE;
	}
	return TRUE;
}

void CDirectxInput::UpdateInputState()
{
	//Save Pre Mouse State
	//CopyMemory(m_PrediMouseState, m_diMouseState, sizeof(m_diMouseState));

	//Update Mouse State
	//::ZeroMemory(&m_diMouseState, sizeof(m_diMouseState));
	//ReadDevice(m_pMouseDevice, (LPVOID)&m_diMouseState, sizeof(m_diMouseState));



	//Save Pre key State
	CopyMemory(g_cPreKeyStateBuffer, g_cKeyStateBuffer, sizeof(g_cKeyStateBuffer));
	
	//Update key State
	::ZeroMemory(g_cKeyStateBuffer, sizeof(g_cKeyStateBuffer));	
	ReadDevice(m_pKeyboardDevice, (LPVOID)g_cKeyStateBuffer, sizeof(g_cKeyStateBuffer));
}

bool CDirectxInput::mousedown() 
{

	if (g_diMouseState.rgbButtons[0] & 0x80)return TRUE;
	else return FALSE;

	/*if(m_cKeyStateBuffer[DIK_J] & 0x80)return TRUE;
	else return FALSE;*/
}