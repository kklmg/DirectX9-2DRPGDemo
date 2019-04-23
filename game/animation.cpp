#include"Animation.h"
#include"RectMath.h"

CAnimation::CAnimation():
	m_enAniState(eAniDefault),
	m_dwCurFrame(0),
	m_dwMaxFrame(0),
	m_fHScale(1),
	m_fVScale(1),
	m_dwRepeatCount(0)
{
}

CAnimation::~CAnimation()
{
}

const DWORD CAnimation::GetCurFrame()
{
	return m_dwCurFrame;
}

bool CAnimation::InitAnimation()
{
	//Init Timer And Animation State
	if (m_enAniState == eAniDefault)
	{
		m_Timer.ResetTimer();
		m_enAniState = eAniGoing;
	}
	return TRUE;
}

void CAnimation::ResetAnimation()
{
	m_dwCurFrame = 0;
	m_enAniState = eAniDefault;
}

bool CAnimation::IsAnimationEnd()
{
	if (m_enAniState == enAniState::eAniEnd)
	{
		return TRUE;
	}
	else return FALSE;
}

void CAnimation::UpdateFrame(float rate,bool repeat)
{
	if (m_Timer.LoopTimer(rate))
	{
		//cur Frame > Max Frame	
		if (m_dwCurFrame >= m_dwMaxFrame - 1)
		{
			//reset 
			if (repeat == TRUE) m_dwCurFrame = 0;
			else m_enAniState = eAniEnd;
		}
		//Frame++
		else m_dwCurFrame++;
	}
}

void CAnimation::UpdateFrame(float rate, DWORD repeatcount)
{
	if (m_Timer.LoopTimer(rate))
	{
		//cur Frame > Max Frame	
		if (m_dwCurFrame >= m_dwMaxFrame - 1)
		{
			m_dwCurFrame = 0;

			m_dwRepeatCount++;

			if (m_dwRepeatCount > repeatcount)m_enAniState = eAniEnd;
		}
		//Frame++
		else m_dwCurFrame++;
	}

}

void CAnimation::SetMaxFrame(DWORD frame)
{
	m_dwMaxFrame = frame;
}

void CAnimation::SetScale(float Hscale, float Vscale)
{
	m_fHScale = Hscale;
	m_fVScale = Vscale;
}

RECT CAnimation::GetScaleRect(RECT DestRect)
{
	return GetScaledRect(DestRect,m_fHScale,m_fVScale);
}
