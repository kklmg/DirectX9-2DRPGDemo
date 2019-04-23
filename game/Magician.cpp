#pragma once
#include"Magician.h"
#include"InputClass.h"
#include"ObjectMng.h"
#include"Damage.h"
#include"Global_data.h"
#include"ObjectMng.h"

CMagician::CMagician(int x, int y, DWORD width, DWORD height, CCommonSpriteSheet *image)
{
	CObject::Init(x, y, width, height);

	m_pImg = image;

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

	CAttribute::Init(&g_pAttMng->GetATTTable(enLOBJ::eHeroMagician), 1);

	//Update();
}

CMagician::~CMagician()
{
}

HRESULT CMagician::DrawObject(LPDIRECTDRAWSURFACE7 suf, DWORD DrawSetting)
{
	if (m_bIsAlive == FALSE) return FALSE;
	//	UpdateInfo();
	enAction CurAction = GetCurAction();
	enDirection CurDir = GetCurDir();
	//character action animation
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

bool CMagician::Control_USER()
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

bool CMagician::InitAI()
{
	return TRUE;
}

enAttackStage CMagician::Attack()
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

void CMagician::UpdateAll()
{
	CObject::UpdateObject();

	CLivingObj::UpdateLivingObj();

	CHero::UpdateHero();
}
