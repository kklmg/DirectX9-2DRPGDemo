#pragma once
#include"CommonSpriteRow.h"
#include"SequenceAni.h"
#include"EnumGroup.h"

class CKnightImage : public CCommonSpriteRow
{
public:
	CKnightImage();

	virtual ~CKnightImage();

	//load run animation	
	bool LoadAniRun(enDirection direction, LPCSTR bitmap, float HScale, float VScale, int Frame, COLORREF key);

	//load walk animation - using sword
	bool LoadAniWalk_F(enDirection direction, LPCSTR bitmap, float HScale, float VScale, int Frame, COLORREF key);
	//load run animation - using sword
	bool LoadAniRun_F(enDirection direction, LPCSTR bitmap, float HScale, float VScale, int Frame, COLORREF key);
	//load stand animation - using sword
	bool LoadAniStand_F(enDirection direction, LPCSTR bitmap, float HScale, float VScale, int Frame, COLORREF key);

	//load other animation
	bool LoadAniSkill(LPCSTR left, LPCSTR right, LPCSTR up, LPCSTR down, int Frame, COLORREF key);

	bool LoadAnibeAttack(LPCSTR left, LPCSTR right, LPCSTR up, LPCSTR down, int Frame, COLORREF key);

	//Display Animation
	void DisplayWalkAni_F(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf);

	void DisplayRunAni(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf);

	void DisplayRunAni_F(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf);

	void DisplayStandAni_F(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf);

	void DisplayDashAni(enDirection direction, float rate, RECT pos, LPDIRECTDRAWSURFACE7 DesSuf);

protected:

	//Character animation_________________________
	//normal state______
	CAni_Sequence m_AniRun[4];

	//fight state_______
	CAni_Sequence m_AniWalk_F[4];
	CAni_Sequence m_AniRun_F[4];
	CAni_Sequence m_AniStand_F[4];

	//Skill ani
	CAni_Sequence m_AniDash[4];


};