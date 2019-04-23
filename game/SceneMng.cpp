//-----------------------------------------------------------
//Include Head File
//-----------------------------------------------------------
#include<stdarg.h>
#include<algorithm>

//-----------------------------------------------------------
//Include My Head File
//-----------------------------------------------------------
#include"SceneMng.h"

#include"Global_data.h"

#include"TileMapFile.h"

#include"InitDD.h"
#include"RectMath.h"
#include"blit.h"
#include"EnumGroup.h"
#include"damage.h"
#include"effect.h"
#include"Astar.h"

//Global Scene Link List
list<CSceneLink*>g_list_maplink;


//-----------------------------------------------------------
//Class Scene Manager Init
//-----------------------------------------------------------
CSceneMng::CSceneMng():
	m_bSceneLoaded(FALSE), 
	m_pSceneSuf(NULL),
	m_pMap(NULL), 
	m_pCamera(NULL),
	m_pObjMng(NULL), 
	m_pEffectMng(NULL), 
	m_pDigDataMng(NULL),
	m_bPause(FALSE),
	m_bSceChanging(FALSE), 
	m_pDestScene(NULL){}

CSceneMng::~CSceneMng()
{
	//ClearAll();
	ClearCurSceneLink();

	SAFE_RELEASE(g_pQuadTree);

	if (m_pMap != NULL)
	{
		delete m_pMap;
		m_pMap = NULL;
	}

	if (m_pSceneSuf != NULL)
	{
		m_pSceneSuf->Release();
		m_pSceneSuf = NULL;
	}

	if (m_pCamera != NULL)
	{
		delete m_pCamera;
		m_pCamera = NULL;
	}

	if (m_pObjMng != NULL)
	{
		delete m_pObjMng;
		m_pObjMng = NULL;
	}
	SAFE_RELEASE(g_pEventMng);

	SAFE_RELEASE(g_pAIMng);

	SAFE_RELEASE(m_pEffectMng);

	SAFE_RELEASE(m_pDigDataMng);
}

bool CSceneMng::Init()
{
	m_pObjMng = new CObjectMng();
	m_pObjMng->Init();

	g_pEventMng = new CEventManager();

	g_pAIMng = new CAIManager();
	g_pAIMng->Init();

	m_pEffectMng = new CEffectMng();
	m_pEffectMng->Init();

	m_pDigDataMng= new CDialogueDataMng();
	m_pDigDataMng->Parsing("Dialogue/Dialogue.txt");

	InitSceneContainer();

	//default scene;
	LoadScene(eSceTown);

	PreCreate();

	//LoadScene(eSceTown);
	//LoadScene(eInHome);

	//CLivingObj *knight =
	//	m_pObjMng->CreateHero(eKnight,
	//		rand() % GetTileMap()->GetMapWidth(),
	//		rand() % GetTileMap()->GetMapHeight(),eForFriend,TRUE);
	//g_pControledObj = knight;

	
	//warrior->m_pAreaCollide->SetCollideStrength(enPhysicalStrength::epyStrong);
	//m_pObjMng->UpdateControlObj();

	//g_pAIMng->Add_CommonAI_Hero(knight);
	//g_pAIMng->Add_CommonAI_Hero(archer);
	//g_pAIMng->Add_CommonAI_Hero(magician);
	//g_pAIMng->Add_CommonAI_Hero(warrior);
	//g_pAIMng->Add_CommonAI_Hero(assassin);

	//m_pAIMng->CreateBev_CommonAttack(knight);

	//m_pAIMng->CreateBev_CommonAttack(archer);	
	//m_pAIMng->CreateBev_CommonAttack(magician);
	//m_pAIMng->CreateBev_CommonAttack(warrior);
	//m_pAIMng->CreateBev_CommonAttack(assassin);
	
	//m_pAIMng->AddBev_FollowEX(archer, m_pObjMng->GetControledObj());

	
	//testtext = new CTextSuf("welcome !!",10,0,g_pUIMng->GetTextSheet());
	
	//g_enPixelFormat = __CheckRGBBit(m_pSceneSuf);
	
	return TRUE;
}

