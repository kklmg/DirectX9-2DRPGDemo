#pragma once
//Inclue Head File
#include<list>
#include<vector>
#include<stdarg.h>

//My Head File
#include"ObjectClass.h"
#include"LivingObjectClass.h"
#include"MonsterClass.h"
#include"Knight.h"
#include"Archer.h"
#include"BuildingClass.h"
#include"EnumGroup.h"
#include"TileMap.h"
#include"StillObject.h"
#include"Ornament.h"
#include"NPCClass.h"


//All Game Object List
extern std::list<CObject*>g_list_pObj;

//All Life Object List 
extern std::list<CLivingObj*>g_List_pLObj;

//All Hero List
extern std::list<CHero*>g_list_pHero;

//all controlable obj list
extern std::vector<CLivingObj*>g_vector_pControlObj;

//All Still Object List
extern std::list <CStillObject*>g_list_pStillObject;

//All Building List
extern std::list<CBuilding*>g_list_pBuilding;

extern std::list<CObject*>g_List_CollideObj;

extern std::list<CLivingObj*>g_List_AttackableObj;





bool _Compare_Y(CObject *obj1, CObject *Obj2);

class CObjectMng
{
public:	
	CObjectMng();

	~CObjectMng();

	bool Init();

	bool UserControlUpdate();

	bool Update_AI();
	
	CHero * CreateHero(enHero heroID,int x,int y,enObjectForce force,bool IScontrollable);

	CNPC * CreateNpc(enNPC NpcID, int x, int y, int width = 40, int Height = 50);

	CMonster * CreateMonster(enMonster MonID , int x, int y, enObjectForce force, bool IScontrollable);

	CBuilding * CreateBuilding(CBuildingImg* Img, int x, int y, DWORD width, DWORD height, DWORD linkCount, ...);
	CBuilding * CreateBuilding(CBuildingImg* Img, RECT rect,DWORD linkCount,...);

	Cornament* CreateOrnament(enOrnamentSheet orna, int x, int y, DWORD width, DWORD height, bool collide, LPCSTR string);
	Cornament* CreateOrnament(enOrnamentSheet orna, RECT rect, bool collide, LPCSTR string);

	bool DrawAllObject(LPDIRECTDRAWSURFACE7 suf, CTileMap *CurMap);
	
	bool DrawAllCollideRect(LPDIRECTDRAWSURFACE7 suf);

	bool DrawAllAttackRect(LPDIRECTDRAWSURFACE7 suf);

	bool TransPortHero(CSceneLink* link);

	//clear function
	bool ClearAll();

	bool ClearAllBuilding();

	bool ClearAllMonster();

	bool ClearAllNpc();

	bool ClearAllOrnament();

	CLivingObj *ChangeSelect();

private:
	
	std::list<CHero*>::iterator m_IterHero;

	DWORD m_dwHeroSelect;



	bool CreateNpc();

	bool CreateObject();

	

	//CHero *hero;
}; 

