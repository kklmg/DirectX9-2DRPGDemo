//-----------------------------------------------
//Head File
//-----------------------------------------------
#include <d3dx9math.h>
#pragma comment(lib, "d3dx9.lib") 

//-----------------------------------------------
//My Head File
//-----------------------------------------------
#include"LivingObjectClass.h"
#include"RectMath.h"
#include"collide.h"
#include"Global_data.h"
#include"Astar.h"

//-----------------------------------------------
//Class Living Object
//-----------------------------------------------
CLivingObj::CLivingObj() :
	m_pAreaBeAttack(NULL), m_bLockAction(FALSE), m_stPathUpdate(this),
	m_enAct(enAction::eStand), m_pFocusEnemy(NULL), m_eAttackDir(GetCurDir()),
	m_bIsAttacking(FALSE), m_bIsAttackAniEnd(TRUE), m_bIsTriggerDamage(FALSE),
	m_AI(new CSelectorNode()), m_listDetectedEnemy(0), m_bIsFightState(TRUE)
{
	m_stObjUI.Init();

	g_List_pLObj.push_back(this);

	//m_listDetectedEnemy.clear();
}

CLivingObj::~CLivingObj()
{
	m_pFocusEnemy = NULL;
	if (m_AI != NULL)
	{
		m_AI->Destroy();
		m_AI = NULL;
	}

	std::list<CLivingObj*>::iterator iter;
	iter = std::find(g_List_pLObj.begin(), g_List_pLObj.end(), this);

	(*iter) = NULL;
	g_List_pLObj.erase(iter);


	if (m_bControlAble)
	{
		std::vector<CLivingObj*>::iterator VectorIter;
		VectorIter = std::find(g_vector_pControlObj.begin(), g_vector_pControlObj.end(), this);

		(*VectorIter) = NULL;
		g_vector_pControlObj.erase(VectorIter);
	}
	g_pEventMng->ClearAttackEvent(this);


	DestroyAttackArea();
}

//-----------------------------------------------
//Living Object Update All
//-----------------------------------------------
void CLivingObj::UpdateLivingObj()
{
	UpdateAtkRange();

	CAttribute::Update();

	if(m_pAreaBeAttack!=nullptr)
	m_pAreaBeAttack->UpdateArea();

	UpdateUI();
}


//-----------------------------------------------
// Object Tag
//-----------------------------------------------
void CLivingObj::SetForce(enObjectForce force)
{
	m_enForce = force;
}

enObjectForce CLivingObj::GetForce()
{
	return m_enForce;
}

//-----------------------------------------------
//object Animation || Action
//-----------------------------------------------
bool CLivingObj::SetAction(enAction act)
{
	if (m_bLockAction)	return FALSE;
	else
	{
		m_enAct = act;
		return TRUE;
	}
}

bool CLivingObj::StopAction()
{
	if (m_bLockAction)	return FALSE;
	else
	{
		m_enAct = enAction::eStand;
		return TRUE;
	}
}

void CLivingObj::LockAction()
{
	LockDir();
	m_bLockAction = TRUE;
}

void CLivingObj::UnLockAction()
{
	UnLockDir();
	m_bLockAction = FALSE;
}

bool CLivingObj::IsActionLocked() const
{
	return m_bLockAction;
}

enAction CLivingObj::GetCurAction()const
{
	return m_enAct;
}

//-----------------------------------------------
//Object Move Related:: User Control
//-----------------------------------------------
void CLivingObj::MoveLeft(bool IsRun)
{
	SetObjectDir(eLeft);

	if (IsRun == FALSE)
	{
		m_enAct = enAction::eWalk;
		CObject::m_V2Pos.x -= CAttribute::GetWalkSpeed()*__GetDeltaTime;
	}
	else
	{
		m_enAct = enAction::eRun;
		CObject::m_V2Pos.x -= CAttribute::GetRunSpeed()*__GetDeltaTime;
	}
}

void CLivingObj::MoveRight(bool IsRun)
{
	SetObjectDir(eRight);

	if (IsRun == FALSE)
	{
		m_enAct = enAction::eWalk;
		CObject::m_V2Pos.x += CAttribute::GetWalkSpeed()*__GetDeltaTime;
	}
	else
	{
		m_enAct = enAction::eRun;
		CObject::m_V2Pos.x += CAttribute::GetRunSpeed()*__GetDeltaTime;
	}
}