void CSceneMng::PreCreate()
{
	int cr_x = 400;
	int cr_y = 400;

	CLivingObj *knight =
		m_pObjMng->CreateHero(eKnight,
			cr_x,
			cr_y, eForFriend, TRUE);
	g_pControledObj = knight;
	knight->SetDialogue(m_pDigDataMng->GetDialogueChain("Kni0"));
	knight->SetDialogue(m_pDigDataMng->GetDialogueChain("Kni1"));
	knight->SetDialogue(m_pDigDataMng->GetDialogueChain("Kni2"));

	CLivingObj *archer =
		m_pObjMng->CreateHero(eArcher,
			cr_x,
			cr_y, eForFriend, TRUE);
	archer->SetDialogue(m_pDigDataMng->GetDialogueChain("Arc0"));
	archer->SetDialogue(m_pDigDataMng->GetDialogueChain("Arc1"));
	archer->SetDialogue(m_pDigDataMng->GetDialogueChain("Arc2"));

	CLivingObj *assassin =
		m_pObjMng->CreateHero(eAssasin,
			cr_x,
			cr_y, eForFriend, TRUE);
	assassin->SetDialogue(m_pDigDataMng->GetDialogueChain("Ass0"));
	assassin->SetDialogue(m_pDigDataMng->GetDialogueChain("Ass1"));
	assassin->SetDialogue(m_pDigDataMng->GetDialogueChain("Ass2"));

	CLivingObj *magician =
		m_pObjMng->CreateHero(eMagician,
			cr_x,
			cr_y, eForFriend, TRUE);
	magician->SetDialogue(m_pDigDataMng->GetDialogueChain("Wiz0"));
	magician->SetDialogue(m_pDigDataMng->GetDialogueChain("Wiz1"));
	magician->SetDialogue(m_pDigDataMng->GetDialogueChain("Wiz2"));

	CLivingObj *warrior =
		m_pObjMng->CreateHero(eWarrior,
			cr_x,
			cr_y, eForFriend, TRUE);
	warrior->SetDialogue(m_pDigDataMng->GetDialogueChain("War0"));
	warrior->SetDialogue(m_pDigDataMng->GetDialogueChain("War1"));
	warrior->SetDialogue(m_pDigDataMng->GetDialogueChain("War2"));


	//Behavior Setting
	g_pAIMng->Add_CommonAI_Hero(knight);
	g_pAIMng->Add_CommonAI_Hero(archer);
	g_pAIMng->Add_CommonAI_Hero(magician);
	g_pAIMng->Add_CommonAI_Hero(warrior);
	g_pAIMng->Add_CommonAI_Hero(assassin);
}

bool CSceneMng::SaveSceneData(LPCTSTR str) 
{



	return TRUE;
}




//---------------------------------------------------------
//Update & Draw Scene
//---------------------------------------------------------
bool CSceneMng::LoadScene(enScene Scene)
{
	if (m_bSceneLoaded == TRUE) return FALSE;

	g_enCurScene = Scene;

	//Create Scene
	(this->*m_stlMap_Scene[Scene])();


	if (m_pMap != NULL)
	{
		g_pCurMap = m_pMap;
		//create camera
		if (m_pCamera == NULL)
		{
			m_pCamera = new CCamera(m_pMap);
		}
		//create scene surface
		if (m_pSceneSuf == NULL)
		{
			m_pSceneSuf = CreateSurfaceEX(m_pMap->GetMapWidth(), m_pMap->GetMapHeight(), TRUE);

			__DDCSetClip(m_pSceneSuf, 0, 0, m_pMap->GetMapWidth(), m_pMap->GetMapHeight());
		}
		//init Collide Quad Tree
		InitQuadTree(5, 5);
	}
	else
	{
		return FALSE;
	}
	//m_pObjMng->UpdateControlObj();

	m_bSceneLoaded = TRUE;
	return TRUE;
}

