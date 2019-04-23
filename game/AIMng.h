#pragma once

//--------------------------------------------
//My Head File
//--------------------------------------------
#include"BasicBevNode.h"
#include"LivingObjectClass.h"


//enum enBev{eFollow,ePatrol,};


//--------------------------------------------
//Class AI Manager
//--------------------------------------------
class CAIManager
{
public:
	CAIManager();
	~CAIManager();

	bool Init();

	bool AddBev(CLivingObj*pObj, CBevNode *node);

	bool Add_CommonAI_Hero(CLivingObj *myself);

	bool Add_CommonAI_Monster(CLivingObj *monster);

	bool Add_CommonAI_NPC(CLivingObj *NPC);

private:

	CBevNode* CreateBev_Follow(CLivingObj*myself, CLivingObj *target);

	CBevNode* CreateBev_FollowEX(CLivingObj*myself, CLivingObj *target);

	CBevNode* CreateBev_FollowUser(CLivingObj*myself,DWORD KeepDistance);

	CBevNode* CreateBev_WalkAround(CLivingObj*myself, float range);

	CBevNode* CreateBev_CommonAttack(CLivingObj *myself);
};