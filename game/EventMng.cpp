#include"EventMng.h"
#include"LivingObjectClass.h"
CEventManager::CEventManager()
{
}

CEventManager::~CEventManager()
{

}

void CEventManager::update()
{
	if (m_list_Obj.size())
	{
		list<CLivingObj*>::iterator iter;

		for (iter = m_list_Obj.begin(); iter != m_list_Obj.end(); )
		{
			if((*iter)==NULL)
			{
				iter = m_list_Obj.erase(iter);
			}
			else if (!(*iter)->Attack())
			{
				iter = m_list_Obj.erase(iter);
			}
			else
				iter++;
		}
	}
}

void CEventManager::AttackEventUpdate()
{
}

void CEventManager::stop()
{
}

void CEventManager::GetNewEvent(bool(CLivingObj::*fptr)(void))
{
	m_List_fptr_bool.push_back(fptr);
}

void CEventManager::ObjectTriggerAttack(CLivingObj* obj)
{
	//obj->m_bIsAttacking = TRUE;
	//obj->SetAction(eAttack_1);

	//obj->m_bIsAttackAniEnd = FALSE;

	//obj->m_bIsAttacking = TRUE;

	//obj->m_eAttackDir = obj->GetObjectDir();

	m_list_Obj.push_back(obj);
}

void CEventManager::ClearEvent() 
{
	std::list<CLivingObj*>::iterator iter;

	for (iter = m_list_Obj.begin(); iter != m_list_Obj.end();)
	{
		(*iter)->UnLockAction();
		(*iter) = NULL;
		iter = m_list_Obj.erase(iter);
	}

}

void CEventManager::ClearAttackEvent(CLivingObj* obj) 
{
	std::list<CLivingObj*>::iterator iter;

	for (iter = m_list_Obj.begin(); iter != m_list_Obj.end();)
	{
		if ((*iter) == obj)
		{
			(*iter)->UnLockAction();
			(*iter) = NULL;
			iter=m_list_Obj.erase(iter);
		}		
		else  iter++;
	}
}