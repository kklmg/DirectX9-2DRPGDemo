#pragma once
#include"BmpFile.h"
#include"Button.h"
#include<vector>



class COverSce
{
	enum emSelect
	{
		eStart,
		eOption,
		eEditMode,
		eExit,
	};


public:
	COverSce();
	~COverSce();

	bool Init();

	HRESULT Draw(LPRECT DestRect, LPDIRECTDRAWSURFACE7 DestSuf);

private:
	void Update();

	void SelectPre();
	void SelectNext();

private:
	int m_nSlectID;
	emSelect m_emSelect;

	std::vector<CButton*>m_vecButtons;

	STDDSData* m_pDDSEX; /*DDLoadBitmapFastEX(IDirectDraw7 * pdd, LPCSTR szBitmap, bool setkey, COLORREF key);*/
};