bool CSceneMng::DrawScene()
{
	//----------------------------------------------------
	//Pause Game
	//---------------------------------------------------- 
	if (DIKEYDOWNONCE(DIK_P) && (!g_pUIMng->IsTalking()))
	{
		g_bGamePause = !g_bGamePause;
		g_pTimeMng->StopTime();
	}

	//----------------------------------------------------
	//Check Scene Change
	//----------------------------------------------------
	ChangeScene();


	//Clear User Touched Link
	//g_pTouchLink = NULL;


	//If Scene Surface is not inited return false
	if (m_bSceneLoaded == FALSE)return FALSE;

	//Scece Loading succeed
	else
	{
		//----------------------------------------------------
		//update Camera
		//----------------------------------------------------
		m_pCamera->LookAtObject(g_pControledObj);

		m_pCamera->Update();

		RECT CameraRect = m_pCamera->GetCameraRect();




		//----------------------------------------------------
		//Update clipper
		//----------------------------------------------------
		__DDCSetClip(m_pSceneSuf, CameraRect.left,
			CameraRect.top,
			CameraRect.right,
			CameraRect.bottom);



		//----------------------------------------------------
		//fiil Scene Surface with black
		//----------------------------------------------------
		_FillRectEx(m_pSceneSuf, &CameraRect, 0);



		//------------------------------------------------------
		//User Talk to Npc
		//------------------------------------------------------
		if (!g_pUIMng->IsTalking())
		{
			if (DIKEYDOWNONCE(DIK_SPACE))
			{
				CDialogueChain* Dialog = g_pControledObj->TryTalk();
				if (Dialog != NULL)
				{
					g_pUIMng->TriggerTalk(Dialog);
				}
			}
		}

	/*	g_pControledObj->GetNearByObj(g_listNearByObj);

		std::list<CLivingObj*>::iterator iter = g_listNearByObj.begin();
		
		while (iter!= g_listNearByObj.end())
		{
			(*iter)->FaceToObject(g_pControledObj);
			(*iter)->m_stObjUI.ActiveEmotion(enEmoticon::eDelight);
			++iter;
		}
*/


		//----------------------------------------------------
		//Check User Input
		//----------------------------------------------------
		if (g_pControledObj == NULL) 
		{
			g_pGameMng->CallGameOver();
			return FALSE;
		}

		//Update User Controled Hero
		m_pObjMng->UserControlUpdate();

		//m_pSceneSuf->BltBatch
		


		//----------------------------------------------------
		//Draw Tile Map
		//----------------------------------------------------	
		m_pMap->DrawMap(m_pSceneSuf, CameraRect);

		

		if ((!g_bGamePause)&&(!m_bSceChanging))
		{
			//----------------------------------------------------
			//collision detection
			//----------------------------------------------------
		
			//Update Quad Tree 
			g_pQuadTree->UpdateTree();

			//Aviud Collision
			g_pQuadTree->CheckCollision();

			//Used in Test
			//g_pQuadTree->DisPlayTreeBound(m_pSceneSuf);

			
			//----------------------------------------------------
			//Event Update
			//----------------------------------------------------
			g_pEventMng->update();


			//----------------------------------------------------
			//update user input
			//----------------------------------------------------
			if (g_pControledObj != NULL)
				g_pControledObj->Control_USER();

			//----------------------------------------------------
			//update ai
			//----------------------------------------------------
			m_pObjMng->Update_AI();

		}
		//----------------------------------------------------
		//Draw All Object
		//----------------------------------------------------
		m_pObjMng->DrawAllObject(m_pSceneSuf, m_pMap);

		//m_pObjMng->DrawAllCollideRect(m_pSceneSuf);

		//----------------------------------------------------
		//Test ........
		//----------------------------------------------------
		//g_pUIMng->DrawDialog(m_pSceneSuf, 'e', 400, 400, 420);
		//g_pUIMng->DrawDialog(m_pSceneSuf, 'o', 420, 400, 420);
		//RECT rect = { 400, 350, 1500,450 };

	//	g_pUIMng->DrawDialog(m_pSceneSuf, "", rect);

		//g_pTestdObj01->MoveToObject_Smart(g_pControledObj, false, 1);
		//g_pControledObj->MoveToPoint_Smart({800,800},false);
		//std::list<POINT> linelist;
		//Astar_PathFinding pathfind;

	//	pathfind(m_pMap, g_pControledObj, { 400,400 }, linelist);
		
	//	int a = 1;
	//	Astar_PathFinding::operator()();

		//CTileMap*map, CObject *obj, POINTFLOAT Dest, std::list<POINT>&GetList
		//#ifdef DEBUG
		//m_pObjMng->DrawAllCollideRect(m_pSceneSuf);

		//m_pObjMng->DrawAllAttackRect(m_pSceneSuf);

		//Display Link information

		//DisplayLinkInfo();

		//DisplayAllInfo();
		//#endif // DEBUG


		//----------------------------------------------------
		//Layout Adjust 
		//----------------------------------------------------
	/*	if (g_pTouchLink != NULL)
		{
			if (g_pTouchLink->m_pObj != NULL)
				g_pTouchLink->m_pObj->DrawObject(m_pSceneSuf, 1);

		}*/



		//----------------------------------------------------
		//DisPlay Damage
		//----------------------------------------------------
		__DisplayDamage(m_pSceneSuf, 1, 50);



		//----------------------------------------------------
		//Put Scene Surface to Back Surface
		//----------------------------------------------------
		RECT DrawRect;

		if (m_pMap->GetMapWidth() >= HRES && m_pMap->GetMapHeight() >= VRES)
		{
			DrawRect = { 0,0,HRES,VRES };
		}
		else
		{
			POINT ScreenCenter = { HRES >> 1,VRES >> 1 };
			DrawRect = CreateRect(ScreenCenter, m_pCamera->GetWidth(), m_pCamera->GetHeight());
		}
		g_pDDSBack->Blt(&DrawRect, m_pSceneSuf, &m_pCamera->GetCameraRect(), DDBLT_WAIT, NULL);

		if(!m_bSceChanging) g_pUIMng->DrawStateBar();
		g_pUIMng->__DrawDialogue(g_pDDSBack,true);
		
		

		m_pEffectMng->SceneEffectUpdate();

		//Test
		//	__GrayScale(g_pDDSBack);
	}
	return TRUE;
}

