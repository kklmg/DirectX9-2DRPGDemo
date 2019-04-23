#pragma once
#include"SequenceAni.h"
#include"EnumGroup.h"

class CCommonSpriteRow
{
public:
	CCommonSpriteRow();

	~CCommonSpriteRow();

	//load portrait
	bool LoadPortrait(LPCSTR portrait, COLORREF key);

	//load walk animation
	bool LoadAniWalk(enDirection direction, LPCSTR bitmap, float HScale, float VScale, int Frame, COLORREF key);

	//Load Idle animation
	bool LoadAniStand(enDirection direction, LPCSTR bitmap, float HScale, float VScale, int Frame, COLORREF key);

	//load attack animation
	bool LoadAniNormalAttack(enDirection direction, LPCSTR bitmap, float HScale, float VScale, int Frame, COLORREF key);

	//Display Animation
	void DisplayWalkAni(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf);

	void DisplayStandAni(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf);
	void DisplayStandAni(enDirection direction, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf);

	void DisplayNormalAttackAni(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf);

	const DWORD GetAttackAniCurFrame(enDirection direction);

	bool IsAttackAnimationEnd(enDirection direction);

	void ResetAttackAnimation();

protected:

	LPDIRECTDRAWSURFACE7 m_pSufPortrait;

	//Character animation
	CAni_Sequence m_AniWalk[4];

	CAni_Sequence m_AniStand[4];

	CAni_Sequence m_AniNormalAttack[4];
	//be attack ani
	CAni_Sequence m_AnibeAttack[4];

};