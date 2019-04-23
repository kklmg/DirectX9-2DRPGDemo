#include"KnightImage.h"
#include"ddutil.h"
#include"Global_data.h"
//---------------------------------
//Load Bitmap File 
//---------------------------------
CKnightImage::CKnightImage() 
{
}

CKnightImage::~CKnightImage() 
{
}

//load run animation	
bool CKnightImage::LoadAniRun(enDirection direction, LPCSTR bitmap, float HScale, float VScale, int Frame, COLORREF key)
{
	m_AniRun[direction].LoadBitmapFile(bitmap, Frame, VScale, HScale, key);
	return TRUE;
}

//load walk animation - using sword
bool CKnightImage::LoadAniWalk_F(enDirection direction, LPCSTR bitmap, float HScale, float VScale, int Frame, COLORREF key)
{
	m_AniWalk_F[direction].LoadBitmapFile(bitmap, Frame, VScale, HScale, key);
	return TRUE;
}

//load run animation - using sword
bool CKnightImage::LoadAniRun_F(enDirection direction, LPCSTR bitmap, float HScale, float VScale, int Frame, COLORREF key)
{
	m_AniRun_F[direction].LoadBitmapFile(bitmap, Frame, VScale, HScale, key);
	return TRUE;
}

//load stand animation - using sword
bool CKnightImage::LoadAniStand_F(enDirection direction, LPCSTR bitmap, float HScale, float VScale, int Frame, COLORREF key)
{
	m_AniStand_F[direction].LoadBitmapFile(bitmap, Frame, VScale, HScale, key);
	return TRUE;
}

//---------------------------------
//Display Animation
//---------------------------------
void CKnightImage::DisplayWalkAni_F(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf)
{
	m_AniWalk_F[direction].DisplayAnimation(pos, DesSuf, rate, TRUE,TRUE);
}

void CKnightImage::DisplayRunAni(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf)
{
	m_AniRun[direction].DisplayAnimation(pos, DesSuf, rate, true,TRUE);
}

void CKnightImage::DisplayRunAni_F(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf)
{
	m_AniRun_F[direction].DisplayAnimation(pos, DesSuf, rate, true, TRUE);
}

void CKnightImage::DisplayStandAni_F(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf) 
{
	m_AniStand_F[direction].DisplayAnimation(pos, DesSuf, rate, true, TRUE);
}

void CKnightImage::DisplayDashAni(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf) 
{
	m_AniDash[direction].DisplayAnimation(pos, DesSuf, rate, true, TRUE);
}
