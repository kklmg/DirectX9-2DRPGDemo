#include"CommonSpriteSheet.h"
#include"ddutil.h"
#include"Global_data.h"

//---------------------------------
//Load Bitmap File 
//---------------------------------
CCommonSpriteSheet::CCommonSpriteSheet():m_pSufPortrait(NULL)
{
}

CCommonSpriteSheet::~CCommonSpriteSheet()
{
}

//load portrait
bool CCommonSpriteSheet::LoadPortrait(LPCSTR PortraitFile, COLORREF key)
{
	m_pSufPortrait = DDLoadBitmap(g_pDD, PortraitFile, 0, 0);
	DDSetColorKey(m_pSufPortrait, key);
	return TRUE;
}

//load walk animation
bool CCommonSpriteSheet::LoadAniWalk(LPCSTR bitmap, DWORD row, DWORD column, float Vscale, float Hscale, COLORREF key)
{
	m_AniWalk.LoadAniSheet(bitmap, row, column, Vscale, Hscale, key);
	return TRUE;
}

//load attack animation
bool CCommonSpriteSheet::LoadAniNormalAttack(LPCSTR bitmap, DWORD row, DWORD column, float Vscale, float Hscale, COLORREF key)
{
	m_AniNormalAttack.LoadAniSheet(bitmap, row, column, Vscale, Hscale, key);
	return TRUE;
}

void  CCommonSpriteSheet::UpdateDirection(enDirection direction)
{

}


//---------------------------------
//Display Animation
//---------------------------------
void CCommonSpriteSheet::DisplayStandAni(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf)
{
	m_AniWalk.DisplayOneFrame(pos, DesSuf, direction,0, TRUE);
}

void CCommonSpriteSheet::DisplayWalkAni(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf)
{
	m_AniWalk.DisplayAnimation(pos, DesSuf,direction, rate, true, TRUE);
}

void CCommonSpriteSheet::DisplayNormalAttackAni(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf)
{
	m_AniNormalAttack.DisplayAnimation(pos, DesSuf, direction, rate, FALSE, TRUE);
}

//---------------------------
//get current attack frame ...used in trigger attack event  
//---------------------------

const DWORD CCommonSpriteSheet::GetAttackAniCurFrame(enDirection direction)
{
	return m_AniNormalAttack.GetCurFrame(direction);
}

bool CCommonSpriteSheet::IsAttackAnimationEnd(enDirection direction)
{
	return m_AniNormalAttack.IsAnimaitionEnd(direction);
}

void CCommonSpriteSheet::ResetAttackAnimation()
{
	m_AniNormalAttack.ResetALLAnimation();
}