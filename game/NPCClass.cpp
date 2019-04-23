#include"NPCClass.h"
#include"InputClass.h"
#include"ObjectMng.h"
#include"Damage.h"
#include"Global_data.h"

CNPC::CNPC(int x, int y, DWORD width, DWORD height, CCommonSpriteSheet *image)
{
	m_enObjType = enObjectType::eOTNpc;

	CObject::Init(x, y, width, height);
	CAttribute::Init(&g_pAttMng->GetATTTable(enLOBJ::eCommon), 1);

	m_pSprite = image;

	g_pAIMng->Add_CommonAI_NPC(this);

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


	//Update();
}

CNPC::~CNPC()
{
}

//void CNPC::UpdateAll()
//{
//	CObject::UpdateObject();
//	CLivingObj::UpdateLivingObj();
//
//}

HRESULT CNPC::DrawObject(LPDIRECTDRAWSURFACE7 suf, DWORD DrawSetting)
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
		case eLeft: m_pSprite->DisplayStandAni(eLeft, 0.2, m_rectDraw, suf); break;
		case eRight: m_pSprite->DisplayStandAni(eRight, 0.2, m_rectDraw, suf); break;
		case eUp: m_pSprite->DisplayStandAni(eUp, 0.2, m_rectDraw, suf); break;
		case eDown: m_pSprite->DisplayStandAni(eDown, 0.2, m_rectDraw, suf); break;
		}
	}break;

	case eWalk:
	eRun:
	{
		switch (CurDir)//character direction
		{
		case eLeft: m_pSprite->DisplayWalkAni(eLeft, 0.08, m_rectDraw, suf); break;
		case eRight: m_pSprite->DisplayWalkAni(eRight, 0.08, m_rectDraw, suf); break;
		case eUp: m_pSprite->DisplayWalkAni(eUp, 0.08, m_rectDraw, suf); break;
		case eDown: m_pSprite->DisplayWalkAni(eDown, 0.08, m_rectDraw, suf); break;
		}
	}break;
	}

	CLivingObj::DrawUI(suf);
	return TRUE;
}

bool CNPC::Control_USER()
{
	if (m_bIsAlive == FALSE) return FALSE;
	if (IsActionLocked()) return FALSE;
	enAction CurAction = GetCurAction();


	//Run________________________________________________________________________________
	if (DIKEYDOWN(DIK_LSHIFT))
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

bool CNPC::InitAI()
{
	return TRUE;
}

enAttackStage CNPC::Attack()
{
	return eAtkEnd;
}