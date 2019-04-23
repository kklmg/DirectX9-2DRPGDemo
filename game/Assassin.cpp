#pragma once
#include"Assassin.h"
#include"InputClass.h"
#include"ObjectMng.h"
#include"Damage.h"
#include"Global_data.h"
#include"ObjectMng.h"

CAssassin::CAssassin(int x, int y, DWORD width, DWORD height, CCommonSpriteSheet *image)
{
	CObject::Init(x, y, width, height);

	//Add Collide Rect
	int __height = GetRectHeight(m_rectDraw);
	RECT rectFoot =
	{
		m_rectDraw.left,
		m_rectDraw.top + (__height / 3) * 2,
		m_rectDraw.right,
		m_rectDraw.bottom
	};

	//init Collide Area
	CreateCollideArea(TRUE);
	m_pCollideArea->AddCollideRect(rectFoot);

	//Init Attackable Area
	CreateAttackEffectArea();
	m_pAreaBeAttack->AddCollideRect(m_rectDraw);

	m_pImg = image;

	CAttribute::Init(&g_pAttMng->GetATTTable(enLOBJ::eHeroAssasin), 1);

	//Update();
}

CAssassin::~CAssassin()
{
}

HRESULT CAssassin::DrawObject(LPDIRECTDRAWSURFACE7 suf, DWORD DrawSetting)
{
	if (m_bIsAlive == FALSE) return FALSE;

	//character action animation
	enAction CurAction = GetCurAction();
	enDirection CurDir = GetCurDir();

	switch (CurAction)//character action 
	{
	case eStand://Idle
	{
		switch (CurDir)//character direction
		{
		case eLeft: m_pImg->DisplayStandAni(eLeft, 0.2, m_rectDraw, suf); break;
		case eRight: m_pImg->DisplayStandAni(eRight, 0.2, m_rectDraw, suf); break;
		case eUp: m_pImg->DisplayStandAni(eUp, 0.2, m_rectDraw, suf); break;
		case eDown: m_pImg->DisplayStandAni(eDown, 0.2, m_rectDraw, suf); break;
		}
	}break;

	case eWalk:
	case eRun:
	{
		switch (CurDir)//character direction
		{
		case eLeft: m_pImg->DisplayWalkAni(eLeft, 0.15, m_rectDraw, suf); break;
		case eRight: m_pImg->DisplayWalkAni(eRight, 0.15, m_rectDraw, suf); break;
		case eUp: m_pImg->DisplayWalkAni(eUp, 0.15, m_rectDraw, suf); break;
		case eDown: m_pImg->DisplayWalkAni(eDown, 0.15, m_rectDraw, suf); break;
		}
	}break;

	case eAttack_1:
	{
		switch (CurDir)
		{
		case eLeft: m_pImg->DisplayNormalAttackAni(eLeft, GetAtkInterval(), m_rectDraw, suf); break;
		case eRight: m_pImg->DisplayNormalAttackAni(eRight, GetAtkInterval(), m_rectDraw, suf); break;
		case eUp: m_pImg->DisplayNormalAttackAni(eUp, GetAtkInterval(), m_rectDraw, suf); break;
		case eDown: m_pImg->DisplayNormalAttackAni(eDown, GetAtkInterval(), m_rectDraw, suf); break;
		}
		break;
	}

	}
	CLivingObj::DrawUI(suf);
	return TRUE;
}

bool CAssassin::Control_USER()
{
	if (m_bIsAlive == FALSE) return FALSE;

	if (IsActionLocked()) return FALSE;//

	enAction CurAction = GetCurAction();

	//Attack_____________________________________________________________________________
	if (DIKEYDOWN(DIK_J))
	{
		if (m_bIsFightState)
		{
			PerformAttack();
		}
	}
	//-----------------------------------------------------------------------------------

	//Run________________________________________________________________________________
	else if (DIKEYDOWN(DIK_LSHIFT))
	{
			if (DIKEYDOWN(DIK_LEFT) && DIKEYDOWN(DIK_UP)) MoveLeftUp(TRUE);
			else if (DIKEYDOWN(DIK_LEFT) && DIKEYDOWN(DIK_DOWN)) MoveLeftDown(TRUE);
			else if (DIKEYDOWN(DIK_RIGHT) && DIKEYDOWN(DIK_UP)) MoveRightUp(TRUE);
			else if (DIKEYDOWN(DIK_RIGHT) && DIKEYDOWN(DIK_DOWN)) MoveRightDown(TRUE);
			else if (DIKEYDOWN(DIK_LEFT)) MoveLeft(TRUE);
			else if (DIKEYDOWN(DIK_RIGHT)) MoveRight(TRUE);
			else if (DIKEYDOWN(DIK_UP)) MoveUp(TRUE);
			else if (DIKEYDOWN(DIK_DOWN)) MoveDown(TRUE);
			else SetAction(eStand);//no action
	}

	//walk_________________________________________________________________________________
	else 
	{
		if (DIKEYDOWN(DIK_LEFT) && DIKEYDOWN(DIK_UP)) MoveLeftUp(FALSE);
		else if (DIKEYDOWN(DIK_LEFT) && DIKEYDOWN(DIK_DOWN)) MoveLeftDown(FALSE);
		else if (DIKEYDOWN(DIK_RIGHT) && DIKEYDOWN(DIK_UP)) MoveRightUp(FALSE);
		else if (DIKEYDOWN(DIK_RIGHT) && DIKEYDOWN(DIK_DOWN)) MoveRightDown(FALSE);
		else if (DIKEYDOWN(DIK_LEFT)) MoveLeft(FALSE);
		else if (DIKEYDOWN(DIK_RIGHT)) MoveRight(FALSE);
		else if (DIKEYDOWN(DIK_UP)) MoveUp(FALSE);
		else if (DIKEYDOWN(DIK_DOWN)) MoveDown(FALSE);
		else SetAction(eStand);
	}

	return TRUE;
}

