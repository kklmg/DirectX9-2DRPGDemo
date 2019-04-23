#pragma once
#include"HeroClass.h"
#include"KnightImage.h"

class CKnight : public CHero
{
public:
	CKnight(int x, int y, DWORD width, DWORD height, CKnightImage *image);

	virtual~CKnight();

	virtual HRESULT DrawObject(LPDIRECTDRAWSURFACE7 DestSuf, DWORD DrawSetting) override;

	virtual bool Control_USER();

	virtual bool InitAI();

	virtual void UpdateAll() override;

protected:

	CKnightImage *m_pImg;//Hero UI

	virtual enAttackStage Attack();
};

