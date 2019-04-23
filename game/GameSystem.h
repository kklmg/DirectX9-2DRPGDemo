#pragma once
#include"GameMng.h"
#include"HHSound.h"
#include"DataBase.h"
#include"InputClass.h"
typedef struct tagAPP_SET
{
	BOOL bIsPause;
	BOOL bIsActive;
	BOOL bIsDisplayInfo;
	BOOL bIsRenderBlank;
	int  nBitPixel;

	void init() 
	{
		bIsActive = TRUE;
		bIsDisplayInfo = TRUE;
		bIsPause = FALSE;
		bIsRenderBlank = FALSE;
	}
} STAPPSetting;

class CGameSystem
{
public:
	CGameSystem();
	~CGameSystem();
	
	bool Init(HINSTANCE hInstance,int nCmdShow);	

	void TestInit();

	void ShutDown();

	int UpdateFrame();

	DWORD GetFPS();

	void UpdateActive(bool);

	STAPPSetting getAppState() const;

private:
	STAPPSetting m_AppSetting;

	CGameManager *m_pGameMng;

	CDirectxInput*m_DXInput;
	//CDataBase *m_pDataBase;
};