void CLivingObj::MoveUp(bool IsRun)
{
	SetObjectDir(eUp);

	if (IsRun == FALSE)
	{
		m_enAct = enAction::eWalk;
		CObject::m_V2Pos.y -= CAttribute::GetWalkSpeed()*__GetDeltaTime;
	}
	else
	{
		m_enAct = enAction::eRun;
		CObject::m_V2Pos.y -= CAttribute::GetRunSpeed()*__GetDeltaTime;
	}
}

void CLivingObj::MoveDown(bool IsRun)
{
	SetObjectDir(eDown);

	if (IsRun == FALSE)
	{
		m_enAct = enAction::eWalk;
		CObject::m_V2Pos.y += CAttribute::GetWalkSpeed()*__GetDeltaTime;
	}
	else
	{
		m_enAct = enAction::eRun;
		CObject::m_V2Pos.y += CAttribute::GetRunSpeed()*__GetDeltaTime;
	}
}

void CLivingObj::MoveLeftUp(bool IsRun)
{
	SetObjectDir(eUp);

	if (IsRun == FALSE)
	{
		m_enAct = enAction::eWalk;
		CObject::m_V2Pos.x -= CAttribute::GetWalkSpeed_Tilt()*__GetDeltaTime;
		CObject::m_V2Pos.y -= CAttribute::GetWalkSpeed_Tilt()*__GetDeltaTime;
	}
	else
	{
		m_enAct = enAction::eRun;
		CObject::m_V2Pos.x -= CAttribute::GetRunSpeed_Tilt()*__GetDeltaTime;
		CObject::m_V2Pos.y -= CAttribute::GetRunSpeed_Tilt()*__GetDeltaTime;
	}

}

void CLivingObj::MoveRightUp(bool IsRun)
{
	SetObjectDir(eUp);

	if (IsRun == FALSE)
	{
		m_enAct = enAction::eWalk;
		CObject::m_V2Pos.x += CAttribute::GetWalkSpeed_Tilt()*__GetDeltaTime;
		CObject::m_V2Pos.y -= CAttribute::GetWalkSpeed_Tilt()*__GetDeltaTime;
	}
	else
	{
		m_enAct = enAction::eRun;
		CObject::m_V2Pos.x += CAttribute::GetRunSpeed_Tilt()*__GetDeltaTime;
		CObject::m_V2Pos.y -= CAttribute::GetRunSpeed_Tilt()*__GetDeltaTime;
	}

}

void CLivingObj::MoveLeftDown(bool IsRun)
{
	SetObjectDir(eDown);

	if (IsRun == FALSE)
	{
		m_enAct = enAction::eWalk;
		CObject::m_V2Pos.x -= CAttribute::GetWalkSpeed_Tilt()*__GetDeltaTime;
		CObject::m_V2Pos.y += CAttribute::GetWalkSpeed_Tilt()*__GetDeltaTime;
	}
	else
	{
		m_enAct = enAction::eRun;
		CObject::m_V2Pos.x -= CAttribute::GetRunSpeed_Tilt()*__GetDeltaTime;
		CObject::m_V2Pos.y += CAttribute::GetRunSpeed_Tilt()*__GetDeltaTime;
	}

}

void CLivingObj::MoveRightDown(bool IsRun)
{
	SetObjectDir(eDown);

	if (IsRun == FALSE)
	{
		m_enAct = enAction::eWalk;
		CObject::m_V2Pos.x += CAttribute::GetWalkSpeed_Tilt()*__GetDeltaTime;
		CObject::m_V2Pos.y += CAttribute::GetWalkSpeed_Tilt()*__GetDeltaTime;
	}
	else
	{
		m_enAct = enAction::eRun;
		CObject::m_V2Pos.x += CAttribute::GetRunSpeed_Tilt()*__GetDeltaTime;
		CObject::m_V2Pos.y += CAttribute::GetRunSpeed_Tilt()*__GetDeltaTime;
	}

}

void CLivingObj::AddtoControlVector()
{
	CObject::m_bControlAble = TRUE;

	g_vector_pControlObj.push_back(this);
}