bool CAssassin::InitAI() 
{
	return TRUE;
}

enAttackStage CAssassin::Attack()
{
	//play attack animation
	switch (m_enAtkStage)
	{
	case eAtkEnd:
		break;
	case eAtkInit: 
	{
		SetAction(eAttack_1);
		LockAction();
	
		m_bIsAttackAniEnd = FALSE;

		m_bIsAttacking = TRUE;

		m_eAttackDir = GetCurDir();

		m_enAtkStage = eAtkRunning;
	}
		break;
	case eAtkRunning: 
	{
		if (m_pImg->IsAttackAnimationEnd(m_eAttackDir))
		{
			m_bIsAttackAniEnd = TRUE;
			UnLockAction();
			m_enAtkStage = eAtkWaitCoolTime;
		}
	
		if (m_pImg->GetAttackAniCurFrame(m_eAttackDir) == 2) //Effective Attack Frame
		{
			if (!m_bIsTriggerDamage)//the damage only trigger once
			{
				g_pQuadTree->PerformAttack(this);

				m_bIsTriggerDamage = TRUE;//for avoid the target get damage continuously
			}
		}

	
	}break;
	case eAtkWaitCoolTime: 
	{
		if (AttackTimer.OnceTimer(0.2)) //wait attack cool time 
		{
			UnLockAction();
			SetAction(eStand);

			m_bIsTriggerDamage = FALSE;
			m_bIsAttacking = FALSE;//attack complete
			m_pImg->ResetAttackAnimation();

			AttackTimer.ResetTimer();

			m_enAtkStage = eAtkEnd;
		}	
	}
		break;
	default:
		break;
	}
	return m_enAtkStage;
}

void CAssassin::UpdateAll()
{
	CObject::UpdateObject();

	CLivingObj::UpdateLivingObj();

	CHero::UpdateHero();
}


void AttackInit() 
{

}

void AttackUpdate() 
{


}

void WaitAttackCoolTime() 
{


}

void Attackend() 
{


}

//class CAttack
//{
//public:
//	CAttack();
//	~CAttack();
//
//private:
//
//	//	bool m_bIsAttackAniEnd = FALSE;
//	//
//	//	bool m_bIsAttacking = TRUE;
//	//
//	//	bool m_eAttackDir = m_enDir;
//	//
//
//};
//
//CAttack::CAttack()
//{
//}
//
//CAttack::~CAttack()
//{
//}
//struct Attack 
//{
//	//CLivingObj
//	bool m_bIsAttackAniEnd;
//
//	bool m_bIsAttackAniEnd = FALSE;
//
//	bool m_bIsAttacking = TRUE;
//
//	bool m_eAttackDir = m_enDir;
//
//	void Init() 
//	{
//	
//	
//	
//	
//	}
//
//	bool operator()() 
//	{
//	
//
//
//
//
//	}
//
//	//play attack animation
//	m_enAct = eAttack_1;
//
//	if (!m_bIsAttacking)
//	{
//
//		m_bIsAttackAniEnd = FALSE;
//
//		m_bIsAttacking = TRUE;
//
//		m_eAttackDir = m_enDir;
//	}
//
//	//complete attack animation
//	if (m_pImg->IsAttackAnimationEnd(m_eAttackDir))
//	{
//		m_bIsAttackAniEnd = TRUE;
//
//		if (AttackTimer.OnceTimer(0.2)) //wait attack cool time 
//		{
//			m_enAct = eStand;
//			m_bIsTriggerDamage = FALSE;
//			m_bIsAttacking = FALSE;//attack complete
//			m_pImg->ResetAttackAnimation();
//
//			AttackTimer.ResetTimer();
//			return FALSE;
//		}
//	}
//
//	if (m_pImg->GetAttackAniCurFrame(m_eAttackDir) == 2) //Effective Attack Frame
//	{
//		if (!m_bIsTriggerDamage)//the damage only trigger once
//		{
//			//			g_pQuadTree->PerformAttack(this);
//
//			m_bIsTriggerDamage = TRUE;//for avoid the target get damage continuously
//		}
//	}
//
//	return TRUE;
//
//};