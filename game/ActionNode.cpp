//---------------------------------------------------
//Head File
//---------------------------------------------------
#include<list>


//---------------------------------------------------
//My Head File
//---------------------------------------------------
#include"ActionNode.h"
#include"collide.h"
#include"Global_data.h"



//---------------------------------------------------
//Class Leaf Node
//---------------------------------------------------
CLeafNode::CLeafNode(CLivingObj *Myself) :m_pMyself(Myself) {}

CLeafNode::~CLeafNode() 
{
	m_pMyself = NULL;
}

void CLeafNode::ResetNodeState()
{
	m_bHasInited = FALSE;
	m_bHasRun = FALSE;
}

void CLeafNode::ClearInited()
{
	m_bHasInited = FALSE;
}

void CLeafNode::Destroy()
{
	delete this;
}


//---------------------------------------------------
//Action Node:: Waiting
//---------------------------------------------------
CActWait::CActWait(CLivingObj *Myself, float lastTime)
	:CLeafNode(Myself), m_fLastTime(lastTime) {}

CActWait::~CActWait() 
{
}

enNodeState CActWait::run() 
{
	m_pMyself->AI_path.push_back(this);

	if (!m_bHasInited) 
	{
		m_Timer.ResetTimer();
		m_bHasInited = TRUE;
	}

	//if timer complete
	if (m_Timer.OnceTimer(m_fLastTime)) 
	{	
		m_bHasRun = TRUE;
		return enNodeState::eNodeTrue;
	}
	return enNodeState::eRunnuing;
}


//---------------------------------------------------
//Action Node::  Change Direction randomly
//---------------------------------------------------
CActTurn::CActTurn(CLivingObj *myself) :CLeafNode(myself) {}

CActTurn::~CActTurn() 
{
}

enNodeState CActTurn::run()
{
	m_pMyself->ChangeDirRandomly();

	m_bHasRun = TRUE;

	return enNodeState::eNodeTrue;
};




//---------------------------------------------------
//Action Node:: Move to Designated Postion
//---------------------------------------------------
CActGoToPos::CActGoToPos(CLivingObj *myself, POINTFLOAT position, float offset)
	:CLeafNode(myself), m_Pointf(position), m_fOffset(offset){}

CActGoToPos::~CActGoToPos() 
{
}

enNodeState CActGoToPos::run()
{
	if (m_pMyself->MoveToPoint({ m_Pointf.x ,m_Pointf.y }, FALSE, 2))
	{
		m_bHasRun = TRUE;

		return enNodeState::eNodeTrue;
	}
	return enNodeState::eRunnuing;
}


//---------------------------------------------------
//Action Node:: Move to Target
//---------------------------------------------------
CActFollow::CActFollow(CLivingObj *myself, CLivingObj *target)
	:CLeafNode(myself), m_pTarget(target) {}

CActFollow::~CActFollow() 
{
	m_pTarget = NULL;
}

enNodeState CActFollow::run()
{
	if (m_pMyself->MoveToObject(m_pTarget, FALSE))
	{
		m_pMyself->UpdatePatrolPoint();
		
		m_bHasRun = TRUE;
		return enNodeState::eNodeTrue;
	}
	else return enNodeState::eRunnuing;
}


//---------------------------------------------------
//Action Node:: Move to User
//---------------------------------------------------
CActFollowUser::CActFollowUser(CLivingObj *myself)
	:CLeafNode(myself) {}

CActFollowUser::~CActFollowUser()
{
}

enNodeState CActFollowUser::run()
{
	if (m_pMyself->MoveToObject_Smart(g_pControledObj, FALSE,10))
	{
		m_pMyself->UpdatePatrolPoint();
		m_bHasRun = TRUE;
		return enNodeState::eNodeTrue;
	}
	else
	{	
		return enNodeState::eRunnuing;
	}
}


//---------------------------------------------------
//Action Node:: Move to Detected Enemy
//---------------------------------------------------
CActFollowFocusEnemy::CActFollowFocusEnemy(CLivingObj *myself)
	:CLeafNode(myself) {}

CActFollowFocusEnemy::~CActFollowFocusEnemy()
{
	m_pMyself = NULL;
}

enNodeState CActFollowFocusEnemy::run()
{
	if (!m_bHasInited) 
	{
		m_pMyself->ClearCurPath();	
		m_bHasInited = TRUE;
	}

	//get user controlling object
	CObject* target = m_pMyself->GetFocusEnemy();

	if (target == NULL) return enNodeState::eNodeFalse;


	//if (m_pMyself->MoveToObject_Smart(target,FALSE,10))
	//{
	//	m_bHasRun = TRUE;
	//	return enNodeState::eNodeTrue;
	//}

	if (m_pMyself->MoveToObject(target, FALSE)) 
	{
		m_bHasRun = TRUE;
		return enNodeState::eNodeTrue;
	}
	else return enNodeState::eRunnuing;
}


