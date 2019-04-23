#include"Button.h"
#include"Global_data.h"

CButton::CButton(LPCSTR pathA, LPCSTR pathB, COLORREF*colorkey, void(*Action)(void), LPCSTR ButtonName)
	:m_lpDDS_Select(nullptr),
	m_lpDDS_Default(nullptr),
	m_bSelect(FALSE),
	m_pfDoSomeThing(Action),
	m_strName(ButtonName),
	m_DrawRect({0,0,0,0})
{
	if (colorkey)
	{
		m_lpDDS_Default = DDLoadBitmapFastEX(g_pDD, pathA, TRUE, *colorkey);
		m_lpDDS_Select = DDLoadBitmapFastEX(g_pDD, pathB, TRUE, *colorkey);
	}
	else 
	{
		m_lpDDS_Default = DDLoadBitmapFastEX(g_pDD, pathA, FALSE, 0);
		m_lpDDS_Select = DDLoadBitmapFastEX(g_pDD, pathB, FALSE, 0);
	}
}

CButton::~CButton()
{
	if (m_lpDDS_Select != NULL) 
	{
		delete m_lpDDS_Select;
		m_lpDDS_Select = NULL;	
	}
	if (m_lpDDS_Default != NULL)
	{
		delete m_lpDDS_Default;
		m_lpDDS_Default = NULL;
	}
}

void CButton::SetDrawRect(RECT rect) 
{
	m_DrawRect = rect;
}

HRESULT CButton::Draw(LPDIRECTDRAWSURFACE7 DestSuf, LPRECT rect)
{
	if (m_bSelect) return m_lpDDS_Select->DrawNormal(rect, DestSuf);
	else return m_lpDDS_Default->DrawNormal(rect, DestSuf);
}
HRESULT CButton::Draw(LPDIRECTDRAWSURFACE7 DestSuf)
{
	if (m_bSelect) return m_lpDDS_Select->DrawNormal(&m_DrawRect, DestSuf);
	else return m_lpDDS_Default->DrawNormal(&m_DrawRect, DestSuf);
}



void CButton::SetSelect(bool State)
{
	m_bSelect = State;
}

void CButton::TriggerClickEvent()
{
	if (!m_pfDoSomeThing)return;
	m_pfDoSomeThing();
}