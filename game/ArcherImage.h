#pragma once
#include"CommonSpriteSheet.h"

class CArcherImage : public CCommonSpriteSheet
{
public:
	CArcherImage();
	virtual ~CArcherImage();
	
	bool LoadAniBowAttack(LPCSTR bitmap, DWORD row, DWORD column, float Vscale, float Hscale, COLORREF key);

	bool LoadAnibeAttack(LPCSTR bitmap, DWORD row, DWORD column, float Vscale, float Hscale, COLORREF key);

	void DisplayBowAttackAni(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf);

	bool IsBowAttackAniEnd(enDirection direction);

	const DWORD GetBowAttackAniCurFrame(enDirection direction);

protected:
	//Archer animation_________________________
	CAni_Sheet* m_pAniBowAttack;

	CAni_Sheet* m_pAnibeAttack;
};