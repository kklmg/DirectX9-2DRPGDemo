#pragma once
#include"FpsMng.h"
#include"HHSound.h"

#include"Menu.h"
#include"SceneMng.h"
#include"GameOver.h"

#include"TileMapEditor.h"
#include"TileEditor.h"

class CGameManager
{
	enum emGameScene
	{
		eMenu,
		eRun,
		eOver,
	};

public:
	CGameManager();
	~CGameManager();

	bool Init();

	void CallMenuScene();
	void CallPlayScene();
	void CallGameOver();
	void QuitGame();

	bool DrawFrame();

	DWORD GetFPS();

	void AdjustFrame();
private:
	//CTileMapMng *m_pMapMng;
	//collideMng *m_pColMng;
	CFpsMng *m_pFpsMng;

	CSceneMng *m_pSceneMng;

	CHHSound *m_pHHSound;

	CTileEditor* m_pTileEditor;

	CTileMapEditor* m_pTileMapEditor;

	emGameScene m_emGameScene;

	CMenu *m_pMenu;

	COverSce *m_pGameOver;
};

