#include"Global_data.h"
#include"ObjectUI.h"

CEmotion::CEmotion() :m_enEmotion(eNoEmotion), m_pEmotion(NULL), m_bExistEmotion(FALSE)
{
}

CEmotion::~CEmotion() 
{
	RemoveEmotion();
}

void CEmotion::RemoveEmotion()
{
	m_enEmotion = enEmoticon::eNoEmotion;
	SAFE_RELEASE(m_pEmotion);
	m_bExistEmotion = FALSE;
}

void CEmotion::SetEmotion(enEmoticon emotion)
{
	if (m_enEmotion == emotion) return;


	if (m_pEmotion != NULL)
	{
		RemoveEmotion();
	}
	m_bExistEmotion = TRUE;

	m_enEmotion = emotion;

	m_pEmotion = g_pUIMng->CreateEmoticon(emotion);
}

bool CEmotion::DisPLayEmotion(LPDIRECTDRAWSURFACE7 suf, RECT DesRect) 
{
	if (m_pEmotion == NULL)return FALSE;

	m_pEmotion->DisplayAnimation(DesRect, suf, 0.2, TRUE, TRUE);
	return TRUE;	
}

bool CEmotion::ExistEmotion()const
{
	return m_bExistEmotion;
}




void STObjUI::Init()
{
	m_timerDrawHP.CloseTimer();
	m_timerDrawEmotion.CloseTimer();
}


void STObjUI::ActiveDisPlayHP()
{
	m_timerDrawHP.ResetTimer();
}

bool STObjUI::DrawHpBar(STHP &hp,LPDIRECTDRAWSURFACE7 suf, float lasttime)
{
	if (!m_timerDrawHP.OnceTimer(lasttime)) 
	{
		g_pUIMng->DrawHPBarLittle(hp, suf, m_rtHPBar);
		return TRUE;
	}
	else return FALSE;
}

void STObjUI::ActiveEmotion(enEmoticon emotion)
{
	m_Emotion.SetEmotion(emotion);
	m_timerDrawEmotion.ResetTimer();
}

bool STObjUI::DrawEmotion(LPDIRECTDRAWSURFACE7 suf, float lasttime)
{
	if (!m_timerDrawEmotion.OnceTimer(lasttime)) 
	{
		m_Emotion.DisPLayEmotion(suf, m_rtEmoticon);
		return TRUE;
	}
	else return FALSE;
}

