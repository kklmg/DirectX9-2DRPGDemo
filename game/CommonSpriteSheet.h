#pragma once
#include"SheetAni.h"
#include"EnumGroup.h"

class CCommonSpriteSheet
{
public:
	CCommonSpriteSheet();

	virtual ~CCommonSpriteSheet();

	void UpdateDirection(enDirection direction);

	//load portrait
	bool LoadPortrait(LPCSTR portrait, COLORREF key);

	//load walk animation
	bool LoadAniWalk(LPCSTR bitmap, DWORD row, DWORD column, float Vscale, float Hscale, COLORREF key);

	//load attack animation
	bool LoadAniNormalAttack(LPCSTR bitmap, DWORD row, DWORD column, float Vscale, float Hscale, COLORREF key);

	//Display Animation
	void DisplayWalkAni(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf);

	void DisplayStandAni(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf);

	void DisplayNormalAttackAni(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf);

	const DWORD GetAttackAniCurFrame(enDirection direction);

	bool IsAttackAnimationEnd(enDirection direction);

	void ResetAttackAnimation();

protected:

	LPDIRECTDRAWSURFACE7 m_pSufPortrait;

	//Character animation_________________________
	CAni_Sheet m_AniWalk;

	CAni_Sheet m_AniNormalAttack;
};