//-----------------------------------------------
//Object Action Related:: AI Control
//-----------------------------------------------
void CLivingObj::FaceToPoint(POINTFLOAT pointf)
{
	float x = CObject::GetX() - pointf.x;
	float y = CObject::GetY() - pointf.y;

	if (x > 0 && y > 0)//face to left up
	{
		if (abs(x) >= abs(y)) SetObjectDir(eLeft);
		else SetObjectDir(eUp);
	}
	else if (x > 0 && y < 0)// face to left down
	{
		if (abs(x) >= abs(y)) SetObjectDir(eLeft);
		else SetObjectDir(eDown);
	}
	else if (x < 0 && y>0)//face to right up
	{
		if (abs(x) >= abs(y)) SetObjectDir(eRight);
		else SetObjectDir(eUp);
	}
	else if (x < 0 && y < 0)
	{
		if (abs(x) >= abs(y))SetObjectDir(eRight);
		else SetObjectDir(eDown);
	}
	else if (x > 0)
	{
		SetObjectDir(eLeft);
	}
	else if (x < 0)
	{
		SetObjectDir(eRight);
	}
	else if (y > 0)
	{
		SetObjectDir(eUp);
	}
	else if (y < 0)
	{
		SetObjectDir(eDown);
	}

}
void CLivingObj::FaceToObject(CObject* obj)
{
	if (obj == NULL)return;
	FaceToPoint(obj->GetPosition());
}

bool CLivingObj::MoveToPoint(D3DXVECTOR2 v2, bool IsRun, float offset)
{
	//adjust direction
	FaceToPoint({ v2.x,v2.y });

	//calculate disp
	D3DXVECTOR2 tempV2 = v2 - GetVector();

	//upadate animation
	if (abs(tempV2.x) <= offset && abs(tempV2.y) <= offset)
	{
		StopAction();

		return TRUE;
	}
	else
	{
		D3DXVec2Normalize(&tempV2, &tempV2);

		if (IsRun)
		{
			m_enAct = enAction::eRun;
			CObject::SetPosition(CObject::GetVector() += tempV2 * GetRunSpeed()*__GetDeltaTime);
		}
		else
		{
			m_enAct = enAction::eWalk;
			CObject::SetPosition(CObject::GetVector() += tempV2 * GetWalkSpeed()*__GetDeltaTime);
		}

		return FALSE;
	}
}
bool CLivingObj::MoveToPoint_ByLine()
{
	bool bRun = FALSE;

	if (m_stPathUpdate.m_listPath.size() > 2) bRun = TRUE;


	std::list<POINT>::iterator iter = m_stPathUpdate.m_listPath.begin();

	if (iter != m_stPathUpdate.m_listPath.end())
	{
		if (MoveToPoint({ (float)(*iter).x, (float)(*iter).y }, bRun, 1))
		{
			m_stPathUpdate.m_listPath.erase(iter);
		}
	}
	if (!m_stPathUpdate.m_listPath.size())
	{
		return TRUE;
	}
	else return FALSE;
}

void CLivingObj::ClearCurPath()
{
	m_list_PathLine.clear();
}

bool CLivingObj::MoveToPoint_Smart(POINTFLOAT Dest)
{
	m_stPathUpdate(g_pCurMap, Dest);

	return MoveToPoint_ByLine();

	//if (m_list_PathLine.size() == 0)
	//{
	//	Astar_PathFinding Pathfind(g_pCurMap);
	//	if (g_pCurMap) Pathfind(g_pCurMap, this, Dest, m_list_PathLine);
	//}

	//MoveToPoint_ByLine(IsRun);
	//return FALSE;
}

bool CLivingObj::MoveToObject(CObject *obj, bool IsRun)
{
	//already reach object
	if (Collide::AreTwoObjCollide(obj, this))
	{
		StopAction();
		return TRUE;
	}

	//Get target position
	D3DXVECTOR2 v2_Target = obj->GetVector();

	//calculate distance
	D3DXVECTOR2 V2_Offset = v2_Target - GetVector();


	//Face toTarget
	FaceToPoint({ v2_Target.x,v2_Target.y });


	if (V2_Offset.x || V2_Offset.y)
	{
		D3DXVECTOR2 V2_Nomalize;

		D3DXVec2Normalize(&V2_Nomalize, &V2_Offset);

		D3DXVECTOR2 V2_Move;
		//run 
		if (IsRun)
		{
			//set run animation
			m_enAct = enAction::eRun;

			//move value
			V2_Move = V2_Nomalize * GetRunSpeed()*__GetDeltaTime;
		}
		//walk
		else
		{
			//set walk animation
			m_enAct = enAction::eWalk;

			//move value
			V2_Move = V2_Nomalize * GetWalkSpeed()*__GetDeltaTime;
		}


		if (D3DXVec2Length(&V2_Move) > D3DXVec2Length(&V2_Offset))
		{
			CObject::SetPosition(CObject::GetVector());

			StopAction();
			return TRUE;
		}
		else
		{
			CObject::SetPosition(CObject::GetVector() + V2_Move);
			return FALSE;
		}

	}
	StopAction();
	return FALSE;
}

