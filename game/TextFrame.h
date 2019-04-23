#pragma once
#include<ddraw.h>
#include"BmpFile.h"
#include<map>

enum enAlign
{
	eAlign_Middle,
	eAlign_top,
	eAlign_Bottom
};

RECT GetDestRect(enAlign align, int left, int top, int bottom, WORD Width, WORD Height);

struct STTextFrame
{
	STTextFrame(LPCSTR szBitmap,COLORREF key, enAlign align = eAlign_Bottom);
	~STTextFrame();

	void SetScale(float x,float y);

	void __DrawText(LPDIRECTDRAWSURFACE7 Dest, int left, int top, int bottom);

	bool m_bInitRect;
	RECT m_rtDrawRect;

	
	WORD m_wWidth;
	WORD m_wHeight;

	float m_fScale_x;
	float m_fScale_Y;

	STDDSData *m_pSuf;
	enAlign m_enAlign;
};

class CTFrameMng
{
public:
	CTFrameMng();
	~CTFrameMng();
	void Init();

	WORD Draw(char cr,LPDIRECTDRAWSURFACE7 Dest,int left, int top, int bottom);
	bool Draw(LPCSTR str, LPDIRECTDRAWSURFACE7 Dest, RECT rect);

	WORD GetFontWidth(char cr);

private:
	void InsertText(char Cr, LPCSTR szBitmap,enAlign align= eAlign_Bottom);
	void InsertText(char Cr, char crBitmap, enAlign align = eAlign_Bottom);


	std::map<char, STTextFrame*> m_mapTextData;
	std::map<char, STTextFrame*>::iterator m_mapiter;
};