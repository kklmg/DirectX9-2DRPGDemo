#pragma once
#include<list>
#include"Knight.h"


class CEventManager
{
public:
	CEventManager();

	~CEventManager();

	void update();

	void AttackEventUpdate();

	void stop();

	void ObjectTriggerAttack(CLivingObj* obj);

	void GetNewEvent(bool (CLivingObj::*fptr)(void));

	void ClearEvent();

	void ClearAttackEvent(CLivingObj* obj);

private:
	list<bool (CLivingObj::*)(void)>m_List_fptr_bool;

	//object Event
	list<CLivingObj*> m_list_Obj;
};