bool CLivingObj::MoveToObject_Smart(CObject *obj, bool IsRun, float offset)
{
	//Get Object position
	//POINTFLOAT PtStart = obj->GetPosition();

	//Get Object position in Tile map
	//CTile*tile = g_pCurMap->GetPointTilePos(PtStart);

	//m_list_PathLine.clear();

	m_stPathUpdate(g_pCurMap, obj);

	//m_stPathUpdate.m_listPath.size()!=0
	//if (m_list_PathLine.size() == 0)
	//{

	//	Astar_PathFinding Pathfind(g_pCurMap);
	//	if (g_pCurMap) 	Pathfind(g_pCurMap, this, obj->GetPosition(), m_list_PathLine);
	//}

	return MoveToPoint_ByLine();

}

void CLivingObj::ChangeDirRandomly()
{
	SetObjectDir(enDirection(rand() % 4));
}

void CLivingObj::ChaseObj(CLivingObj* obj)
{
	D3DXVECTOR2 tempVet2 = obj->GetVector() - this->GetVector();
	D3DXVec2Normalize(&tempVet2, &tempVet2);

	this->SetPosition(this->GetVector() += tempVet2*this->GetWalkSpeed()*__GetDeltaTime);
}

bool CLivingObj::CheckObjectInAtkRng(CLivingObj *obj)
{
	if (Collide::RECTtoRECT(obj->m_rectDraw, this->m_rectAtkRange[GetCurDir()])) return TRUE;
	else return FALSE;
}

bool CLivingObj::CheckObjectInDetectRng(CLivingObj *obj)
{
	float Distance = CalculateDistance(obj->m_V2Pos, this->m_V2Pos);

	if (Distance < obj->CAttribute::GetDetectDistance())return TRUE;
	else return FALSE;

}

bool CLivingObj::GetNearByObj(std::list<CLivingObj*>&list)
{
	//Clear
	list.clear();

	//Init
	//-------------------------------------------------------------
	STCircle Cir = GetCircleInvRect(m_rectDraw);
	Cir.radius += 50;
	//get nearby objects
	g_pQuadTree->GetObjCollideWithCircle(Cir, list);

	if (list.size() == 0) return FALSE;
	else return TRUE;
}

CLivingObj* CLivingObj::GetNearestObj(std::list<CLivingObj*>&_list)
{
	if (_list.size() == 0) return NULL;

	//Init
	//-------------------------------------------------------------
	std::list<CLivingObj*>::iterator Iter = _list.begin();
	if (_list.size() == 1)	return (*Iter);

	CLivingObj* Target = (*Iter);
	POINTFLOAT myPosition = GetPosition();
	float sDistance = CalculateDistance(myPosition, (*Iter)->GetPosition());

	//Search
	for (; Iter != _list.end(); ++Iter)
	{
		float Dis = CalculateDistance(myPosition, (*Iter)->GetPosition());

		if (Dis <= sDistance)
		{
			sDistance = Dis;
			Target = (*Iter);
		}
	}
	return Target;
}

CDialogueChain* CLivingObj::TryTalk()
{
	STCircle Cir = GetCircleInvRect(m_rectDraw);

	std::list<CLivingObj*> tempList;
	std::list<CLivingObj*>::iterator iter;
	//get nearby objects
	g_pQuadTree->GetObjCollideWithCircle(Cir, tempList);

	if (tempList.size() == 0) return NULL;
	else
	{
		for (iter = tempList.begin(); iter != tempList.end();)
		{
			if ((*iter)->GetDialogue() == NULL)
			{
				tempList.erase(iter++);
			}
			else ++iter;
		}

		if (tempList.size() == 0) return NULL;
		else
		{
			CLivingObj* Target = GetNearestObj(tempList);
			Target->FaceToPoint(this->GetPosition());
			return Target->GetDialogueRand();
		}
	}
}


