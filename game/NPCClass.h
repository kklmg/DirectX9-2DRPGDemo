#pragma once
#include"LivingObjectClass.h"
#include"CommonSpriteSheet.h"

class CNPC : public CLivingObj 
{
public:
	CNPC(int x, int y, DWORD width, DWORD height, CCommonSpriteSheet *image);
	virtual~CNPC();

public:
	virtual HRESULT DrawObject(LPDIRECTDRAWSURFACE7 DestSuf, DWORD DrawSetting) override;

	virtual bool Control_USER();

	virtual bool InitAI();
	
	virtual void UpdateAll() override;


protected:
	CCommonSpriteSheet *m_pSprite;//Hero UI

	virtual enAttackStage Attack();
};

