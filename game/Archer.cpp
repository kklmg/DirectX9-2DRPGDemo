#pragma once
#include"Archer.h"
#include"InputClass.h"
#include"ObjectMng.h"
#include"Damage.h"
#include"Global_data.h"
#include"ActionNode.h"

CArcher::CArcher(int x, int y, DWORD width, DWORD height, CArcherImage *image)
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

	//InitAI();

	CAttribute::Init(&g_pAttMng->GetATTTable(enLOBJ::eHeroArcher), 1);

	//UpdateAll();
}

CArcher::~CArcher()
{
}

HRESULT CArcher::DrawObject(LPDIRECTDRAWSURFACE7 suf, DWORD DrawSetting)
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
			case eLeft: m_pImg->DisplayNormalAttackAni(eLeft, CAttribute::GetAtkInterval(), m_rectDraw, suf); break;
			case eRight: m_pImg->DisplayNormalAttackAni(eRight, CAttribute::GetAtkInterval(), m_rectDraw, suf); break;
			case eUp: m_pImg->DisplayNormalAttackAni(eUp, CAttribute::GetAtkInterval(), m_rectDraw, suf); break;
			case eDown: m_pImg->DisplayNormalAttackAni(eDown, CAttribute::GetAtkInterval(), m_rectDraw, suf); break;
			}
			break;
		}
		case eAttack_2: 
		{		
			switch (CurDir)
			{
			case eLeft: m_pImg->DisplayBowAttackAni(eLeft, CAttribute::GetAtkInterval(), m_rectDraw, suf); break;
			case eRight: m_pImg->DisplayBowAttackAni(eRight, CAttribute::GetAtkInterval(), m_rectDraw, suf); break;
			case eUp: m_pImg->DisplayBowAttackAni(eUp, CAttribute::GetAtkInterval(), m_rectDraw, suf); break;
			case eDown: m_pImg->DisplayBowAttackAni(eDown, CAttribute::GetAtkInterval(), m_rectDraw, suf); break;
			}
		}break;		
	}
	
	CLivingObj::DrawUI(suf);

	return TRUE;
}

bool CArcher::Control_USER()
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
	else if (DIKEYDOWN(DIK_K))
	{
		if (m_bIsFightState)
		{
			SetAction(eAttack_2);
			LockAction();

			//g_pEventMng->GetNewEvent(&this->Attack);//attack
			g_pEventMng->ObjectTriggerAttack(this);
		}
	}
	//-----------------------------------------------------------------------------------

	//Run________________________________________________________________________________
	else if (DIKEYDOWN(DIK_LSHIFT))
	{
		if (m_bIsAttacking == FALSE)//when character is attacking will lock move action
		{
			if (DIKEYDOWN(DIK_LEFT) && DIKEYDOWN(DIK_UP)) MoveLeftUp(TRUE);
			else if (DIKEYDOWN(DIK_LEFT) && DIKEYDOWN(DIK_DOWN)) MoveLeftDown(TRUE);
			else if (DIKEYDOWN(DIK_RIGHT) && DIKEYDOWN(DIK_UP)) MoveRightUp(TRUE);
			else if (DIKEYDOWN(DIK_RIGHT) && DIKEYDOWN(DIK_DOWN)) MoveRightDown(TRUE);
			else if (DIKEYDOWN(DIK_LEFT)) MoveLeft(TRUE);
			else if (DIKEYDOWN(DIK_RIGHT)) MoveRight(TRUE);
			else if (DIKEYDOWN(DIK_UP)) MoveUp(TRUE);
			else if (DIKEYDOWN(DIK_DOWN)) MoveDown(TRUE);
		}
	}

	//walk_________________________________________________________________________________
	else if (m_bIsAttacking == FALSE)//when character is attacking will lock move action
		{
			if (DIKEYDOWN(DIK_LEFT) && DIKEYDOWN(DIK_UP)) MoveLeftUp(FALSE);
			else if (DIKEYDOWN(DIK_LEFT) && DIKEYDOWN(DIK_DOWN)) MoveLeftDown(FALSE);
			else if (DIKEYDOWN(DIK_RIGHT) && DIKEYDOWN(DIK_UP)) MoveRightUp(FALSE);
			else if (DIKEYDOWN(DIK_RIGHT) && DIKEYDOWN(DIK_DOWN)) MoveRightDown(FALSE);
			else if (DIKEYDOWN(DIK_LEFT)) MoveLeft(FALSE);
			else if (DIKEYDOWN(DIK_RIGHT)) MoveRight(FALSE);
			else if (DIKEYDOWN(DIK_UP)) MoveUp(FALSE);
			else if (DIKEYDOWN(DIK_DOWN)) MoveDown(FALSE);

			else SetAction(eStand);//no action
		}	
	

	return TRUE;
}

bool CArcher::InitAI() 
{
	//m_AI = new CSelectorNode();

	
	//CActGoToPos *gotopos1 = new CActGoToPos(this, { 300,200 },2);
	//CActGoToPos *gotopos2 = new CActGoToPos(this, { 300,300 },2);
	//CActGoToPos *gotopos3 = new CActGoToPos(this, { 300,400 },2);
	//CActWait *wait1 = new CActWait(2);
	//CActWait *wait2 = new CActWait(2);
	//CActTurn *turn = new CActTurn(this);


	//m_AI->AddChild(gotopos1);
	//m_AI->AddChild(wait1);

	//m_AI->AddChild(gotopos2);
	//m_AI->AddChild(turn);
	//m_AI->AddChild(wait2);
	//m_AI->AddChild(gotopos3);
	return TRUE;
}

enAttackStage CArcher::Attack()
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

void CArcher::UpdateAll()
{
	CObject::UpdateObject();
	CLivingObj::UpdateLivingObj();
	CHero::UpdateHero();
}
