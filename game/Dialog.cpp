#include"Global_data.h"
#include"Dialog.h"
#include"BmpFile.h"
#include"effect.h"
#include<string>


CDialogMng::CDialogMng()
	:m_lpDDSDialogBox(NULL), m_lpDDSDialogBox_AB(NULL), m_pstSlot(NULL), 
	m_bInited(FALSE), m_nCurRef(0), m_pDialogLoad(NULL),m_enTalkStage(eTalkNone),
m_rtDestrect({ long(HRES*0.1),long(VRES*0.7),long(HRES*0.9),long(VRES*0.98) }),
m_wCharInColumn(16), m_wCharInRow(5), m_Timer(FALSE)
{	
	m_rtTextBound = GetScaledRect(m_rtDestrect, 0.9, 0.9);
}
CDialogMng::~CDialogMng() 
{
	if (m_lpDDSDialogBox != NULL) 
	{
		m_lpDDSDialogBox->Release();
		m_lpDDSDialogBox = NULL;	
	}

	if(m_pstSlot != NULL)
	{
		delete m_pstSlot;
		m_pstSlot = NULL;
	}
}
bool CDialogMng::Init()
{
	m_lpDDSDialogBox = DDLoadBitmapFast(g_pDD, "UI/DialogBox.bmp", RGB(255,255,255));
	if (!m_lpDDSDialogBox) return FALSE;

	m_pstSlot = new STTextSlot(m_rtTextBound,4,5);
	
	LoadText("Hello World,nice to meet you,can you play with me?,i love youHello World,nice to meet you,can you play with me?,i love you Hello World,nice to meet you,can you play with me?,i love you Hello World,nice to meet you,can you play with me?,i love you");

	return TRUE;
}



HRESULT CDialogMng ::DrawDialogBox(LPDIRECTDRAWSURFACE7 Dest)
{
	return Dest->Blt(&m_rtDestrect, m_lpDDSDialogBox, NULL, DDBLT_WAIT| DDBLT_KEYSRC, NULL);
}
HRESULT CDialogMng::DrawDialogBox_AB(LPDIRECTDRAWSURFACE7 Dest)
{
	if (m_lpDDSDialogBox_AB == NULL) return FALSE;

	return Dest->Blt(&m_rtDestrect, m_lpDDSDialogBox_AB, NULL, DDBLT_WAIT, NULL);
}
bool CDialogMng::DrawDialogEx(LPDIRECTDRAWSURFACE7 Dest)
{
	switch (m_enTalkStage)
	{
	case CDialogMng::eTalkNone:
		return TRUE;
		break;
	case CDialogMng::eTalkInit:
	{

		

	}
		break;
	case CDialogMng::eTalking:
		break;
	case CDialogMng::eTalkingEnd:
		break;
	default:
		break;
	}


}



void CDialogMng::reset()
{
	ClearABSurf();
	m_lpDDSDialogBox_AB = __GetAlphaBlend(m_lpDDSDialogBox, g_pDDSBack, m_rtDestrect, 0.5);
	m_nCurRef = 0;
}
void CDialogMng::ClearABSurf()
{
	if (m_lpDDSDialogBox_AB != NULL)
	{
		m_lpDDSDialogBox_AB->Release();
		m_lpDDSDialogBox_AB = NULL;
	}
}



bool CDialogMng::UpdateStr(float rate)
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
bool CDialogMng::UpdateDialog(float rate) 
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
				if (m_nCurRef >= m_pDialogLoad->GetSize()) 
				{
					m_nCurRef = 0;
					m_pDialogLoad = NULL;
					return FALSE;
				}
				else
				{
					//m_nCurRef++;
					LoadText(m_pDialogLoad->getDialog(m_nCurRef)->m_strDiag.data());
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



void CDialogMng::LoadText(LPCSTR str)
{
	m_strLoad.clear();
	m_strLoad = str;

	InitIterator();
}
void CDialogMng::LoadDialog(CDialogChain* dialog) 
{
	m_nCurRef = 0;
	m_pDialogLoad = dialog;
	
	LoadText(dialog->getDialog(m_nCurRef)->m_strDiag.data());
}

bool CDialogMng::NextPage()
{
	++m_nCurRef;
	if (m_nCurRef > m_pDialogLoad->GetSize())return FALSE;
	LoadText(m_pDialogLoad->getDialog(m_nCurRef)->m_strDiag.data());
	return TRUE;
}


void CDialogMng::InitIterator() 
{
	m_strIterBegin = m_strLoad.begin();
	m_strIterEnd = m_strLoad.begin();
	m_strIterCur = m_strLoad.begin();
	UpdateIter();
}
void CDialogMng::UpdateIter()
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



void CDialogMng::DrawDialogText(LPDIRECTDRAWSURFACE7 Dest)
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
void CDialogMng::DrawDialogText_Dynamic(LPDIRECTDRAWSURFACE7 Dest)
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
bool CDialogMng::DrawDialog_Dynamic(LPDIRECTDRAWSURFACE7 Dest)
{
	if (!UpdateDialog(0.02)) return FALSE;


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







