#include"UIManager.h"
#include"Global_data.h"
#include"blit.h"

CUIManager::CUIManager():m_bTalking(FALSE), m_pTextSheet(NULL)
{
}

CUIManager::~CUIManager()
{
	SAFE_RELEASE(m_pTextSheet);

	SAFE_RELEASE(m_pStateBar);

	SAFE_RELEASE(m_pHPBarLittle);

	SAFE_RELEASE(m_pDigMng);

	SAFE_RELEASE(m_pTFrameMng);
}

void CUIManager::Init()
{
	m_pTFrameMng = new CTFrameMng();
	m_pTFrameMng->Init();

	m_pStateBar = new CStateBar();
	m_pStateBar->Init();

	m_pHPBarLittle = new CHPBarLittle();
	m_pHPBarLittle->Init();

	InitTextSheet();

	m_pDigMng = new CDialogueMng();
	m_pDigMng->Init();

	InitEmoticon();
}

void CUIManager::InitTextSheet()
{
	LoadTextSheet("UI/Text03.bmp", 10, 10, RGB(255, 255, 255));
}

void CUIManager::InitEmoticon()
{
	m_vector_pEmoticon.resize(13);

	COLORREF colorkey = RGB(0, 128, 128);

	//m_vector_pEmoticon[0].SetAniSeqDesc() = new CAni_Sequence("UI/Emo00.bmp", 3, 1, 1, colorkey);
	//m_vector_pEmoticon[1] = new CAni_Sequence("UI/Emo01.bmp", 4, 1, 1, colorkey);
	//m_vector_pEmoticon[2] = new CAni_Sequence("UI/Emo02.bmp", 4, 1, 1, colorkey);
	//m_vector_pEmoticon[3] = new CAni_Sequence("UI/Emo03.bmp", 4, 1, 1, colorkey);
	//m_vector_pEmoticon[4] = new CAni_Sequence("UI/Emo04.bmp", 4, 1, 1, colorkey);
	//m_vector_pEmoticon[5] = new CAni_Sequence("UI/Emo05.bmp", 4, 1, 1, colorkey);
	//m_vector_pEmoticon[6] = new CAni_Sequence("UI/Emo06.bmp", 6, 1, 1, colorkey);
	//m_vector_pEmoticon[7] = new CAni_Sequence("UI/Emo07.bmp", 4, 1, 1, colorkey);
	//m_vector_pEmoticon[8] = new CAni_Sequence("UI/Emo08.bmp", 4, 1, 1, colorkey);
	//m_vector_pEmoticon[9] = new CAni_Sequence("UI/Emo09.bmp", 5, 1, 1, colorkey);
	//m_vector_pEmoticon[10] = new CAni_Sequence("UI/Emo10.bmp", 5, 1, 1, colorkey);
	//m_vector_pEmoticon[11] = new CAni_Sequence("UI/Emo11.bmp", 6, 1, 1, colorkey);
	//m_vector_pEmoticon[12] = new CAni_Sequence("UI/Emo12.bmp", 2, 1, 1, colorkey);
	//m_vector_pEmoticon[13] = new CAni_Sequence("UI/Emo13.bmp", 4, 1, 1, colorkey);
}

CAni_Sequence* CUIManager::CreateEmoticon(enEmoticon index)
{
	CAni_Sequence* Emoticon = NULL;

	Emoticon = new CAni_Sequence(g_pDataBase->GetEmoticonDesc(index));

	return Emoticon;
}

CTextSheet *CUIManager::GetTextSheet()
{
	return m_pTextSheet;
}

void CUIManager::LoadTextSheet(LPCSTR FileName, DWORD row, DWORD column, COLORREF key) 
{
	m_pTextSheet = new CTextSheet(FileName, row, column, key);
}

void CUIManager::DrawTest(LPDIRECTDRAWSURFACE7 dessuf, RECT desRect)
{
	m_pTextSheet->DrawTextSheet(dessuf, desRect);
}

void CUIManager::DrawStateBar() 
{
	m_pStateBar->DrawStateBar(g_pControledObj,g_pDDSBack);
}


void CUIManager::DrawHPBarLittle(CLivingObj *Obj, LPDIRECTDRAWSURFACE7 lpDDSDest, RECT DesRect)
{
	if (m_pHPBarLittle == NULL)return;
	if (Obj == NULL)return;

	m_pHPBarLittle->DrawHPBar(Obj, lpDDSDest, DesRect);
}

void CUIManager::DrawHPBarLittle(STHP &hp, LPDIRECTDRAWSURFACE7 lpDDSDest, RECT DesRect)
{
	if (m_pHPBarLittle == NULL)return;

	m_pHPBarLittle->DrawHPBar(hp, lpDDSDest, DesRect);
}

bool CUIManager::IsTalking() const 
{
	return m_bTalking;
}

void CUIManager::TriggerTalk(CDialogueChain* dialog) 
{
	if (m_bTalking == FALSE)
	{
		m_pDigMng->reset();
		m_pDigMng->LoadDialogue(dialog);
		m_bTalking = TRUE;
	}
}

WORD CUIManager::GetTextWidth(char cr)
{
	return m_pTFrameMng->GetFontWidth(cr);
}

WORD CUIManager::__DrawText(LPDIRECTDRAWSURFACE7 lpDDSDest, char cr, int left, int top, int bottom)
{
	return m_pTFrameMng->Draw(cr, lpDDSDest, left, top, bottom);
}

void CUIManager::__DrawText(LPDIRECTDRAWSURFACE7 lpDDSDest, LPCSTR str, RECT rect)
{
	m_pTFrameMng->Draw(str, lpDDSDest, rect);
}

bool CUIManager::__DrawDialogue(LPDIRECTDRAWSURFACE7 lpDDSDest, bool AB)
{
	if (!m_bTalking)return FALSE;

	else
	{
		g_bGamePause = TRUE;

		if (AB)  m_pDigMng->DrawDialogueBox_AB(lpDDSDest);
		else  m_pDigMng->DrawDialogueBox(lpDDSDest);

		if (!m_pDigMng->DrawDialogue_Dynamic(lpDDSDest))
		{
			g_bGamePause = FALSE;
			m_bTalking = FALSE;
			
			return FALSE;
		}

	}
	return TRUE;
}
