#pragma once
#include"ObjectMng.h"
#include"Global_data.h"
#include"Knight.h"
#include"Archer.h"
#include"Magician.h"
#include"Warrior.h"
#include"Assassin.h"
#include"blit.h"
#include"damage.h"

//All Game Object List
std::list<CObject*>g_list_pObj;

//All Life Object List 
std::list<CLivingObj*>g_List_pLObj;

//All Hero List
std::list<CHero*>g_list_pHero;

//user controlable object
std::vector<CLivingObj*>g_vector_pControlObj;
 
//All Still Object List
std::list <CStillObject*>g_list_pStillObject;

//All Building List
std::list<CBuilding*>g_list_pBuilding;

//collide
std::list<CObject*>g_List_CollideObj;

std::list<CLivingObj*>g_List_AttackableObj;


bool _Compare_Y(CObject *obj1, CObject *Obj2)
{
	if (obj1->m_bDrawFirst||Obj2->m_bDrawFirst) 
	{
		if (obj1->m_bDrawFirst&&obj1->m_bDrawFirst) 
		{
			return obj1->GetY() < Obj2->GetY();	
		}
		else if (obj1->m_bDrawFirst) 
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return obj1->GetY() < Obj2->GetY();
	}
	return FALSE;
	//return obj1->m_bDrawFirst;
	//return obj1->GetY() < Obj2->GetY();
}


//Constructor /Destroyer
//-----------------------------------------
CObjectMng::CObjectMng()
{
	m_dwHeroSelect = 0;
}

CObjectMng::~CObjectMng()
{
	SAFE_RELEASE(g_pAttMng);

	ClearAll();
}

//Init
//-----------------------------------------
bool CObjectMng::Init() 
{
	g_pAttMng = new CATTMng;
	g_pAttMng->Init();

	if (g_pAttMng)return TRUE;
	else return FALSE;
}

//Object behavior update
//-------------------------------------------
bool CObjectMng::UserControlUpdate()
{
	if (g_vector_pControlObj.size() != 0)
	{
		if (DIKEYDOWNONCE(DIK_TAB)) 
		{		
			g_pControledObj = ChangeSelect();
		}
		return TRUE;
	}
	else return FALSE;
}

bool CObjectMng::Update_AI()
{
	std::list<CLivingObj*>::iterator ObjIter;

	for (ObjIter = g_List_pLObj.begin(); ObjIter != g_List_pLObj.end(); ObjIter++)
	{
		//Skip user controled object
		if ((*ObjIter) == g_pControledObj) continue;

		(*ObjIter)->Control_COM();
	}

	return TRUE;
}

CLivingObj *CObjectMng::ChangeSelect()
{
	if (g_vector_pControlObj.size() == 0)
		return NULL;

	m_dwHeroSelect++;

	if (m_dwHeroSelect >= g_vector_pControlObj.size())
	{
		m_dwHeroSelect = 0;
	}
	return g_vector_pControlObj[m_dwHeroSelect];
}

bool CObjectMng::TransPortHero(CSceneLink* link)
{
	std::vector<CLivingObj*>::iterator iter;

	for (iter = g_vector_pControlObj.begin(); iter != g_vector_pControlObj.end(); iter++)
	{
		POINT Dest = link->GetDestPos((*iter));
		(*iter)->SetPosition(Dest.x, Dest.y);
	}
	return TRUE;
}

//Create Object
//------------------------------------------------------------
CBuilding * CObjectMng::CreateBuilding(CBuildingImg* Img,int x, int y, DWORD width, DWORD height, DWORD linkCount, ...)
{
	CBuilding *building = NULL;

	building = new CBuilding(x,y,width,height,Img);

	va_list ap;

	va_start(ap, linkCount);

	for (int i = 0; i < linkCount; i++)
	{
		STBDLinkSetting LinkSetting = va_arg(ap, STBDLinkSetting);

		building->SetDoorLink(LinkSetting);

	}
	va_end(ap);

	//g_list_pObj.push_back(building);
	return building;
}
CBuilding* CObjectMng::CreateBuilding(CBuildingImg* Img, RECT rect, DWORD linkCount, ...)
{
	CBuilding *building = NULL;

	building = new CBuilding(rect, Img);

	va_list ap;

	va_start(ap, linkCount);

	for (int i = 0; i < linkCount; i++)
	{
		STBDLinkSetting LinkSetting = va_arg(ap, STBDLinkSetting);

		building->SetDoorLink(LinkSetting);
		
	}
	va_end(ap);

//	g_list_pObj.push_back(building);
	return building;
}

