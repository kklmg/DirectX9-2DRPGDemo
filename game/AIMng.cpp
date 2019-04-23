//--------------------------------------------
//My Head File
//--------------------------------------------
#include"AIMng.h"
#include"ActionNode.h"
#include"Global_data.h"


//--------------------------------------------
//Class AI Manager
//--------------------------------------------
CAIManager::CAIManager()
{
}

CAIManager::~CAIManager()
{
}

bool CAIManager::Init()
{
	return TRUE;
}

//--------------------------------------------
//Create AI Node 
//--------------------------------------------
bool CAIManager::AddBev(CLivingObj*pObj, CBevNode *node) 
{
	if (node == NULL) return FALSE;

	pObj->GetAI()->AddChild(node);

	return TRUE;
}

CBevNode* CAIManager::CreateBev_Follow(CLivingObj*myself, CLivingObj *target)
{
	CSequenceNode *seq_follow = new CSequenceNode();

	CConFarAway_obj* FarAway = new CConFarAway_obj(myself, target, 200);

	CActFollow *follow = new CActFollow(myself, target);

	CRepeatNode *repeat = new CRepeatNode();

	seq_follow->AddChild(FarAway);

	seq_follow->AddChild(follow);

	repeat->setChild(seq_follow);

	myself->GetAI()->AddChild(repeat);

	CActGoToPos* gotopos = new CActGoToPos(myself, target->GetPosition(), 2);
	

	
	//seq_follow->AddChild(follow);
	
	
	

	
	CActGoToPos *gotopos1 = new CActGoToPos(myself, { 300,200 },2);
	//CActGoToPos *gotopos2 = new CActGoToPos(myself, { 300,300 },2);
	//CActGoToPos *gotopos3 = new CActGoToPos(myself, { 300,400 },2);
	//CActWait *wait1 = new CActWait(2);
	//CActWait *wait2 = new CActWait(2);
	//CActTurn *turn = new CActTurn(myself);


	//myself->GetAI()->AddChild(gotopos1);
	//myself->GetAI()->AddChild(wait1);

	//myself->GetAI()->AddChild(gotopos2);
	//myself->GetAI()->AddChild(turn);
	//myself->GetAI()->AddChild(wait2);
	//myself->GetAI()->AddChild(gotopos3);

	//myself->GetAI()->AddChild(seq_follow);

	
	return repeat;
}

CBevNode* CAIManager::CreateBev_FollowEX(CLivingObj*myself, CLivingObj *target)
{
	//follow part
	CSequenceNode *seq_follow = new CSequenceNode();

	CConFarAway_obj* leaf_FarAway = new CConFarAway_obj(myself, target, 200);

	CActFollow *leaf_follow = new CActFollow(myself, target);

	CRepeatNode *rep_follow = new CRepeatNode();

	seq_follow->AddChild(leaf_FarAway);

	seq_follow->AddChild(leaf_follow);

	rep_follow->setChild(seq_follow);


	//patrol part
	CActWalkAround_time *leaf_walkaround = new CActWalkAround_time(myself, 50);

	CActTurn *leaf_Turn = new CActTurn(myself);

	CRepeatNode *rep_patrol = new CRepeatNode();

	CRandomSelector *randsle = new CRandomSelector();

	CActWait *leaf_wait1 = new CActWait(myself,2);

	CActWait *leaf_wait2 = new CActWait(myself,2);

	CSequenceNode *seq_turn = new CSequenceNode();

	CSequenceNode *seq_walk = new CSequenceNode();



	//turn behavior
	seq_turn->AddChild(leaf_Turn);

	seq_turn->AddChild(leaf_wait1);

	//walk behavior
	seq_walk->AddChild(leaf_walkaround);

	seq_walk->AddChild(leaf_wait2);

	//random selector
	randsle->AddChild(seq_turn);

	randsle->AddChild(seq_walk);

	rep_patrol->setChild(randsle);

	//euip ai
	AddBev(myself, rep_follow);

	AddBev(myself, rep_patrol);

	return rep_patrol;
}

CBevNode* CAIManager::CreateBev_FollowUser(CLivingObj*myself,DWORD KeepDistance)
{
	CSequenceNode *seq_follow = new CSequenceNode();

	CConFarAway_User* leaf_FarAway = new CConFarAway_User(myself, KeepDistance);

	CActFollowUser *leaf_follow = new CActFollowUser(myself);

	//CRepeatNode *rep_follow = new CRepeatNode();

	seq_follow->AddChild(leaf_FarAway);

	seq_follow->AddChild(leaf_follow);

	//rep_follow->setChild(seq_follow);

	return seq_follow;
	//return rep_follow;
}