bool CSceneMng::ChangeScene()
{
	//
	if (g_pTouchLink_new)
	{
		if (!g_pTouchLink_new->IsObjTouchLink(g_pControledObj))
		{
			g_pTouchLink_new->ActiveLink();
			g_pTouchLink_new = NULL;
		}
	}

	//checek if user wanna move to other Scene 
	if (g_pTouchLink != NULL && m_bSceChanging == FALSE)
	{
		if (g_pTouchLink->IsObjTouchLink(g_pControledObj) && g_pTouchLink->IsActive())
		{
			m_bSceChanging = TRUE;	//Mark scene Is Changing

			g_bLockInput = TRUE;

			g_pTouchLink->DisActiveLink();
		}
	}

	//create new scene
	if (m_pEffectMng->IsFadeInEnd() && m_bSceChanging == TRUE)
	{
		CreateOppLinkAuto(g_pTouchLink);//create opposite link

		ClearScene();	//Cleat Previous Scene

		LoadScene(g_pTouchLink->GetDestScene());	//Load New Scene

		m_pObjMng->TransPortHero(g_pTouchLink);	//Transport Heroes to New Scene

		m_pEffectMng->PlayFadeOut();	//play effect fade out effect

		m_bSceChanging = FALSE;			//Mark Scene Has Changed

		//clear Backuup Link Data
		list<CSceneLink*>::iterator iter = find(g_list_maplink.begin(), g_list_maplink.end(), g_pTouchLink);
		if (iter != g_list_maplink.end())	g_list_maplink.erase(iter);
		SAFE_RELEASE(g_pTouchLink);
		g_pQuadTree->ReBuildTree();
	}
	if (m_bSceChanging == FALSE &&m_pEffectMng->IsFadeOutEnd())
	{
		g_bLockInput = FALSE;
	}

	//play effect fade in effect
	if (m_bSceChanging) m_pEffectMng->PlayFadeIn();

	

	return TRUE;
}


//-----------------------------------------------------------
//Clear Scene
//-----------------------------------------------------------
bool CSceneMng::ClearAll()
{
	m_pObjMng->ClearAll();
	return TRUE;
}