//-----------------------------------------------
//Detect Enemy
//-----------------------------------------------
bool CLivingObj::DetectEnemy()
{
	//reset detected enemy 
	m_listDetectedEnemy.clear();
	m_listDetectedEnemy.resize(0);

	m_pFocusEnemy = NULL;
	//m_pFocusEnemy = NULL;

	g_pQuadTree->CheckObjInDetectRange(this, m_listDetectedEnemy);

	if (m_listDetectedEnemy.size() == 0)return FALSE;

	list<CLivingObj*>::iterator Iter;

	for (Iter = m_listDetectedEnemy.begin(); Iter != m_listDetectedEnemy.end();)
	{
		enObjectForce Force = (*Iter)->GetForce();

		//erase ally focus
		if (m_enForce == enObjectForce::eForFriend) 
		{
			if(Force== enObjectForce::eForFriend 
				|| Force== enObjectForce::eForFNeutral)
				Iter = m_listDetectedEnemy.erase(Iter);
			else Iter++;
		}
		if (m_enForce == enObjectForce::eForHostile)
		{
			if (Force == enObjectForce::eForHostile)
				Iter = m_listDetectedEnemy.erase(Iter);
			else Iter++;
		}
	}

	if (m_listDetectedEnemy.size() == 0)return FALSE;

	FocusNearestEnemy();
	return TRUE;
}

bool CLivingObj::FocusNearestEnemy()
{
	if (m_listDetectedEnemy.size() == 0)return FALSE;

	m_pFocusEnemy = GetNearestObj(m_listDetectedEnemy);

	//if (m_listDetectedEnemy.size() == 0) return FALSE;

	//list<CLivingObj*>::iterator Iter;

	//for (Iter = m_listDetectedEnemy.begin(); Iter != m_listDetectedEnemy.end(); Iter++)
	//{

	//	if (m_listDetectedEnemy.size() == 1)
	//	{
	//		m_pFocusEnemy = (*Iter);
	//		return TRUE;
	//	}

	//	//my 
	//	static float a = CalculateDistance(GetPosition(), (*Iter)->GetPosition());

	//	float b = CalculateDistance(GetPosition(), (*Iter)->GetPosition());

	//	if (b <= a)
	//	{
	//		a = b;
	//		m_pFocusEnemy = (*Iter);
	//	}
	//}
	return TRUE;
}

list<CLivingObj*> CLivingObj::GetDetectedEnemyList()
{
	return m_listDetectedEnemy;
}

void CLivingObj::ClearDetected()
{
	m_listDetectedEnemy.clear();

	m_pFocusEnemy = NULL;
}

CLivingObj* CLivingObj::GetFocusEnemy()
{
	return m_pFocusEnemy;
}


//-----------------------------------------------
//object AI 
//-----------------------------------------------
bool CLivingObj::Control_COM()
{
	if (m_AI == NULL)  return FALSE;
	if (m_bIsAlive == FALSE)  return FALSE;
	if (IsActionLocked())return FALSE;

	AI_path.clear();

	m_AI->run();

	return TRUE;
}

CCompositeNode* CLivingObj::GetAI()
{
	return m_AI;
}

bool CLivingObj::AddBevNode(CBevNode *node)
{
	if (node)
	{
		m_AI->AddChild(node);
		return TRUE;
	}
	else return FALSE;
}




//-----------------------------------------------
//object Battle
//-----------------------------------------------
void CLivingObj::GetDamage(int damage)
{
	CObject::m_stObjState.bHasBeenAttack = TRUE;

	CAttribute::SetHP(CAttribute::GetCurHp() - damage);
	if (CAttribute::GetCurHp() < 0)
	{
		m_bIsAlive = FALSE;
	}
}

RECT CLivingObj::GetAttackRect()
{
	return m_rectAtkRange[m_eAttackDir];
}
RECT CLivingObj::GetAttackRect_E()
{
	return m_rectAtkRange[GetCurDir()];
}

