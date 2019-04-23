#pragma once
//-----------------------------------------------
//Head File
//-----------------------------------------------
#include<list>

//-----------------------------------------------
//My Head File
//-----------------------------------------------
#include"ObjectClass.h"
#include"Attribute.h"
#include"BasicBevNode.h"
#include"TimerClass.h"
#include"SequenceAni.h"
#include"ObjectUI.h"
#include"Astar.h"
#include"CTalk.h"


//-----------------------------------------------
//Class Living Object
//-----------------------------------------------
class CLivingObj :virtual public CObject, public CAttribute,public CTalk
{
public:
	CLivingObj();
	virtual~CLivingObj();

	//-----------------------------------------------
	//Update Living object
	//-----------------------------------------------
	void UpdateLivingObj();

	//-----------------------------------------------
	//Object Move Related:: User Control
	//-----------------------------------------------
public:
	void MoveLeft(bool IsRun);
	void MoveRight(bool IsRun);
	void MoveUp(bool IsRun);
	void MoveDown(bool IsRun);

	void MoveLeftUp(bool IsRun);
	void MoveRightUp(bool IsRun);
	void MoveLeftDown(bool IsRun);
	void MoveRightDown(bool IsRun);

	//-----------------------------------------------
	//User Control
	//-----------------------------------------------
	virtual bool Control_USER() = 0;//get user input


	//-----------------------------------------------
	//Object Action Related:: AI Control
	//-----------------------------------------------
public:
	void ChaseObj(CLivingObj* obj);

	void FaceToPoint(POINTFLOAT pointf);
	void FaceToObject(CObject* obj);

	void ClearCurPath();

	bool MoveToPoint(D3DXVECTOR2 v2, bool IsRun,float offset);
	bool MoveToPoint_ByLine();
	bool MoveToPoint_Smart(POINTFLOAT Dest);
	

	bool MoveToObject(CObject *obj, bool IsRun);
	bool MoveToObject_Smart(CObject *obj, bool IsRun, float offset);


	void ChangeDirRandomly();


	bool CheckObjectInAtkRng(CLivingObj *obj);
	bool CheckObjectInDetectRng(CLivingObj *obj);


	bool GetNearByObj(std::list<CLivingObj*>&list);
	CLivingObj *GetNearestObj(std::list<CLivingObj*>&list);


	CDialogChain* TryTalk();

protected:
	STPathUpdate m_stPathUpdate;

	//-----------------------------------------------
	//Detect Enemy
	//-----------------------------------------------
public:
	bool DetectEnemy();

	bool FocusNearestEnemy();

	void ClearDetected();

	CLivingObj *GetFocusEnemy();

	CLivingObj *m_pFocusEnemy;
	//detected enemies list
	list<CLivingObj*>m_listDetectedEnemy;

	list<CLivingObj*> GetDetectedEnemyList();

	list<POINT> m_list_PathLine;


	//-----------------------------------------------
	//object AI 
	//-----------------------------------------------
public:
	bool Control_COM();

	CCompositeNode* GetAI();

	bool AddBevNode(CBevNode *node);

protected:
	virtual bool InitAI() = 0;

	CCompositeNode* m_AI = NULL;
	//-----------------------------------------------
	//object Battle
	//-----------------------------------------------
public:
	void GetDamage(int damage);

	RECT GetAttackRect();

	RECT GetAttackRect_E();

protected:
	enAttackStage m_enAtkStage = enAttackStage::eAtkEnd;

public:
	enAttackStage GetAtkStage()const;

	virtual enAttackStage Attack() = 0;

	void PerformAttack();

	void UpdateAtkRange();

	CTimer AttackTimer;

	enDirection m_eAttackDir;

	bool m_bIsAttackAniEnd;

	bool m_bIsAttacking;

	bool m_bIsTriggerDamage;

	//attack range
	RECT m_rectAtkRange[4];

protected:
	bool m_bIsFightState;
	//--------------------------------------------------
	//object Attack Effective Area
	//--------------------------------------------------
protected:
	CCollideArea *m_pAreaBeAttack;

	void CreateAttackEffectArea();

	void DestroyAttackArea();
public:
	CCollideArea *GetBeAttackArea()const;


	//-----------------------------------------------
	//object Animation || Action
	//-----------------------------------------------
public:
	bool SetAction(enAction act);

	bool StopAction();
	
	void LockAction();

	void UnLockAction();

	bool IsActionLocked() const;

	enAction GetCurAction()const;
	
private:
	enAction m_enAct;

	bool m_bLockAction;

	//-----------------------------------------------
	//Object UI
	//-----------------------------------------------
public:
	STObjUI m_stObjUI;
protected:
	void UpdateUI();

	void DrawUI(LPDIRECTDRAWSURFACE7 suf);

	void DisplayDetectRange(LPDIRECTDRAWSURFACE7 suf);

	//-----------------------------------------------
	//Object Tag
	//-----------------------------------------------
public:
	void SetForce(enObjectForce force);

	enObjectForce GetForce();

	void AddtoControlVector();



	//TEST
	public :
		std::list<CBevNode*> AI_path;
};
