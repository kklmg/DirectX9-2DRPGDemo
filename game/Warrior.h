#pragma once
#include"HeroClass.h"
#include"CommonSpriteSheet.h"

class CWarrior : public CHero
{
public:
	CWarrior(int x, int y, DWORD width, DWORD height, CCommonSpriteSheet *image);

	virtual~CWarrior();

	virtual HRESULT DrawObject(LPDIRECTDRAWSURFACE7 DestSuf, DWORD DrawSetting) override;

	virtual bool Control_USER();//user control

	virtual bool InitAI();

	virtual void UpdateAll() override;

protected:

	CCommonSpriteSheet *m_pImg;//Hero UI

	virtual enAttackStage Attack();
};

