#pragma once
#include"LivingObjectClass.h"
#include"CommonSpriteRow.h"

class CMonster : public CLivingObj
{
public:
	CMonster(int x, int y, DWORD width, DWORD height, CCommonSpriteRow *image);
	virtual~CMonster();

public:
	virtual HRESULT DrawObject(LPDIRECTDRAWSURFACE7 DestSuf, DWORD DrawSetting) override;

	virtual bool Control_USER();

	virtual bool InitAI();

	virtual void UpdateAll() override;

protected:
	CCommonSpriteRow *m_pSprite;

	virtual enAttackStage Attack();
};

