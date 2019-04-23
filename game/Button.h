#pragma once
#include"BmpFile.h"
#include<string>

class CButton
{
public:
	CButton(LPCSTR PathA,LPCSTR pathB, COLORREF*colorkey=nullptr, void(*Action)(void) = nullptr,LPCSTR ButtonName="UnKnown");
	~CButton();

	HRESULT Draw(LPDIRECTDRAWSURFACE7 DestSuf, LPRECT rect);
	HRESULT Draw(LPDIRECTDRAWSURFACE7 DestSuf);

	void SetSelect(bool State);

	void SetDrawRect(RECT rect);

	void TriggerClickEvent();

private:
	std::string m_strName;

	bool m_bSelect;

	void(*m_pfDoSomeThing)(void);

	RECT m_DrawRect;

	STDDSData* m_lpDDS_Select;
	STDDSData* m_lpDDS_Default;
};

