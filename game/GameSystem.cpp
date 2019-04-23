#include"GameSystem.h"
#include"InitWindow.h"
#include"InitDD.h"
#include"Global_data.h"
#include"blit.h"
#include"ddutil.h"
#include"effect.h"

CGameSystem::CGameSystem():m_pGameMng(NULL), m_DXInput(NULL)
{
	m_AppSetting.init();	
}

CGameSystem::~CGameSystem()
{
	ShutDown();
}

bool CGameSystem::Init(HINSTANCE hInstance, int nCmdShow)
{
	//Init window___________________________________	
	if (InitWindow(hInstance, nCmdShow) != TRUE)
		return FALSE;

	//Init DirectDraw________________________________
	DXW_Init();

	//Init Time Manager
	g_pTimeMng = new CTimeMng();
	g_pTimeMng->Init();

	//Init DataBase_________________________________
	g_pDataBase = new CDataBase;
	g_pDataBase->Init();

	//Init UI Manager
	g_pUIMng = new CUIManager();
	g_pUIMng->Init();


	//Init GameManager______________________________
	g_pGameMng = m_pGameMng = new CGameManager;
	m_pGameMng->Init();

	//Init Direct Input_____________________________
	m_DXInput = new CDirectxInput;
	m_DXInput->Init(hInstance,g_hWnd);

	return TRUE;
}

int CGameSystem::UpdateFrame() 
{
	MSG msg;
	HRESULT hRet;

	//Game Loop
	HCURSOR hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	SetCursor(hCursor);

	while (1)
	{
		//Message Loop_________________________________
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0)) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//---------------------------------------------
		
		else if (g_pGameSystem->m_AppSetting.bIsActive)
		{
			//clear previous surface____________________ 
			//_FillSurface(g_pDDSBack, 0x00000000);

			//Time Update
			g_pTimeMng->TimeUpdate();

			//check input
			m_DXInput->UpdateInputState();

			//Main Frame
			m_pGameMng->DrawFrame();

			//Display Infomation
			DXW_DisplayInfo();

			//DirectDraw Flip
#ifdef _DEBUG
			hRet = DDSWBlt();
#else
			//hRet = g_pDDSPrimary->Flip(NULL, 0);
			//hRet = g_pDDSPrimary->Flip(NULL, DDFLIP_WAIT);
			hRet = g_pDDSPrimary->Flip(NULL, DDFLIP_WAIT | DDFLIP_NOVSYNC);
			//hRet = g_pDDSPrimary->Flip(NULL, DDFLIP_WAIT|DDFLIP_INTERVAL2);	
#endif
			//if (hRet != DD_OK) { DDERRCHK(hRet); return; }


			//Clear keyboard input buff
			g_keyBuff.clear();
			g_keyBuff_figure.clear();	

			//adjust frame
			m_pGameMng->AdjustFrame();
		}
		else 
		{
			WaitMessage();
		}
	}
	TRACE1("===== Close Process ===== [ DxWFrame - %x]\n", ::GetCurrentProcessId());
	return msg.wParam;
}

void CGameSystem::TestInit() 
{	
	g_pDDSRect = DDLoadBitmap(g_pDD, "UI/rect.bmp", 0, 0);

	DDSetColorKey(g_pDDSRect, RGB(255,0,255));

	g_pDDSCircle = DDLoadBitmap(g_pDD, "UI/circle.bmp", 0, 0);

	DDSetColorKey(g_pDDSCircle, RGB(255, 0, 255));
	//StaBar = new StateBar("UI/layout.bmp", "UI/HPbar.bmp", "UI/MPbar.bmp", RGB(255, 255, 255));
}

void CGameSystem::ShutDown()
{
	SAFE_RELEASE(g_pDataBase);

	SAFE_RELEASE(m_pGameMng);

	SAFE_RELEASE(m_DXInput);

	SAFE_RELEASE(g_pUIMng);

	SAFE_RELEASE(g_pTimeMng);

}

STAPPSetting CGameSystem::getAppState()const 
{
	return m_AppSetting;
}

DWORD CGameSystem::GetFPS() 
{
	return m_pGameMng->GetFPS();
}

void CGameSystem::UpdateActive(bool IsActive)
{
	m_AppSetting.bIsActive = IsActive;
}