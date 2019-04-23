#pragma once
#include"MonsterClass.h"
#include"InputClass.h"
#include"ObjectMng.h"
#include"Damage.h"
#include"Global_data.h"
#include"ObjectMng.h"

CMonster::CMonster(int x, int y, DWORD width, DWORD height, CCommonSpriteRow *image)
{
	m_enObjType = enObjectType::eOTMonster;

	CObject::Init(x, y, width, height);

	m_pSprite = image;
	
	//init Collide Area
	CreateCollideArea(TRUE);
	m_pCollideArea->AddCollideRect(m_rectDraw);

	//Init Attackable Area
	CreateAttackEffectArea();
	m_pAreaBeAttack->AddCollideRect(m_rectDraw);

	CAttribute::Init(&g_pAttMng->GetATTTable(enLOBJ::eMonRabbit), 1);

	g_pAIMng->Add_CommonAI_Monster(this);

	//Update();
}

CMonster::~CMonster()
{
}

HRESULT CMonster::DrawObject(LPDIRECTDRAWSURFACE7 suf, DWORD DrawSetting)
{
	if (m_bIsAlive == FALSE) return FALSE;
	//	UpdateInfo();
	enAction CurAction = GetCurAction();
	enDirection CurDir = GetCurDir();
	switch (CurAction)//character action 
	{
	case eStand://Idle
	{
		switch (CurDir)//character direction
		{
		case eLeft: m_pSprite->DisplayStandAni(eLeft,m_rectDraw, suf); break;
		case eRight: m_pSprite->DisplayStandAni(eRight,m_rectDraw, suf); break;
		case eUp: m_pSprite->DisplayStandAni(eUp, m_rectDraw, suf); break;
		case eDown: m_pSprite->DisplayStandAni(eDown, m_rectDraw, suf); break;
		}
	}break;

	case eWalk://walking
	{
		switch (CurDir)//character direction
		{
		case eLeft: m_pSprite->DisplayWalkAni(eLeft, 0.18, m_rectDraw, suf); break;
		case eRight: m_pSprite->DisplayWalkAni(eRight, 0.18, m_rectDraw, suf); break;
		case eUp: m_pSprite->DisplayWalkAni(eUp, 0.18, m_rectDraw, suf); break;
		case eDown: m_pSprite->DisplayWalkAni(eDown, 0.18, m_rectDraw, suf); break;
		}
	}break;

	case eRun://running
	{
		switch (CurDir)//character direction
		{
		case eLeft: m_pSprite->DisplayWalkAni(eLeft, 0.2, m_rectDraw, suf); break;
		case eRight: m_pSprite->DisplayWalkAni(eRight, 0.2, m_rectDraw, suf); break;
		case eUp: m_pSprite->DisplayWalkAni(eUp, 0.2, m_rectDraw, suf); break;
		case eDown: m_pSprite->DisplayWalkAni(eDown, 0.2, m_rectDraw, suf); break;
		}
	}break;

	case eAttack_1:
	{
		switch (CurDir)
		{
		case eLeft: m_pSprite->DisplayNormalAttackAni(eLeft, GetAtkRate(), m_rectDraw, suf); break;
		case eRight: m_pSprite->DisplayNormalAttackAni(eRight, GetAtkRate(), m_rectDraw, suf); break;
		case eUp: m_pSprite->DisplayNormalAttackAni(eUp, GetAtkRate(), m_rectDraw, suf); break;
		case eDown: m_pSprite->DisplayNormalAttackAni(eDown, GetAtkRate(), m_rectDraw, suf); break;
		}
	}break;
	}

	CLivingObj::DrawUI(suf);

	return TRUE;
}

bool CMonster::Control_USER()
{
	if (m_bIsAlive == FALSE) return FALSE;

	if (IsActionLocked()) return FALSE;//

	enAction CurAction = GetCurAction();

	//Attack_____________________________________________________________________________
	if (DIKEYDOWN(DIK_J))
	{
		PerformAttack();
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

bool CMonster::InitAI()
{
	return TRUE;
}


enAttackStage CMonster::Attack()
{
	//play attack animation
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
		if (m_pSprite->IsAttackAnimationEnd(m_eAttackDir))
		{
			m_bIsAttackAniEnd = TRUE;

			UnLockAction();
			SetAction(eStand);

			m_enAtkStage = eAtkWaitCoolTime;
		}

		if (m_pSprite->GetAttackAniCurFrame(m_eAttackDir) == 0) //Effective Attack Frame
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
		if (AttackTimer.OnceTimer(GetAtkInterval())) //wait attack cool time 
		{
			m_bIsTriggerDamage = FALSE;
			m_bIsAttacking = FALSE;//attack complete
			m_pSprite->ResetAttackAnimation();

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

void CMonster::UpdateAll()
{
	CObject::UpdateObject();

	CLivingObj::UpdateLivingObj();
	
}


