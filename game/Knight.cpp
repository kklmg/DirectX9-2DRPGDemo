#pragma once
#include"Knight.h"
#include"InputClass.h"
#include"ObjectMng.h"
#include"Damage.h"
#include"Global_data.h"
#include"ObjectMng.h"
#include"collide.h"
#include"ActionNode.h"

CKnight::CKnight(int x, int y, DWORD width, DWORD height, CKnightImage *image)
	:m_pImg(image)
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

	//Init Attribute
	CAttribute::Init(&g_pAttMng->GetATTTable(enLOBJ::eHeroKnight),1);
	

	//Update();
}

CKnight::~CKnight()
{
}

HRESULT CKnight::DrawObject(LPDIRECTDRAWSURFACE7 suf, DWORD DrawSetting) 
{
	if (m_bIsAlive == FALSE) return FALSE;
//	UpdateInfo();
	enAction CurAction = GetCurAction();
	enDirection CurDir = GetCurDir();
	//if character in fight state
	if (m_bIsFightState == FALSE)
	{
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

			case eWalk://walking
			{
				switch (CurDir)//character direction
				{
				case eLeft: m_pImg->DisplayWalkAni(eLeft, 0.2, m_rectDraw, suf); break;
				case eRight: m_pImg->DisplayWalkAni(eRight, 0.2, m_rectDraw, suf); break;
				case eUp: m_pImg->DisplayWalkAni(eUp, 0.2, m_rectDraw, suf); break;
				case eDown: m_pImg->DisplayWalkAni(eDown, 0.2, m_rectDraw, suf); break;
				}
			}break;

			case eRun://running
			{
				switch (CurDir)//character direction
				{
				case eLeft: m_pImg->DisplayRunAni(eLeft, 0.2, m_rectDraw, suf); break;
				case eRight: m_pImg->DisplayRunAni(eRight, 0.2, m_rectDraw, suf); break;
				case eUp: m_pImg->DisplayRunAni(eUp, 0.2, m_rectDraw, suf); break;
				case eDown: m_pImg->DisplayRunAni(eDown, 0.2, m_rectDraw, suf); break;
				}
			}break;
		}
	}
	else if (m_bIsFightState == TRUE)
	{
		switch (CurAction)
		{
			case eStand://Idle
			{
				switch (CurDir)//character direction
				{
				case eLeft: m_pImg->DisplayStandAni_F(eLeft, 0.2, m_rectDraw, suf); break;
				case eRight: m_pImg->DisplayStandAni_F(eRight, 0.2, m_rectDraw, suf); break;
				case eUp: m_pImg->DisplayStandAni_F(eUp, 0.2, m_rectDraw, suf); break;
				case eDown: m_pImg->DisplayStandAni_F(eDown, 0.2, m_rectDraw, suf); break;
				}
			}break;

			case eWalk://walking
			{
				switch (CurDir)//character direction
				{
				case eLeft: m_pImg->DisplayWalkAni_F(eLeft, 0.2, m_rectDraw, suf); break;
				case eRight: m_pImg->DisplayWalkAni_F(eRight, 0.2, m_rectDraw, suf); break;
				case eUp: m_pImg->DisplayWalkAni_F(eUp, 0.2, m_rectDraw, suf); break;
				case eDown: m_pImg->DisplayWalkAni_F(eDown, 0.2, m_rectDraw, suf); break;
				}
			}break;

			case eRun://running
			{
				switch (CurDir)//character direction
				{
				case eLeft: m_pImg->DisplayRunAni_F(eLeft, 0.2, m_rectDraw, suf); break;
				case eRight: m_pImg->DisplayRunAni_F(eRight, 0.2, m_rectDraw, suf); break;
				case eUp: m_pImg->DisplayRunAni_F(eUp, 0.2, m_rectDraw, suf); break;
				case eDown: m_pImg->DisplayRunAni_F(eDown, 0.2, m_rectDraw, suf); break;
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
			}break;
		}
	}

	CLivingObj::DrawUI(suf);

	return TRUE;
}

bool CKnight::Control_USER()
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
		//if (m_bIsAttacking == FALSE)//when character is attacking will lock move action
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
	}

	//walk_________________________________________________________________________________
	else
	{
		//if (m_bIsAttacking == FALSE)//when character is attacking will lock move action
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
	}
	//if(DIKEYDOWN(DIK_LEFT)||)
	return TRUE;
}

bool CKnight::InitAI() 
{
	//m_AI = new CSequenceNode();
	//CActGoToPos *gotopos1 = new CActGoToPos(this, { 200,200 }, 2);
	//CActGoToPos *gotopos2 = new CActGoToPos(this, { 500,100 }, 2);
	//CActGoToPos *gotopos3 = new CActGoToPos(this, { 1000,200 }, 2);
	//CActWait *wait1 = new CActWait(5);
	
	//AI->AddChild(gotopos1);
	//AI->AddChild(wait1);
	//AI->AddChild(gotopos2);
	//AI->AddChild(gotopos3);

	//AI->AddChild();
	//CFollowObj *bevFollow = new CFollowObj(this,this);

	//CBevIsinAtkRange *test = new CBevIsinAtkRange();
	//test->m_pTarget = this;
	//test->m_pMyself = this;

	//AI->AddChild(test);


	return TRUE;
}

enAttackStage CKnight::Attack()
{
	//play attack animation
	switch (m_enAtkStage)
	{
	case eAtkEnd:
		
		break;
	case eAtkInit:
	{
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

void CKnight::UpdateAll()
{
	CObject::UpdateObject();

	CLivingObj::UpdateLivingObj();

	CHero::UpdateHero();
}
