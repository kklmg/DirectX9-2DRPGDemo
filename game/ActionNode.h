#pragma once
//---------------------------------------------------
//My Head File
//---------------------------------------------------
#include"BasicBevNode.h"
#include"LivingObjectClass.h"
#include"QuadTree.h"
#include"TimerClass.h"

//---------------------------------------------------
//Class Leaf Node
//---------------------------------------------------
class CLeafNode : public CBevNode
{
public:
	CLeafNode(CLivingObj *Myself);
	~CLeafNode();

	virtual void ResetNodeState();

	virtual void ClearInited();

	virtual void Destroy();

protected:
	CLivingObj *m_pMyself;
};

//---------------------------------------------------
//Action Node:: Waiting ,nothing to do
//---------------------------------------------------
class CActWait : public CLeafNode
{
public:
	CActWait(CLivingObj *Myself,float lastTime);
	~CActWait();

	virtual enNodeState run();

private:
	float m_fLastTime;

	CTimer m_Timer;	
};

//---------------------------------------------------
//Action Node:: Change Direction randomly
//---------------------------------------------------
class CActTurn : public CLeafNode
{
public:
	CActTurn(CLivingObj *myself);
	~CActTurn();

	virtual enNodeState run();
};

//---------------------------------------------------
//Action Node:: Move to Designated Postion
//---------------------------------------------------
class CActGoToPos : public CLeafNode
{
public:
	CActGoToPos(CLivingObj *myself, POINTFLOAT position,float offset);
	~CActGoToPos();

	virtual enNodeState run();

private:
	POINTFLOAT m_Pointf;

	float m_fOffset;
};

//---------------------------------------------------
//Action Node:: Move to Target
//---------------------------------------------------
class CActFollow : public CLeafNode
{
public:
	CActFollow(CLivingObj *myself ,CLivingObj *target);
	~CActFollow();

	virtual enNodeState run();

private:
	CLivingObj *m_pTarget;
};

//---------------------------------------------------
//Action Node:: Move to User
//---------------------------------------------------
class CActFollowUser : public CLeafNode
{
public:
	CActFollowUser(CLivingObj *myself);
	~CActFollowUser();

	virtual enNodeState run();
};

//---------------------------------------------------
//Action Node:: Move to Detected Enemy
//---------------------------------------------------
class CActFollowFocusEnemy : public CLeafNode
{
public:
	CActFollowFocusEnemy(CLivingObj *myself);
	virtual ~CActFollowFocusEnemy();

	virtual enNodeState run();
};

//---------------------------------------------------
//Action Node:: Walk Around
//---------------------------------------------------
class CActWalkAround : public CLeafNode
{
public:
	CActWalkAround(CLivingObj *myself,int radius);
	~CActWalkAround();

	virtual enNodeState run();

protected:
	int m_nRadius;
	POINTFLOAT m_ptPos;
};


class CActWalkAround_time : public CLeafNode
{
public:
	CActWalkAround_time(CLivingObj *myself, int radius,float LimitTime = 4);
	~CActWalkAround_time();

	virtual enNodeState run();

protected:
	float m_fTime;

	int m_nRadius;
	POINTFLOAT m_ptPos;
	CTimer m_Timer;
};

//---------------------------------------------------
//Action Node:: Perform Attack 
//---------------------------------------------------
class CActAttack: public CLeafNode
{
public:
	CActAttack(CLivingObj *myself);
	~CActAttack();

	virtual enNodeState run();
};




//----------------------------------------------------------
//Condition Node:: check Distance between Position and me
//----------------------------------------------------------
class CConNear_pt : public CLeafNode
{
public:
	CConNear_pt(CLivingObj *myself, POINTFLOAT position, float Distance);
	~CConNear_pt();

	virtual enNodeState run();

private:
	POINTFLOAT m_Pointf;

	float m_fDistance;
};

class CConFarAway_pt : public CLeafNode
{
public:
	CConFarAway_pt(CLivingObj *myself, POINTFLOAT position, float Distance);
	~CConFarAway_pt();

	virtual enNodeState run();

private:
	POINTFLOAT m_Pointf;

	float m_fDistance;
};

//----------------------------------------------------------
//Condition Node:: check Distance between Object and me
//----------------------------------------------------------
class CConNear_obj : public CLeafNode
{
public:
	CConNear_obj(CLivingObj *myself, CLivingObj *target, float Distance);
	~CConNear_obj();

	virtual enNodeState run();

private:
	CLivingObj *m_pTarget;

	float m_fDistance;
};

class CConFarAway_obj : public CLeafNode
{
public:
	CConFarAway_obj(CLivingObj *myself, CLivingObj *target, float Distance);
	~CConFarAway_obj();

	virtual enNodeState run();

private:
	CLivingObj *m_pTarget;

	float m_fDistance;
};

//----------------------------------------------------------
//Condition Node:: check Distance between User and me
//----------------------------------------------------------
class CConNear_User : public CLeafNode
{
public:
	CConNear_User(CLivingObj *myself,float Distance);
	~CConNear_User();

	virtual enNodeState run();

private:
	float m_fDistance;
};

class CConFarAway_User : public CLeafNode
{
public:
	CConFarAway_User(CLivingObj *myself, float Distance);
	~CConFarAway_User();

	virtual enNodeState run();

private:
	float m_fDistance;
};



//----------------------------------------------------------
//Condition Node:: check if there are enemies nearby
//----------------------------------------------------------
class CConIsEnemyNearBy : public CLeafNode
{
public:
	CConIsEnemyNearBy(CLivingObj* myself);
	~CConIsEnemyNearBy();
	
	virtual enNodeState run();
};

//----------------------------------------------------------
//Condition Node:: check if the enemy is in my attack range
//----------------------------------------------------------
class CConIsEnemyInMyRange :public CLeafNode
{
public:
	CConIsEnemyInMyRange(CLivingObj* myself);
	~CConIsEnemyInMyRange();

	virtual enNodeState run(); 
};

