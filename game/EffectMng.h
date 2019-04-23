#pragma once
#include<ddraw.h>
#include"effect.h"


struct STRate 
{
	DWORD StartRate = 0;
	int CurRate = 0;
	DWORD EndRate = 0;
};


class CEffectMng
{
public:
	enum enTriggerState
	{
		eDefault,
		eRunning,
		eEnd
	};
public:
	CEffectMng();

	~CEffectMng();

	bool Init();

	void LookUpTableInit();

	void SceneEffectUpdate();

	bool IsFadeInEnd();

	bool IsFadeOutEnd();

	bool IsFadeLoopEnd();

	void PlayFadeOut();

	void PlayFadeIn();

	void PlayFadeLoop();

private:
	///////////////
	//Fade In Out//
	///////////////

	//Fade In
	enTriggerState m_enTrgFadeIn;
	STRate m_rateFadeIn;

	bool TriFadeIn(LPDIRECTDRAWSURFACE7 lpDDSDst, DWORD startRate, DWORD EndRate, DWORD Speed);


	//Fade Out
	enTriggerState m_enTrgFadeOut;
	STRate m_rateFadeOut;

	bool TriFadeOut(LPDIRECTDRAWSURFACE7 lpDDSDst,DWORD startRate, DWORD EndRate, DWORD Speed);

	
	//Fade Loop
	enTriggerState m_enTrgFadeLoop;
	STRate m_rateFadeLoop;

	bool TriFadeLoop(LPDIRECTDRAWSURFACE7 lpDDSDst, DWORD startRate, DWORD EndRate, DWORD Speed);


	//Fade To
	enTriggerState m_enTrgFadeTo;


	//Fade In Out Function
	void FadeInOutEx(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc, DWORD dwRate);

	void FadeInOutEx_Self(LPDIRECTDRAWSURFACE7 lpDDSSrc, DWORD dwRate);
	//Fade Look Up Table
	DWORD m_dwFADELUT_16[64][101];

	DWORD m_dwFADELUT_32[255][101];

	//Scale
	bool GrayScale();

	bool ColorScale();

	bool GrayScaleSelf();

	////////////////////////////
	//Create Fast Temp Buffer//
	///////////////////////////

	LPDIRECTDRAWSURFACE7 m_pDDSVBuffer;
	BOOL CreateFastVBuffer(DWORD dwWidth, DWORD dwHeight, COLORREF rgb);


	LPDIRECTDRAWSURFACE7 m_pDDSVBuffer2;
	BOOL CreateFastVBuffer2(DWORD dwWidth, DWORD dwHeight, COLORREF rgb);


	LPDIRECTDRAWSURFACE7 m_pDDSSBuffer;
	BOOL CreateFastSBuffer(DWORD dwWidth, DWORD dwHeight, COLORREF rgb);


	LPDIRECTDRAWSURFACE7 m_pDDSSBuffer2;
	BOOL CreateFastSBuffer2(DWORD dwWidth, DWORD dwHeight, COLORREF rgb);

};
