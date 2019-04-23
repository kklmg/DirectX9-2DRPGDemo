#pragma once
#include<windows.h>
#include<ddraw.h>
#include"LivingObjectClass.h"
#include"RectMath.h"
#include"Attribute.h"

class CStateBar
{	
public:
	CStateBar();
	~CStateBar();

	void Init();

	void InitFrameWork();

	void InitHPBar();

	void InitMPBar();

	void SetScale(float Hscale,float Vscale);

	void SetDrawRect(RECT rect);

	LPDIRECTDRAWSURFACE7 m_lpDDSFrameWork;

	LPDIRECTDRAWSURFACE7 m_lpDDSHPBar;

	LPDIRECTDRAWSURFACE7 m_lpDDSMPBar;


	CRECTNode *m_pRectGroup;

	RECT m_DrawRect;

	void DrawStateBar(CLivingObj *Obj, LPDIRECTDRAWSURFACE7 lpDDSDest);

private:

	/////////////
	//State Bar//
	/////////////

	RECT m_rectDraw;

	RECT m_rectHPBar;
	DWORD m_dwHPBarWidth;

	RECT m_rectMPBar;
	DWORD m_dwMPBarWidth;
};

class CHPBarLittle 
{
public:
	CHPBarLittle();
	~CHPBarLittle();

	void Init();

	void InitBarFrameWork();

	void InitHPBar();

	void UpdateDrawRect(RECT rect);

	void DrawHPBar(CLivingObj *Obj, LPDIRECTDRAWSURFACE7 lpDDSDest, RECT DesRect);

	void DrawHPBar(STHP &HP, LPDIRECTDRAWSURFACE7 lpDDSDest, RECT DesRect);

	//Frame Work Rect Node
	CRECTNode *m_pRTnode_FW;

	//////////////////
	//HP Bar Little//
	/////////////////

	DWORD m_dwHPBarWidth;

	RECT m_rectHPBar;

	LPDIRECTDRAWSURFACE7 m_lpDDSHPBar;

	LPDIRECTDRAWSURFACE7 m_lpDDSHPBarFW;
};

