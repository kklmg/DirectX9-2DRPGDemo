#include"GameMng.h"
#include"Global_data.h"

CGameManager::CGameManager():
	m_pFpsMng(NULL), 
	m_pSceneMng(NULL), 
	m_pHHSound(NULL),
	m_pTileEditor(NULL), 
	m_pTileMapEditor(NULL), 
	m_emGameScene(emGameScene::eMenu),
	m_pGameOver(NULL),
	m_pMenu(NULL)
{}

CGameManager::~CGameManager()
{
	if (m_pFpsMng != NULL)
	{
		delete	m_pFpsMng;
		m_pFpsMng = NULL;
	}
	if (m_pSceneMng != NULL)
	{
		delete	m_pSceneMng;
		m_pSceneMng = NULL;
	}
	if (m_pHHSound != NULL)
	{
		delete	m_pHHSound;
		m_pHHSound = NULL;
	}
	if (m_pTileEditor != NULL)
	{
		delete	m_pTileEditor;
		m_pTileEditor = NULL;
	}
	if (m_pTileMapEditor != NULL)
	{
		delete	m_pTileMapEditor;
		m_pTileMapEditor = NULL;
	}
	if (m_pMenu != NULL)
	{
		delete	m_pMenu;
		m_pMenu = NULL;
	}
	if (m_pGameOver != NULL)
	{
		delete	m_pGameOver;
		m_pGameOver = NULL;
	}
}

bool CGameManager::Init() 
{
	m_pHHSound = new CHHSound;
	m_pHHSound->Init(g_hWnd);

	m_pFpsMng = new CFpsMng;
	m_pFpsMng->SetFPS(FPS);
	
	if (g_emGAMEMODE ==emGameMode::eGameMode_Normal)
	{
		m_pMenu = new CMenu();
		m_pMenu->Init();
	}
	else if (g_emGAMEMODE == emGameMode::eGameMode_TileEditror)
	{
		m_pTileEditor = new CTileEditor(50, 8, 8, "Common", 1, "map/tile/TileB.bmp");
	}
	else if (g_emGAMEMODE == emGameMode::eGameMode_TileMapEditor)
	{
		m_pTileMapEditor = new CTileMapEditor();
		//m_TileMapEditor->CreateTileMap(50, 50, 24, 34,"Town");
		m_pTileMapEditor->LoadTileMap("Data/Map/Town");
	}

	
//	m_pSceneMng->InitSound(m_HHSound);

	
	
	return TRUE;
}

void CGameManager::CallMenuScene() 
{
	if (m_emGameScene == emGameScene::eOver) 
	{
		m_emGameScene = emGameScene::eMenu;

		//call menu Scene
		if (m_pGameOver != NULL)
		{
			delete	m_pGameOver;
			m_pGameOver = NULL;
		}
		//Init menu
		m_pMenu = new CMenu();
		m_pMenu->Init();

	}
}
void CGameManager::CallPlayScene() 
{
	if (m_emGameScene == emGameScene::eMenu)
	{
		m_emGameScene = emGameScene::eRun;

		//release menu
		if (m_pMenu != NULL)
		{
			delete	m_pMenu;
			m_pMenu = NULL;
		}

		//call play Scene
		m_pSceneMng = new CSceneMng;
		m_pSceneMng->Init();
	}
}
void CGameManager::CallGameOver() 
{
	if (m_emGameScene == emGameScene::eRun)
	{
		m_emGameScene = emGameScene::eOver;

		//release play scene
		if (m_pSceneMng != NULL)
		{
			delete	m_pSceneMng;
			m_pSceneMng = NULL;
		}

		//call gameover Scene
		m_pGameOver = new COverSce;
		m_pGameOver->Init();
	}
}

void CGameManager::QuitGame() 
{
	PostQuitMessage(0);
}

bool CGameManager::DrawFrame()
{
	if (g_emGAMEMODE == emGameMode::eGameMode_Normal)
	{
		switch (m_emGameScene)
		{
		case CGameManager::eMenu:m_pMenu->DrawMenu(NULL,g_pDDSBack);
			break;
		case CGameManager::eRun: m_pSceneMng->DrawScene();
			break;
		case CGameManager::eOver:m_pGameOver->Draw(NULL, g_pDDSBack);
			break;
		default:
			break;
		}

	}
	else if (g_emGAMEMODE == emGameMode::eGameMode_TileEditror)
	{
		m_pTileEditor->Update();
		m_pTileEditor->Draw(g_pDDSBack);
		m_pTileEditor->DrawTileInfo(g_pDDSBack);
	}
	else if (g_emGAMEMODE == emGameMode::eGameMode_TileMapEditor)
	{
		m_pTileMapEditor->Update();
		m_pTileMapEditor->Draw(g_pDDSBack);
	}
	else return FALSE;
	return TRUE;
}

DWORD CGameManager::GetFPS()
{
	return m_pFpsMng->GetFPS();
}


void CGameManager::AdjustFrame() 
{
	m_pFpsMng->FrameWaiting();
}
//Init sound_____________________________________
