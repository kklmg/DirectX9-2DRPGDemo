#pragma once
//-----------------------------------------------------------
//Include Head File
//-----------------------------------------------------------
#include<ddraw.h>
#include<list>
#include<map>

using namespace std;

//-----------------------------------------------------------
//Include My Head File
//-----------------------------------------------------------
#include"DataBase.h"
#include"QuadTree.h"
#include"ObjectMng.h"
#include"AIMng.h"
#include"EffectMng.h"
#include"TileMap.h"
#include"TileSheet.h"
#include"SceneLink.h"
#include"CameraClass.h"
#include"TextSurface.h"
#include"HHSound.h"
#include"DialogueData.h"




//Global Scene Link List
extern list<CSceneLink*>g_list_maplink;


//-----------------------------------------------------------
//Class Scene Manager
//-----------------------------------------------------------
class CSceneMng
{
public:
	CSceneMng();
	~CSceneMng();
public:
	//Init Scene Manager
	bool Init();

	void PreCreate();

	//-----------------------------------------------------------
	//Scene Attribute
	//-----------------------------------------------------------
private:
	bool m_bPause;	

	bool m_bSceneLoaded;

	bool m_bSceChanging;

	CSceneLink* m_pDestScene;
	
	LPDIRECTDRAWSURFACE7 m_pSceneSuf;
	

	//-----------------------------------------------------------
	//Scene Sound
	//-----------------------------------------------------------
private:
	bool InitSound(CHHSound *_pSound);

	CHHSound *m_pSound;



	//-----------------------------------------------------------
	//Scene Main Function
	//-----------------------------------------------------------
private:
	//Change Scene
	bool ChangeScene();

public:
	//Draw and Update
	bool DrawScene();



	//-----------------------------------------------------------
	//About User Input

	//-----------------------------------------------------------
private:
	//User Input Update
	void UserInputUpdate();



	//-----------------------------------------------------------
	//About Quad Tree
	//-----------------------------------------------------------
	//quad tree used in collide detection
	bool InitQuadTree(int MaxObj, int depth);



	//-----------------------------------------------------------
	//Clear Scene
	//-----------------------------------------------------------
	bool ClearAll();

	bool ClearCurSceneLink();

	bool ClearScene();



	//-----------------------------------------------------------
	//About Tile map
	//-----------------------------------------------------------
public:
	CTileMap * GetTileMap() const;

private:
	//-----------------------------------------------------------
	//Tile map Setting
	//-----------------------------------------------------------
	int __Rand_x();
	int __Rand_y();


	DWORD __Rand_l(DWORD minWidth, DWORD maxWidth);

	void CreateTileMap(DWORD Row, DWORD Column, DWORD CellLength);
	void CreateTileMap(LPCSTR ParsingFile);

	//
	void SetTile(GRID grid, enTile eTile, WORD TileType, WORD level);

	void SetTileAll(enTile eTile, WORD TileType, WORD level);

	void SetTileH(DWORD row, enTile eTile, WORD TileType, WORD level);

	void SetTileV(DWORD column, enTile eTile, WORD TileType, WORD level);

	void SetTileMapEdge(enTile eTile, WORD TileType, WORD level);

private:
	CTileMap *m_pMap;

	//-----------------------------------------------------------
	//Scene Camera
	//-----------------------------------------------------------
private:
	//Camera
	CCamera *m_pCamera;



	//-----------------------------------------------------------
	//Scene Effect
	//-----------------------------------------------------------
private:
	CEffectMng *m_pEffectMng;



	//-----------------------------------------------------------
	//Scene Object
	//-----------------------------------------------------------
private:
	//Create Object
	CBuilding* CreateBuilding(RECT rect, enBuilding eBuilding, enScene DestScene, RECT DestRect);
	CBuilding* CreateBuilding(GRID BD_L, GRID BD_R, enBuilding eBuilding, enScene DestScene, GRID Des_L, GRID Des_R);

	CMonster* CreateMonster(POINT pos, enMonster MonID, enObjectForce force, bool IScontrollable);
	CNPC* CreateNpc(GRID GRID, enNPC NpcID, int width = 40, int Height = 50);

	Cornament* CreateOrnament(enOrnamentSheet orna,LPCSTR Name, POINT pos,DWORD Width,DWORD Height,bool ActiveCollide);
	Cornament* CreateOrnament(enOrnamentSheet orna, LPCSTR Name, GRID BD_L, GRID BD_R, bool ActiveCollide);
	Cornament* CreateOrnament(enOrnamentSheet orna, LPCSTR Name, GRID GD, bool ActiveCollide);

private:
	//manager
	CObjectMng *m_pObjMng;


	//-----------------------------------------------------------
	//Scene Link
	//-----------------------------------------------------------
private:
	//Create Scene Link
	CSceneLink* CreateLink(RECT Entrance, enScene DesScene, RECT DestRT,enDirection dir);

	CSceneLink* CreateLink(GRID Ent_L, GRID Ent_R, enScene DesScene, GRID Des_L, GRID Des_R, enDirection dir);

	void CreateOppLinkAuto(CSceneLink *link);
	//-----------------------------------------------------------
	//Dialog
	//-----------------------------------------------------------
	private:
	CDialogueDataMng *m_pDigDataMng;

	//-----------------------------------------------------------
	//About Scene
	//-----------------------------------------------------------
private:
	void InitSceneContainer();

	//Load Scene 
	bool LoadScene(enScene Scene);
	bool LoadSceneMenu(enScene Scene);


	//Create Scene 
	bool CS_Town();
	bool CS_Forest_1();
	bool CS_Forest_2();
	bool CS_Forest_3();
	bool CS_InHome();
	bool CS_InSmith();
	bool CS_Town_North();
	bool CS_InGuild();
	bool CS_InRoom01();
	bool CS_InRoom02();
	bool CS_InRoom03();
	bool CS_InRoom04();
	bool CS_InRoom05();
	bool CS_InRoom06();
	bool CS_Mountain();
	bool CS_Coast();



private:
	//declare function pointer
	typedef bool (CSceneMng::*Create)();

	//map container Scene Creator
	map<enScene, Create>  m_stlMap_Scene;



	//-----------------------------------------------------------
	//Used in Test 
	//-----------------------------------------------------------
private:
	//test
	bool SaveSceneData(LPCTSTR str);



	bool TestInit();

	//Display Scene state
	void DisplayAllInfo();

	void DisplayLinkInfo();

	CTextSuf *testtext;
};