Cornament* CObjectMng::CreateOrnament(enOrnamentSheet orna, int x, int y, DWORD width, DWORD height,bool collide,LPCSTR string)
{
	CSheetFile* bmpFile= g_pDataBase->GetSheetFile(orna);
	if (bmpFile == NULL)	return NULL;

	if (!bmpFile->CheckIfExists(string))	return NULL;

	Cornament *obj = new Cornament(x, y, width, height, bmpFile, string, collide);
	if (obj == NULL)return NULL;

	//g_list_pObj.push_back(obj);
	return obj;
}
Cornament* CObjectMng::CreateOrnament(enOrnamentSheet orna, RECT rect, bool collide, LPCSTR string)
{
	CSheetFile* bmpFile = g_pDataBase->GetSheetFile(orna);
	if (bmpFile == NULL)	return NULL;

	if (!bmpFile->CheckIfExists(string))	return NULL;

	Cornament *obj = new Cornament(rect, bmpFile, string, collide);
	if (obj == NULL)return NULL;

	//g_list_pObj.push_back(obj);
	return obj;
}

CHero* CObjectMng::CreateHero(enHero heroID,int x,int y, enObjectForce force,bool IScontrollable)
{
	CHero *hero = NULL;
	switch (heroID)
	{
	case eKnight: 
	{
		hero = new CKnight(x, y, 40, 54, g_pDataBase->GetKnightImg());
	
	}break;
	case eArcher:
	{
		hero = new CArcher(x, y, 40, 60, g_pDataBase->GetArcherImg());

	}break;
	case eMagician: 
	{
		hero = new CMagician(x, y, 40, 60, g_pDataBase->GetSpriteSheet(eSSMagician));

	}break;
	case eWarrior: 
	{
		hero = new CWarrior(x, y, 40, 60, g_pDataBase->GetSpriteSheet(eSSWarrior));
	} break;
	case eAssasin:
	{
		hero = new CAssassin(x, y, 40, 60, g_pDataBase->GetSpriteSheet(eSSAssassin));
	}break;

	default:
		break;
	}
	if (hero)
	{
		//set force
		hero->SetForce(force);

		//active control
		if (IScontrollable)	hero->AddtoControlVector();

		//g_list_pObj.push_back(hero);
	}
	 return hero;
}

CNPC* CObjectMng::CreateNpc(enNPC NpcID, int x, int y, int width,int Height)
{
	CNPC *NPC = NULL;
	NPC = new CNPC(x, y, width, Height, g_pDataBase->GetSpriteSheet(NpcID));
	
	if (NPC)
	{
		//set force
		NPC->SetForce(enObjectForce::eForFNeutral);		

		//g_list_pObj.push_back(NPC);
	}
	return NPC;
}
	
CMonster * CObjectMng::CreateMonster(enMonster MonID, int x, int y, enObjectForce force, bool IScontrollable)
{
	CMonster *mon = NULL;
	switch (MonID)
	{
	case enMonster::eMRabbit: mon = new CMonster(x, y, 40 ,40,g_pDataBase->GetSpriteRow(eSRRabbit)); break;
	//case
	//..................
	default:
		break;
	}

	if (mon)
	{
		//Set Force
		mon->SetForce(force);

		if (IScontrollable)mon->AddtoControlVector();

		//g_list_pObj.push_back(mon);
	}
	return mon;
}


//Update and Draw Object
//------------------------------------------------------------
bool CObjectMng::DrawAllObject(LPDIRECTDRAWSURFACE7 suf, CTileMap *CurMap)
{
	std::list<CObject*>::iterator ObjIter;

	//Adjust layout 
	g_list_pObj.sort(_Compare_Y);

	//travalsal Object List
	for (ObjIter = g_list_pObj.begin(); ObjIter != g_list_pObj.end();) 
	{
		//remove dead object
		//--------------------------------------------
		if ((*ObjIter)->IsAlive() == FALSE) 
		{
			//user dead
			if (g_pControledObj==(*ObjIter))
			{
				delete (*ObjIter);
				(*ObjIter) = NULL;

				//change hero
				g_pControledObj=ChangeSelect();
			}
			//remove normal
			else
			{
				delete (*ObjIter);
				(*ObjIter) = NULL;
			}
			//erase 
			ObjIter = g_list_pObj.erase(ObjIter);
			continue;
		};

		//update Object
		//------------------------------------------------------------------
		(*ObjIter)->UpdateAll();


		//Avoid Collision
		//----------------------------------------------------------------------
		enObjectType objType = (*ObjIter)->GetObjType();
		if (objType == enObjectType::eOTHero)
		{
			//avoid collide with map edge
			Collide::AvoidCollideWithMapEdge((*ObjIter), CurMap);			
		}
		if (objType == eOTMonster || objType == eOTNpc || objType == eOTHero)
		{
			Collide::AvoidcollideTile(CurMap, (*ObjIter));
		}
	
		//Draw Object
		//----------------------------------------------------------------
		(*ObjIter)->DrawObject(suf, TRUE);		//dispplay object

		//Reset Trigger State
		(*ObjIter)->ResetState();

		ObjIter++;
	}
	return TRUE;
}

