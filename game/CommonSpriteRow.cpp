#include"CommonSpriteRow.h"
#include"ddutil.h"
#include"Global_data.h"
//---------------------------------
//Load Bitmap File 
//---------------------------------
CCommonSpriteRow::CCommonSpriteRow()
{
}

CCommonSpriteRow::~CCommonSpriteRow()
{
}

//load portrait
bool CCommonSpriteRow::LoadPortrait(LPCSTR PortraitFile, COLORREF key)
{
	m_pSufPortrait = DDLoadBitmap(g_pDD, PortraitFile, 0, 0);
	DDSetColorKey(m_pSufPortrait, key);
	return TRUE;
}
//Load Stand animation
bool CCommonSpriteRow::LoadAniStand(enDirection direction, LPCSTR bitmap, float HScale, float VScale, int Frame, COLORREF key)
{
	m_AniStand[direction].LoadBitmapFile(bitmap, Frame, VScale, HScale, key);
	return TRUE;
}

//load walk animation
bool CCommonSpriteRow::LoadAniWalk(enDirection direction, LPCSTR bitmap, float HScale, float VScale, int Frame, COLORREF key)
{
	m_AniWalk[direction].LoadBitmapFile(bitmap, Frame, VScale, HScale, key);
	return TRUE;
}

//load Normal attack animation
bool CCommonSpriteRow::LoadAniNormalAttack(enDirection direction, LPCSTR bitmap, float HScale, float VScale, int Frame, COLORREF key)
{
	m_AniNormalAttack[direction].LoadBitmapFile(bitmap, Frame, VScale, HScale, key);
	return TRUE;
}

//---------------------------------
//Display Animation
//---------------------------------

void CCommonSpriteRow::DisplayWalkAni(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf)
{
	m_AniWalk[direction].DisplayAnimation(pos,DesSuf,rate,TRUE, TRUE);
}

void CCommonSpriteRow::DisplayStandAni(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf)
{
	m_AniStand[direction].DisplayAnimation(pos,DesSuf,rate, TRUE, TRUE);
}

void CCommonSpriteRow::DisplayStandAni(enDirection direction,RECT pos, LPDIRECTDRAWSURFACE7 DesSuf)
{
	m_AniWalk[direction].DisplayOneFrame(pos, DesSuf, 0, TRUE);
}

void CCommonSpriteRow::DisplayNormalAttackAni(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf)
{
	m_AniNormalAttack[direction].DisplayAnimation(pos, DesSuf, rate, FALSE, TRUE);

	//if(m_AniNormalAttack[direction].IsAnimationEnd())
	//m_AniNormalAttack[direction].ResetAnimation();
}

//---------------------------
//visit data
//---------------------------

const DWORD CCommonSpriteRow::GetAttackAniCurFrame(enDirection direction)
{
	return m_AniNormalAttack[direction].GetCurFrame();
}

bool CCommonSpriteRow::IsAttackAnimationEnd(enDirection direction)
{

	if (m_AniNormalAttack[direction].IsAnimationEnd())return TRUE;
	
	return FALSE;
}

void CCommonSpriteRow::ResetAttackAnimation()
{
	for (int x = 0; x < 4; x++)
	{
		m_AniNormalAttack[x].ResetAnimation();
	}
}