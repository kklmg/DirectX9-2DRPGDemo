#include"Global_data.h"
#include"Dialogue.h"
#include"BmpFile.h"
#include"effect.h"
#include<string>


CDialogueMng::CDialogueMng()
	:m_lpDDSDialogueBox(NULL), m_lpDDSDialogueBox_AB(NULL), m_pstSlot(NULL),
	m_bInited(FALSE), m_nCurRef(0), m_pDialogueLoad(NULL),m_enTalkStage(eTalkNone),
m_rtDestrect({ long(HRES*0.1),long(VRES*0.7),long(HRES*0.9),long(VRES*0.98) }),
m_wCharInColumn(16), m_wCharInRow(5), m_Timer(FALSE)
{	
	m_rtTextBound = GetScaledRect(m_rtDestrect, 0.9, 0.9);
}
CDialogueMng::~CDialogueMng()
{
	if (m_lpDDSDialogueBox != NULL)
	{
		m_lpDDSDialogueBox->Release();
		m_lpDDSDialogueBox = NULL;
	}

	if(m_pstSlot != NULL)
	{
		delete m_pstSlot;
		m_pstSlot = NULL;
	}
}
bool CDialogueMng::Init()
{
	m_lpDDSDialogueBox = DDLoadBitmapFast(g_pDD, "UI/DialogBox.bmp", RGB(255,255,255));
	if (!m_lpDDSDialogueBox) return FALSE;

	m_pstSlot = new STTextSlot(m_rtTextBound,4,5);
	
	LoadText("Hello World,nice to meet you,can you play with me?,i love youHello World,nice to meet you,can you play with me?,i love you Hello World,nice to meet you,can you play with me?,i love you Hello World,nice to meet you,can you play with me?,i love you");

	return TRUE;
}



HRESULT CDialogueMng ::DrawDialogueBox(LPDIRECTDRAWSURFACE7 Dest)
{
	return Dest->Blt(&m_rtDestrect, m_lpDDSDialogueBox, NULL, DDBLT_WAIT| DDBLT_KEYSRC, NULL);
}
HRESULT CDialogueMng::DrawDialogueBox_AB(LPDIRECTDRAWSURFACE7 Dest)
{
	if (m_lpDDSDialogueBox_AB == NULL) return FALSE;

	return Dest->Blt(&m_rtDestrect, m_lpDDSDialogueBox_AB, NULL, DDBLT_WAIT, NULL);
}
bool CDialogueMng::DrawDialogueEx(LPDIRECTDRAWSURFACE7 Dest)
{
	switch (m_enTalkStage)
	{
	case CDialogueMng::eTalkNone:
		return TRUE;
		break;
	case CDialogueMng::eTalkInit:
	{

		

	}
		break;
	case CDialogueMng::eTalking:
		break;
	case CDialogueMng::eTalkingEnd:
		break;
	default:
		break;
	}


}



void CDialogueMng::reset()
{
	ClearABSurf();
	m_lpDDSDialogueBox_AB = __GetAlphaBlend(m_lpDDSDialogueBox, g_pDDSBack, m_rtDestrect, 0.5);
	m_nCurRef = 0;
}
void CDialogueMng::ClearABSurf()
{
	if (m_lpDDSDialogueBox_AB != NULL)
	{
		m_lpDDSDialogueBox_AB->Release();
		m_lpDDSDialogueBox_AB = NULL;
	}
}



bool CDialogueMng::UpdateStr(float rate)
{
	if (m_strIterCur == m_strIterEnd)
	{
		if (DIKEYDOWNONCE(DIK_SPACE))
		{
			UpdateIter();
			if (m_strIterCur == m_strLoad.end())	return FALSE;
		}
		else return TRUE;
	}

	else if (DIKEYDOWNONCE(DIK_SPACE)) //skip animation
	{
		m_strIterCur = m_strIterEnd;
		return TRUE;
	}

	else if (m_Timer.LoopTimer(rate))
	{
		++m_strIterCur;
		return TRUE;
	}
}
bool CDialogueMng::UpdateDialogue(float rate)
{	
	if (m_strIterCur == m_strIterEnd)
	{
		if (DIKEYDOWNONCE(DIK_SPACE)) 
		{
			UpdateIter(); 
			
			if (m_strIterCur == m_strLoad.end()) 
			{
				++m_nCurRef;
				//Dialog ended
				if (m_nCurRef >= m_pDialogueLoad->GetSize())
				{
					m_nCurRef = 0;
					m_pDialogueLoad = NULL;
					return FALSE;
				}
				else
				{
					//m_nCurRef++;
					LoadText(m_pDialogueLoad->getDialogue(m_nCurRef)->m_strDiag.data());
					return TRUE;
				}			
			}
			
			return TRUE;
		}
		else return TRUE;
	}
	else if (DIKEYDOWNONCE(DIK_SPACE)) //skip animation
	{
		m_strIterCur = m_strIterEnd;
		return TRUE;
	}
	else if (m_Timer.LoopTimer(rate)) 
	{	
		++m_strIterCur;
		return TRUE;
	}
	else return TRUE;
}