bool CSceneMng::ClearScene()
{
	//clear object 
	m_pObjMng->ClearAllMonster();
	m_pObjMng->ClearAllBuilding();
	m_pObjMng->ClearAllNpc();
	m_pObjMng->ClearAllOrnament();

    //clear Tile map
	SAFE_RELEASE(m_pMap);

	//clear camera
	SAFE_RELEASE(m_pCamera);

	//clear direct draw surface
	if (m_pSceneSuf != NULL)
	{
		m_pSceneSuf->Release();
		m_pSceneSuf = NULL;
	}
	//clear event
	g_pEventMng->ClearEvent();

	//clear inited data 
	g_pDataBase->ClearTileImg();
	g_pDataBase->ClearTile();
	g_pDataBase->ClearBuildingImg();

	//clear Scene Link
	ClearCurSceneLink();

	g_LIst_DmgData.clear();
	//clear QuadTree
	SAFE_RELEASE(g_pQuadTree);

	m_bSceneLoaded = FALSE;

	return TRUE;
}





//-----------------------------------------------------------
//Scene Sound
//-----------------------------------------------------------
bool CSceneMng::InitSound(CHHSound *_pSound)
{
	m_pSound = _pSound;

	return TRUE;
}



//-----------------------------------------------------------
//Quad Tree
//-----------------------------------------------------------
bool CSceneMng::InitQuadTree(int MaxObj, int depth)
{
	g_pQuadTree = new CQuadTree();

	g_pQuadTree->InitTree(m_pMap->GetMapRect(), MaxObj, depth);

	return TRUE;
}



//-----------------------------------------------------------
//Tile Map Setting
//-----------------------------------------------------------
CTileMap * CSceneMng::GetTileMap() const
{
	return m_pMap;
}
int CSceneMng::__Rand_x()
{
	return rand() % GetTileMap()->GetMapWidth();
}
int CSceneMng::__Rand_y()
{
	return rand() % GetTileMap()->GetMapHeight();
}



DWORD CSceneMng::__Rand_l(DWORD minWidth, DWORD maxWidth)
{
	return (rand() % (maxWidth + minWidth))+ minWidth;
}



void CSceneMng::CreateTileMap(DWORD Row, DWORD Column, DWORD CellLength)
{
	g_pCurMap = m_pMap = new CTileMap(Row, Column, CellLength);
}

void CSceneMng::CreateTileMap(LPCSTR ParsingFile) 
{
	FSTTileMapData MapData;
	MapData.Parsing(ParsingFile);
	m_pMap=	MapData.CreateTileMap();
	g_pCurMap = m_pMap;
}

void CSceneMng::SetTile(GRID grid, enTile eTile, WORD TileType, WORD level) 
{
	m_pMap->SetTile(grid,g_pDataBase->GetTile(eTile), TileType ,level);
}
void CSceneMng::SetTileAll(enTile eTile, WORD TileType, WORD level) 
{
	m_pMap->SetTileAll(g_pDataBase->GetTile(eTile), TileType, level);
}
void CSceneMng::SetTileH(DWORD row, enTile eTile, WORD TileType, WORD level)
{
	m_pMap->SetTileH(row, g_pDataBase->GetTile(eTile), TileType, level);
}
void CSceneMng::SetTileV(DWORD column, enTile eTile, WORD TileType, WORD level)
{
	m_pMap->SetTileV(column,g_pDataBase->GetTile(eTile), TileType, level);
}
void CSceneMng::SetTileMapEdge(enTile eTile, WORD TileType, WORD level) 
{
	m_pMap->SetTileEdge(g_pDataBase->GetTile(eTile), TileType, level);
}




//-----------------------------------------------------------
//Create Function
//-----------------------------------------------------------
CBuilding* CSceneMng::CreateBuilding(RECT rect, enBuilding eBuilding,enScene DestScene,RECT DestRect)
{	
	return	m_pObjMng->CreateBuilding(g_pDataBase->GetBuildingImg(eBuilding), rect, 1, STBDLinkSetting(0, DestScene, DestRect));
}

CBuilding* CSceneMng::CreateBuilding(GRID BD_L, GRID BD_R, enBuilding eBuilding, enScene DestScene, GRID Des_L, GRID Des_R)
{
	return	CreateBuilding(m_pMap->GetGridRect(BD_L, BD_R), eBuilding, DestScene, m_pMap->GetGridRect(Des_L, Des_R));
}