//test 
bool CObjectMng::DrawAllCollideRect(LPDIRECTDRAWSURFACE7 suf)
{
	std::list<CObject*>::iterator ObjIter;

	for (ObjIter = g_list_pObj.begin(); ObjIter != g_list_pObj.end(); ObjIter++)
	{
		(*ObjIter)->UpdateAll();
		CCollideArea*  Area=(*ObjIter)->GetCollideArea();	
		if (Area == NULL)continue;
		Area->DrawCollideArea(suf);//display collide rect
	}
	return TRUE;
}
//test
bool CObjectMng::DrawAllAttackRect(LPDIRECTDRAWSURFACE7 suf) 
{
	std::list<CLivingObj*>::iterator ObjIter;

	for (ObjIter = g_List_pLObj.begin(); ObjIter != g_List_pLObj.end(); ObjIter++)
	{
		_FillRectEx(suf, &(*ObjIter)->GetAttackRect(), RGB(255, 255, 0));
	}
	return TRUE;
}



//OBject Clear
//-------------------------------------------------
bool CObjectMng::ClearAll()
{
	/*g_List_pLObj.clear();

	g_list_pHero.clear();

	g_list_pStillObject.clear();

	g_list_pBuilding.clear();*/

	std::list<CObject*>::iterator Iter;

	for (Iter = g_list_pObj.begin(); Iter != g_list_pObj.end(); Iter++) 
	{
		if ((*Iter) != NULL) 
		{
			delete (*Iter);
			(*Iter) = NULL;		
		}	
	}
	g_list_pObj.clear();

	__ClearDamageData();

	return TRUE;
}

bool CObjectMng::ClearAllBuilding() 
{
	//erase building point at object list
	std::list<CObject*>::iterator Iter_Obj;

	for (Iter_Obj = g_list_pObj.begin(); Iter_Obj != g_list_pObj.end();)
	{
		if ((*Iter_Obj)->GetObjType() ==enObjectType::eOTBuilding)
		{
			Iter_Obj=g_list_pObj.erase(Iter_Obj);
		}
		else Iter_Obj++;
	}
	//erase building pointer at still object list
	std::list <CStillObject*>::iterator Iter_StillObj;

	for (Iter_StillObj = g_list_pStillObject.begin(); Iter_StillObj != g_list_pStillObject.end();)
	{
		if ((*Iter_StillObj)->GetObjType() == enObjectType::eOTBuilding)
		{
			Iter_StillObj = g_list_pStillObject.erase(Iter_StillObj);
		}
		else Iter_StillObj++;
	}

	//delete and clear all building point
	std::list<CBuilding*>::iterator Iter_Building;

	for (Iter_Building = g_list_pBuilding.begin(); Iter_Building != g_list_pBuilding.end(); Iter_Building++)
	{
		if ((*Iter_Building)!=NULL)
		{
			delete (*Iter_Building);
			(*Iter_Building) = NULL;
		}
	}
	g_list_pBuilding.clear();
	return TRUE;
}

bool CObjectMng::ClearAllMonster() 
{
	//erase building point at life object list
	std::list <CObject*>::iterator Iter_Obj;

	for (Iter_Obj = g_list_pObj.begin(); Iter_Obj != g_list_pObj.end();)
	{
		if ((*Iter_Obj)->GetObjType() == enObjectType::eOTMonster)
		{
			//SAFE_RELEASE((*Iter_LifeObj));
			delete (*Iter_Obj);
			(*Iter_Obj) = NULL;
			Iter_Obj = g_list_pObj.erase(Iter_Obj);
		}
		else Iter_Obj++;
	}
	return TRUE;
}

bool CObjectMng::ClearAllNpc() 
{
	//erase building point at life object list
	std::list <CObject*>::iterator Iter_Obj;

	for (Iter_Obj = g_list_pObj.begin(); Iter_Obj != g_list_pObj.end();)
	{
		if ((*Iter_Obj)->GetObjType() == enObjectType::eOTNpc)
		{
			//SAFE_RELEASE((*Iter_LifeObj));
			delete (*Iter_Obj);
			(*Iter_Obj) = NULL;
			Iter_Obj = g_list_pObj.erase(Iter_Obj);
		}
		else Iter_Obj++;
	}
	return TRUE;
}

bool CObjectMng::ClearAllOrnament()
{
	//erase building point at life object list
	std::list <CObject*>::iterator Iter_Obj;

	for (Iter_Obj = g_list_pObj.begin(); Iter_Obj != g_list_pObj.end();)
	{
		if ((*Iter_Obj)->GetObjType() == enObjectType::eOTOrnament)
		{
			//SAFE_RELEASE((*Iter_LifeObj));
			delete (*Iter_Obj);
			(*Iter_Obj) = NULL;
			Iter_Obj = g_list_pObj.erase(Iter_Obj);
		}
		else Iter_Obj++;
	}
	return TRUE;
}





