#pragma once
#include"HeroClass.h"
#include"ArcherImage.h"

class CArcher : public CHero
{
public:
	CArcher(int x, int y, DWORD width, DWORD height, CArcherImage *image);

	virtual~CArcher();

	virtual HRESULT DrawObject(LPDIRECTDRAWSURFACE7 DestSuf, DWORD DrawSetting) override;

	virtual bool Control_USER();//user control

	virtual bool InitAI();

	virtual void UpdateAll() override;

protected:
	CArcherImage *m_pImg;//Hero UI

	virtual enAttackStage Attack();
};