CMonster* CSceneMng::CreateMonster(POINT pos, enMonster enMon, enObjectForce force, bool UserControl)
{
	return m_pObjMng->CreateMonster(enMon,pos.x,pos.y, force, UserControl);
}

CNPC* CSceneMng::CreateNpc(GRID GRID, enNPC NpcID, int width, int Height)
{
	POINT pt = m_pMap->GetGridCoord(GRID);
	return m_pObjMng->CreateNpc(NpcID, pt.x, pt.y, width, Height);
}


Cornament* CSceneMng::CreateOrnament(enOrnamentSheet orna, LPCSTR str, POINT pos, DWORD Width, DWORD Height, bool ActiveCollide)
{
	Cornament *obj = NULL;
	obj = m_pObjMng->CreateOrnament(orna,pos.x,pos.y,Width,Height,ActiveCollide,str);

	return obj;
}
Cornament* CSceneMng::CreateOrnament(enOrnamentSheet orna, LPCSTR Name, GRID LT, GRID RB, bool ActiveCollide)
{
	Cornament *obj = NULL;

	obj = m_pObjMng->CreateOrnament(orna, m_pMap->GetGridRect(LT, RB), ActiveCollide, Name);

	return obj;
}
Cornament* CSceneMng::CreateOrnament(enOrnamentSheet orna, LPCSTR Name,GRID GD, bool ActiveCollide)
{
	Cornament *obj = NULL;

	obj = m_pObjMng->CreateOrnament(orna, m_pMap->GetGridRect(GD), ActiveCollide, Name);

	return obj;
}

//-----------------------------------------------------------
//Create Scene Link
//-----------------------------------------------------------
CSceneLink* CSceneMng::CreateLink(RECT Entrance, enScene DesScene, RECT DestRT, enDirection dir)
{
	CSceneLink *link = NULL;
	link = new CRectSLink(Entrance, DesScene, DestRT, dir);

	return link;
}

CSceneLink* CSceneMng::CreateLink(GRID Ent_L, GRID Ent_R, enScene DesScene, GRID Des_L, GRID Des_R, enDirection dir)
{
	CSceneLink *link = NULL;
	

	link = new CRectSLink(m_pMap->GetGridRect(Ent_L, Ent_R), DesScene, m_pMap->GetGridRect(Des_L, Des_R), dir);

	return link;
}

void CSceneMng::CreateOppLinkAuto(CSceneLink *link)
{
	list<CSceneLink*>::iterator iter;

	for (iter = g_list_maplink.begin(); iter != g_list_maplink.end(); iter++)
	{
		if ((*iter)->GetDestScene() == link->GetDestScene())
		{
			if ((*iter) == g_pTouchLink)
			{
				g_pTouchLink_new = (*iter)->CreateOPPLinkAuto(FALSE);
			}
			else if ((*iter)->IsTheSameAsLink(g_pTouchLink)) 
			{
				continue;
			}
			else (*iter)->CreateOPPLinkAuto(TRUE);
		}
	}
}

bool CSceneMng::ClearCurSceneLink()
{
	list<CSceneLink*>::iterator iter;

	for (iter = g_list_maplink.begin(); iter != g_list_maplink.end(); )
	{
		if ((*iter)->GetCurScene() == g_enCurScene && (*iter)!=g_pTouchLink)
		{
			SAFE_RELEASE((*iter));
			iter = g_list_maplink.erase(iter);
		}
		else iter++;
	}
	return TRUE;
}



//-----------------------------------------------------------
//Used in Test
//-----------------------------------------------------------
void CSceneMng::DisplayLinkInfo()
{
	list<CSceneLink*>::iterator SceneIter;

	for (SceneIter = g_list_maplink.begin(); SceneIter != g_list_maplink.end(); SceneIter++)
	{
		(*SceneIter)->DisplayLinkInfo(m_pSceneSuf);
	}
}

void CSceneMng::DisplayAllInfo()
{
	//m_pSceneSuf->GetDC(&gHdc);

	//g_pQuadTree->DisPlayTreeInfo(g_pQuadTree->m_pRoot, m_pSceneSuf);

	//m_pMap->DisplayMapData();

	//m_pSceneSuf->ReleaseDC(gHdc);
}