//---------------------------------------------------
//Action Node:: Walk Around
//---------------------------------------------------
CActWalkAround::CActWalkAround(CLivingObj *myself, int radius)
	:CLeafNode(myself), m_nRadius(radius) {}

CActWalkAround::~CActWalkAround() 
{
	m_pMyself = NULL;
}

enNodeState CActWalkAround::run()
{
	if (!m_bHasInited) 
	{
		POINTFLOAT patrolpoint = m_pMyself->GetPatrolPoint();

		//get random position
		m_ptPos.x = rand() % (m_nRadius * 2) - m_nRadius + patrolpoint.x;
		m_ptPos.y = rand() % (m_nRadius * 2) - m_nRadius + patrolpoint.y;
	
		
		m_bHasInited = TRUE;
	}

	if (m_pMyself->MoveToPoint({m_ptPos.x,m_ptPos.y }, FALSE, 10)) 
	{
		m_bHasRun = TRUE;
		return enNodeState::eNodeTrue;
	}
	return enNodeState::eNodeFalse;
}



//---------------------------------------------------
//Action Node:: Walk Around Limit Time
//---------------------------------------------------
CActWalkAround_time::CActWalkAround_time(CLivingObj *myself, int radius,float TimeLimit)
	:CLeafNode(myself), m_nRadius(radius) , m_fTime(TimeLimit){}

CActWalkAround_time::~CActWalkAround_time()
{
	m_pMyself = NULL;
}

enNodeState CActWalkAround_time::run()
{
	if (!m_bHasInited)
	{
		POINTFLOAT patrolpoint = m_pMyself->GetPatrolPoint();

		//get random position
		m_ptPos.x = rand() % (m_nRadius * 2) - m_nRadius + patrolpoint.x;
		m_ptPos.y = rand() % (m_nRadius * 2) - m_nRadius + patrolpoint.y;

		m_Timer.ResetTimer();

		m_bHasInited = TRUE;
	}

	if (m_Timer.OnceTimer(m_fTime)) 
	{
		m_pMyself->SetAction(enAction::eStand);
		m_bHasRun = TRUE;
		return enNodeState::eNodeTrue;
	}
	
	if (m_pMyself->MoveToPoint({ m_ptPos.x,m_ptPos.y }, FALSE, 10))
	{
		m_pMyself->SetAction(enAction::eStand);
		m_bHasRun = TRUE;
		return enNodeState::eNodeTrue;
	}
	return enNodeState::eNodeFalse;
}

//---------------------------------------------------
//Action Node:: Perform Attack 
//---------------------------------------------------
CActAttack::CActAttack(CLivingObj *myself)
	:CLeafNode(myself) {}

CActAttack::~CActAttack()
{
	m_pMyself = NULL;
}

enNodeState CActAttack::run()
{
	enAttackStage stage = m_pMyself->GetAtkStage();

	switch (stage)
	{
	case eAtkEnd:
	{		
		m_pMyself->PerformAttack();
		m_bHasInited = TRUE;
		return enNodeState::eRunnuing;
	}
		break;
	case eAtkRunning:
		return enNodeState::eRunnuing;
		break;
	case eAtkWaitCoolTime: 
	{
		m_bHasRun = TRUE;
		return enNodeState::eNodeTrue;
	}
		break;
	default:
		break;
	}
}


//----------------------------------------------------------
//Condition Node:: check Distance between Position and me
//----------------------------------------------------------
//Nearby
CConNear_pt::CConNear_pt(CLivingObj *myself, POINTFLOAT position, float Distance)
	:CLeafNode(myself), m_Pointf(position), m_fDistance(Distance){}
CConNear_pt::~CConNear_pt()
{
}

enNodeState CConNear_pt::run()
{
	float x = abs(m_pMyself->GetPosition().x - m_Pointf.x);
	float y = abs(m_pMyself->GetPosition().y - m_Pointf.y);

	if (x <= m_fDistance && y <= m_fDistance) 
	{
		m_bHasRun = TRUE;
		return enNodeState::eNodeTrue;
	}
	
	else return enNodeState::eNodeFalse;
}

//Far Away
CConFarAway_pt::CConFarAway_pt(CLivingObj *myself, POINTFLOAT position, float Distance)
	:CLeafNode(myself), m_Pointf(position), m_fDistance(Distance) {}
CConFarAway_pt::~CConFarAway_pt()
{
}

enNodeState CConFarAway_pt::run()
{
	float x = abs(m_pMyself->GetPosition().x - m_Pointf.x);
	float y = abs(m_pMyself->GetPosition().y - m_Pointf.y);

	if (x >= m_fDistance || y >= m_fDistance)
	{
		m_bHasRun = TRUE;
		return enNodeState::eNodeTrue;
	}

	else return enNodeState::eNodeFalse;
}