void CLivingObj::UpdateAtkRange()
{
	m_rectAtkRange[eLeft].left = m_rectDraw.left - (long)CAttribute::GetAttackDistance();
	m_rectAtkRange[eLeft].top = m_rectDraw.top;
	m_rectAtkRange[eLeft].right = m_rectDraw.left;
	m_rectAtkRange[eLeft].bottom = m_rectDraw.bottom;


	m_rectAtkRange[eRight].left = m_rectDraw.right;
	m_rectAtkRange[eRight].top = m_rectDraw.top;
	m_rectAtkRange[eRight].right = m_rectDraw.right + (long)CAttribute::GetAttackDistance();
	m_rectAtkRange[eRight].bottom = m_rectDraw.bottom;


	m_rectAtkRange[eUp].left = m_rectDraw.left;
	m_rectAtkRange[eUp].top = m_rectDraw.top - (long)CAttribute::GetAttackDistance();
	m_rectAtkRange[eUp].right = m_rectDraw.right;
	m_rectAtkRange[eUp].bottom = m_rectDraw.top;


	m_rectAtkRange[eDown].left = m_rectDraw.left;
	m_rectAtkRange[eDown].top = m_rectDraw.bottom;
	m_rectAtkRange[eDown].right = m_rectDraw.right;
	m_rectAtkRange[eDown].bottom = m_rectDraw.bottom + (long)CAttribute::GetAttackDistance();
}

enAttackStage CLivingObj::GetAtkStage()const
{
	return m_enAtkStage;
}

void CLivingObj::PerformAttack()
{
	if (m_enAtkStage == enAttackStage::eAtkEnd)
	{
		SetAction(eAttack_1);
		LockAction();
		m_enAtkStage = enAttackStage::eAtkInit;
		//g_pEventMng->GetNewEvent(&this->Attack);//attack
		g_pEventMng->ObjectTriggerAttack(this);
	}
}

//--------------------------------------------------
//object Attack Effective Area
//--------------------------------------------------
void CLivingObj::CreateAttackEffectArea()
{
	m_pAreaBeAttack = new CCollideArea(CPosition::GetVectorRef());

	g_List_AttackableObj.push_back(this);
}

void CLivingObj::DestroyAttackArea()
{
	list<CLivingObj*>::iterator iter;

	if (m_pAreaBeAttack != NULL)
	{
		for (iter = g_List_AttackableObj.begin(); iter != g_List_AttackableObj.end(); iter++)
		{
			if ((*iter) == this)
			{
				(*iter) = NULL;
				g_List_AttackableObj.erase(iter);
				break;
			}
		}
		SAFE_RELEASE(m_pAreaBeAttack);
	}
}

CCollideArea* CLivingObj::GetBeAttackArea()const
{
	return m_pAreaBeAttack;
}

//-----------------------------------------------
//Object UI
//-----------------------------------------------
void CLivingObj::UpdateUI()
{
	m_stObjUI.m_rtHPBar = CreateRect({ (long)CObject::m_V2Pos.x,(long)(m_rectDraw.top - CObject::m_dwHeight*0.1) },
		(long)CObject::m_dwWidth*1.3,
		(long)CObject::m_dwHeight*0.1);

	m_stObjUI.m_rtEmoticon = CreateRect({ (long)CObject::m_V2Pos.x, (long)(m_rectDraw.top + CObject::m_dwHeight*0.1) },
		(long)CObject::m_dwWidth*0.8,
		(long)CObject::m_dwHeight*0.8);


	//m_stObjUI.m_rtBuff = CreateRect({ CObject::m_V2Pos.x, m_rectDraw.top + CObject::m_dwHeight*0.1 },
	//	CObject::m_dwWidth*0.8,
	//	CObject::m_dwHeight*0.8);

	//m_stObjUI.m_rtAura = CreateRect({ CObject::m_V2Pos.x, m_rectDraw.top + CObject::m_dwHeight*0.1 },
	//	CObject::m_dwWidth*0.8,
	//	CObject::m_dwHeight*0.8);



	if (CObject::m_stObjState.bHasBeenAttack) m_stObjUI.ActiveDisPlayHP();
}

void CLivingObj::DrawUI(LPDIRECTDRAWSURFACE7 suf)
{
	m_stObjUI.DrawHpBar(CAttribute::m_stHP, suf, 4);

	m_stObjUI.DrawEmotion(suf, 4);

	//DisplayDetectRange(suf);
}

void CLivingObj::DisplayDetectRange(LPDIRECTDRAWSURFACE7 suf)
{
	int Diameter = GetDetectDistance() * 2;
	RECT rect = CreateRect({ (long)GetX(), (long)GetY() }, Diameter, Diameter);

	suf->Blt(&rect, g_pDDSCircle, NULL, DDBLT_KEYSRC | DDBLT_WAIT, NULL);
}