CBevNode* CAIManager::CreateBev_WalkAround(CLivingObj*myself, float range)
{
	CActWalkAround_time *walkaround = new CActWalkAround_time(myself,range,1);

	CActTurn *Turn = new CActTurn(myself);

	//CRepeatNode *repeat = new CRepeatNode();

	CRandomSelector *randselector = new CRandomSelector();
	
	CActWait *wait1 = new CActWait(myself,1.5);

	CActWait *wait2 = new CActWait(myself,1.5);

	CSequenceNode *seq1 = new CSequenceNode();

	CSequenceNode *seq2 = new CSequenceNode();

	//add action turn
	seq1->AddChild(Turn);

	seq1->AddChild(wait1);

	//add action walk around

	seq2->AddChild(walkaround);

	seq2->AddChild(wait2);

	randselector->AddChild(seq1);

	randselector->AddChild(seq2);

	//repeat->setChild(randselector);

	return randselector;
	//return repeat;
}

CBevNode* CAIManager::CreateBev_CommonAttack(CLivingObj *myself)
{
	CSequenceNode *seq_root = new CSequenceNode();

	CSelectorNode *slector = new CSelectorNode();

	CConIsEnemyNearBy *leaf_IsEnemyNearby = new CConIsEnemyNearBy(myself);

	CConIsEnemyInMyRange *leaf_IsEnemyInMyAtkRange = new CConIsEnemyInMyRange(myself);

	CActFollowFocusEnemy *leaf_ChaseEnemy = new CActFollowFocusEnemy(myself);

	CActAttack *leaf_attack = new CActAttack(myself);

	CRepeatNode *rep_DetectEnemy= new CRepeatNode();

	CRepeatNode *rep_Attack = new CRepeatNode();

	CSequenceNode *seq_Attack = new CSequenceNode();


	slector->AddChild(leaf_IsEnemyInMyAtkRange);

	slector->AddChild(leaf_ChaseEnemy);


	seq_Attack->AddChild(slector);

	seq_Attack->AddChild(leaf_attack);

	rep_Attack->setChild(seq_Attack);


	rep_DetectEnemy->setChild(leaf_IsEnemyNearby);

	seq_root->AddChild(rep_DetectEnemy);

	seq_root->AddChild(rep_Attack);

	//repeatNode->setChild(seq_root);

	return seq_root;
	//return repeatNode;
}

//--------------------------------------------
//Create AI Auto 
//--------------------------------------------
bool CAIManager::Add_CommonAI_Hero(CLivingObj*myself)
{
	CBevNode* bev_follow = CreateBev_FollowUser(myself, 250);

	CBevNode* bev_Patrol = CreateBev_WalkAround(myself,200);

	CBevNode* bev_Attack = CreateBev_CommonAttack(myself);

	CSelectorNode *sle = new CSelectorNode();
	//
	CRepeatNode *repeat = new CRepeatNode();

	sle->AddChild(bev_follow);
	sle->AddChild(bev_Attack);
	sle->AddChild(bev_Patrol);
	//
	repeat->setChild(sle);

	AddBev(myself, repeat);

	//AddBev(myself, bev_follow);
	
	//AddBev(myself, bev_Attack);

	//AddBev(myself, bev_Patrol);

	return TRUE;
}

bool CAIManager::Add_CommonAI_Monster(CLivingObj *monster)
{
	CBevNode* bev_Patrol = CreateBev_WalkAround(monster, 200);

	CBevNode* bev_Attack = CreateBev_CommonAttack(monster);

	CSelectorNode *sle = new CSelectorNode();
	//
	CRepeatNode *repeat = new CRepeatNode();

	sle->AddChild(bev_Attack);
	sle->AddChild(bev_Patrol);
	//
	repeat->setChild(sle);

	AddBev(monster, repeat);

	return TRUE;
}

bool CAIManager::Add_CommonAI_NPC(CLivingObj *NPC)
{
	CBevNode* bev_Patrol = CreateBev_WalkAround(NPC, 200);

	CRepeatNode *repeat = new CRepeatNode();

	repeat->setChild(bev_Patrol);

	AddBev(NPC, repeat);

	return TRUE;
}