void CDialogueMng::LoadText(LPCSTR str)
{
	m_strLoad.clear();
	m_strLoad = str;

	InitIterator();
}
void CDialogueMng::LoadDialogue(CDialogueChain* dialog)
{
	m_nCurRef = 0;
	m_pDialogueLoad = dialog;
	
	LoadText(dialog->getDialogue(m_nCurRef)->m_strDiag.data());
}

bool CDialogueMng::NextPage()
{
	++m_nCurRef;
	if (m_nCurRef > m_pDialogueLoad->GetSize())return FALSE;
	LoadText(m_pDialogueLoad->getDialogue(m_nCurRef)->m_strDiag.data());
	return TRUE;
}


void CDialogueMng::InitIterator()
{
	m_strIterBegin = m_strLoad.begin();
	m_strIterEnd = m_strLoad.begin();
	m_strIterCur = m_strLoad.begin();
	UpdateIter();
}
void CDialogueMng::UpdateIter()
{
	m_strIterBegin = m_strIterEnd;

	m_nTCount = 0;
	int i;
	int Left;

	for (i = 0; i < m_pstSlot->m_wLineCount; ++i)
	{
		Left = m_pstSlot->m_pRect[i].left;

		while (Left < m_pstSlot->m_pRect[i].right)
		{
			if (m_strIterEnd == m_strLoad.end())return;
			Left += g_pUIMng->GetTextWidth((*m_strIterEnd));
			++m_strIterEnd;
			++m_nTCount;
		}
	}
}



void CDialogueMng::DrawDialogueText(LPDIRECTDRAWSURFACE7 Dest)
{

	int i;
	int Left;
	int top;
	int bottom;

	string::iterator tempiter = m_strLoad.begin();

	for (i = 0; i < m_pstSlot->m_wLineCount;++i)
	{
		Left = m_pstSlot->m_pRect[i].left;
		top = m_pstSlot->m_pRect[i].top;
		bottom = m_pstSlot->m_pRect[i].bottom;

		while (Left < m_pstSlot->m_pRect[i].right)
		{
			if (tempiter == m_strIterCur)return;
			Left += g_pUIMng->__DrawText(Dest, (*tempiter), Left, top, bottom);
			++tempiter;
		}
	}
}
void CDialogueMng::DrawDialogueText_Dynamic(LPDIRECTDRAWSURFACE7 Dest)
{
	UpdateStr(0.02);

	//if (!m_strBuff.size())return;

	int i;
	int Left;
	int top;
	int bottom;

	string::iterator IterTemp = m_strIterBegin;


	for (i = 0; i < m_pstSlot->m_wLineCount; ++i)
	{
		Left = m_pstSlot->m_pRect[i].left;
		top = m_pstSlot->m_pRect[i].top;
		bottom = m_pstSlot->m_pRect[i].bottom;

		while (Left < m_pstSlot->m_pRect[i].right)
		{
			if (IterTemp == m_strIterCur)return;
			Left += g_pUIMng->__DrawText(Dest, (*IterTemp), Left, top, bottom);
			++IterTemp;	
		}
	}

}
bool CDialogueMng::DrawDialogue_Dynamic(LPDIRECTDRAWSURFACE7 Dest)
{
	if (!UpdateDialogue(0.02)) return FALSE;


	//if (!m_strBuff.size())return;

	int i;
	int Left;
	int top;
	int bottom;

	string::iterator IterTemp = m_strIterBegin;


	for (i = 0; i < m_pstSlot->m_wLineCount; ++i)
	{
		Left = m_pstSlot->m_pRect[i].left;
		top = m_pstSlot->m_pRect[i].top;
		bottom = m_pstSlot->m_pRect[i].bottom;

		while (Left < m_pstSlot->m_pRect[i].right)
		{
			if (IterTemp == m_strIterCur)return TRUE;
			Left += g_pUIMng->__DrawText(Dest, (*IterTemp), Left, top, bottom);
			++IterTemp;
		}
	}
	return TRUE;
}







