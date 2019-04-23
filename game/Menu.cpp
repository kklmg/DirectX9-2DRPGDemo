#include"Menu.h"
#include"Global_data.h"
#include"RectMath.h"
#include"ChangeFun.h"


CMenu::CMenu()
	:m_pDDSEX(nullptr),
	m_emSelect(emSelect::eStart),
	m_nSlectID(0),
	m_vecButtons(2, nullptr)
{
}

CMenu::~CMenu()
{
	for (auto iter : m_vecButtons)
	{		
		if (iter != NULL) 
		{
			delete iter;
			iter = NULL;
		}
	}

	if (m_pDDSEX != NULL) 
	{
		delete m_pDDSEX;
		m_pDDSEX = NULL;
	}
}

bool CMenu::Init()
{
	m_pDDSEX = DDLoadBitmapFastEX(g_pDD, "UI/Menu.bmp", FALSE, 0);
	if (!m_pDDSEX)return FALSE;

	COLORREF key = RGB(255, 255, 255);
	m_vecButtons[0] = new CButton("UI/btStart.bmp", "UI/btStart_p.bmp", &key, __LoadInGame, "START");
	m_vecButtons[1] = new CButton("UI/btExit.bmp", "UI/btExit_p.bmp", &key, __QuitGame, "EXIT");

	RECT temp = CreateRect({ (long)(HRES*0.15),(long)(VRES*0.9)}, (long)HRES*0.2, (long)VRES*0.15);
	m_vecButtons[0]->SetDrawRect(temp);


	temp = CreateRect({ (long)(HRES*0.4),(long)(VRES*0.9)}, (long)HRES*0.2, (long)VRES*0.15);
	m_vecButtons[1]->SetDrawRect(temp);
	m_vecButtons[m_nSlectID]->SetSelect(TRUE);

	return TRUE;
}

void CMenu::Update()
{
	if (DIKEYDOWNONCE(DIK_LEFT))
	{
		SelectPre();
	}
	if (DIKEYDOWNONCE(DIK_RIGHT))
	{
		SelectNext();
	}
	if (DIKEYDOWNONCE(DIK_RETURN))
	{
		m_vecButtons[m_nSlectID]->TriggerClickEvent();
	}
}

void CMenu::SelectPre()
{
	m_vecButtons[m_nSlectID--]->SetSelect(FALSE);
	if (m_nSlectID < 0)m_nSlectID = m_vecButtons.size() - 1;
	m_vecButtons[m_nSlectID]->SetSelect(TRUE);
}

void CMenu::SelectNext()
{
	m_vecButtons[m_nSlectID++]->SetSelect(FALSE);
	if (m_nSlectID > m_vecButtons.size()-1)m_nSlectID = 0;
	m_vecButtons[m_nSlectID]->SetSelect(TRUE);
}

HRESULT CMenu::DrawMenu(LPRECT DestRect, LPDIRECTDRAWSURFACE7 DestSuf)
{
	HRESULT hrlt;
	hrlt = m_pDDSEX->DrawNormal(DestRect, DestSuf);
	if (hrlt != DD_OK)return hrlt;

	for (auto iter : m_vecButtons)
	{
		hrlt=iter->Draw(DestSuf);
		if (hrlt != DD_OK)return hrlt;
	}

	Update();
}