#include"ArcherImage.h"
#include"ddutil.h"
#include"Global_data.h"
//---------------------------------
//Load Bitmap File 
//---------------------------------
CArcherImage::CArcherImage()
{
	m_pAniBowAttack = NULL;

	m_pAnibeAttack = NULL;
}

CArcherImage::~CArcherImage()
{
	SAFE_RELEASE(m_pAniBowAttack);
	SAFE_RELEASE(m_pAnibeAttack)
}

//load attack animation
bool CArcherImage::LoadAniBowAttack(LPCSTR bitmap, DWORD row, DWORD column, float Vscale, float Hscale, COLORREF key)
{

	m_pAniBowAttack = new CAni_Sheet();
	m_pAniBowAttack->LoadAniSheet(bitmap, row, column, Vscale, Hscale, key);
	return TRUE;
}

//---------------------------------
//Display Animation
//---------------------------------
void CArcherImage::DisplayBowAttackAni(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf)
{
	m_pAniBowAttack->DisplayAnimation(pos, DesSuf,direction, rate, true, TRUE);
}

//---------------------------
//get current attack frame ...used in trigger attack event  
//---------------------------

const DWORD CArcherImage::GetBowAttackAniCurFrame(enDirection direction)
{
	return m_pAniBowAttack->GetCurFrame(direction);
}

bool CArcherImage::IsBowAttackAniEnd(enDirection direction)
{
	return m_pAniBowAttack->IsAnimaitionEnd(direction);
}