//----------------------------------------------------------
//Condition Node:: check Distance between Object and me
//----------------------------------------------------------
//Nearby
CConNear_obj::CConNear_obj(CLivingObj *myself, CLivingObj *target, float Distance)
	:CLeafNode(myself), m_pTarget(target), m_fDistance(Distance) {}

CConNear_obj::~CConNear_obj()
{
}

enNodeState CConNear_obj::run()
{
	float x = abs(m_pMyself->GetPosition().x - m_pTarget->GetPosition().x);
	float y = abs(m_pMyself->GetPosition().y - m_pTarget->GetPosition().y);

	if (x <= m_fDistance && y <= m_fDistance)
	{
		m_bHasRun = TRUE;
		return enNodeState::eNodeTrue;
	}

	else return enNodeState::eNodeFalse;
}

//Far Away
CConFarAway_obj::CConFarAway_obj(CLivingObj *myself, CLivingObj *target, float Distance)
	:CLeafNode(myself), m_pTarget(target), m_fDistance(Distance) {}

CConFarAway_obj::~CConFarAway_obj()
{
}

enNodeState CConFarAway_obj::run()
{
	float x = abs(m_pMyself->GetPosition().x - m_pTarget->GetPosition().x);
	float y = abs(m_pMyself->GetPosition().y - m_pTarget->GetPosition().y);

	if (x >= m_fDistance || y >= m_fDistance)
	{
		m_bHasRun = TRUE;
		return enNodeState::eNodeTrue;
	}

	else return enNodeState::eNodeFalse;
}
//----------------------------------------------------------
//Condition Node:: check Distance between User and me
//----------------------------------------------------------
//Nearby
CConNear_User::CConNear_User(CLivingObj *myself, float Distance)
	:CLeafNode(myself), m_fDistance(Distance) {}

CConNear_User::~CConNear_User()
{
}

enNodeState CConNear_User::run()
{
	float x = abs(m_pMyself->GetPosition().x - g_pControledObj->GetPosition().x);
	float y = abs(m_pMyself->GetPosition().y - g_pControledObj->GetPosition().y);

	if (x <= m_fDistance && y <= m_fDistance)
	{
		m_bHasRun = TRUE;
		return enNodeState::eNodeTrue;
	}

	else return enNodeState::eNodeFalse;
}

//Far Away
CConFarAway_User::CConFarAway_User(CLivingObj *myself, float Distance)
	:CLeafNode(myself), m_fDistance(Distance) {}

CConFarAway_User::~CConFarAway_User()
{
}

enNodeState CConFarAway_User::run()
{
	float x = abs(m_pMyself->GetPosition().x - g_pControledObj->GetPosition().x);
	float y = abs(m_pMyself->GetPosition().y - g_pControledObj->GetPosition().y);

	if (x >= m_fDistance || y >= m_fDistance)
	{
		m_bHasRun = TRUE;
		return enNodeState::eNodeTrue;
	}

	else return enNodeState::eNodeFalse;
}



//----------------------------------------------------------
//Condition Node:: check if there are enemies nearby
//----------------------------------------------------------
CConIsEnemyNearBy::CConIsEnemyNearBy(CLivingObj* myself)
	:CLeafNode(myself) {}

CConIsEnemyNearBy::~CConIsEnemyNearBy() 
{
}

enNodeState CConIsEnemyNearBy::run()
{
	if (m_pMyself->DetectEnemy())
	{
		m_pMyself->m_stObjUI.ActiveEmotion(enEmoticon::eAmaze);

		m_bHasRun = TRUE;
		return enNodeState::eNodeTrue;
	}
	else return enNodeState::eNodeFalse;
}

//----------------------------------------------------------
//Condition Node:: check if the enemy is in my attack range
//----------------------------------------------------------
CConIsEnemyInMyRange::CConIsEnemyInMyRange(CLivingObj* myself)
	:CLeafNode(myself) {}

CConIsEnemyInMyRange::~CConIsEnemyInMyRange() 
{
	m_pMyself = NULL;
}

enNodeState CConIsEnemyInMyRange::run()
{
	//if (!m_pMyself->GetFocusEnemy())return enNodeState::eNodeFalse;
	//return Collide::IsRectIntersect(m_pMyself->GetAttackRect(), m_pMyself->GetFocusEnemy()->GetDrawRect(),0.5);

	if (!m_pMyself->GetFocusEnemy()) 
	{
		m_pMyself->SetAction(enAction::eStand);
		m_bHasRun = TRUE;
		return enNodeState::eNodeFalse;
	}
	RECT rect = GetScaledRect(m_pMyself->GetAttackRect(), 0.8,0.8 );
	if (Collide::IsObjectBeAttackAreaCollideWithRect(m_pMyself->GetFocusEnemy(), rect))
	{
		m_pMyself->SetAction(enAction::eStand);
		m_bHasRun = TRUE;
		return enNodeState::eNodeTrue;
	}
	else return enNodeState::eNodeFalse;
}

