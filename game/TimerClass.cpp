#include"TimerClass.h"
#include"Global_data.h"




CTimer::CTimer(bool GetGlobalTime)
	:m_enTmState(eTmdefault), m_bGetGlobalTime(GetGlobalTime),
	m_dStartTime(0), m_dLastTime(0), m_dEndTime(0), m_dStopTime(0)
{
	int a = 1;
}

CTimer::CTimer(enTimerState eState, bool GetGlobalTime)
	: m_enTmState(eState), m_bGetGlobalTime(GetGlobalTime),
	m_dStartTime(0), m_dLastTime(0), m_dEndTime(0), m_dStopTime(0)
{
}

CTimer::~CTimer() 
{
}

void CTimer::TimeUpdate() 
{
	if (m_bGetGlobalTime)
	{
		if (m_enTmState == eTmEnd)return;
		else if (m_enTmState == eTmStop)
		{
			m_dStopTime = __GetCurTime - m_dStartTime;
			return;
		}

		else if (m_enTmState == eTmdefault)
		{
			m_dStartTime = __GetCurTime;
			m_dStopTime = 0;
			m_enTmState = eTmGoing;
		}
		//get last time
		m_dLastTime = __GetCurTime - m_dStartTime - m_dStopTime;
	}
	else
	{
		if (m_enTmState == eTmEnd)return;
		else if (m_enTmState == eTmStop)
		{
			m_dStopTime = timeGetTime()*0.001 - m_dStartTime;
			return;
		}

		else if (m_enTmState == eTmdefault)
		{
			m_dStartTime = timeGetTime()*0.001;
			m_dStopTime = 0;
			m_enTmState = eTmGoing;
		}
		//get last time
		m_dLastTime = timeGetTime()*0.001 - m_dStartTime - m_dStopTime;
	}
}

void CTimer::ResetTimer()
{
	m_enTmState = eTmdefault;
	m_dStartTime = 0;
	m_dLastTime = 0;
	m_dEndTime = 0;
	m_dStopTime = 0;
}

void CTimer::stopTimer()
{
	m_enTmState = eTmStop;
}

bool CTimer::LoopTimer(float Rate)
{
	TimeUpdate();

	if (m_dLastTime >= Rate)
	{
		m_enTmState = eTmdefault;
		return TRUE;
	}
	else return FALSE;
}

bool CTimer::OnceTimer(float dEndTime)
{
	if (m_enTmState == eTmEnd)return TRUE;

	TimeUpdate();

	if (m_dLastTime>= dEndTime) 
	{
		m_enTmState = eTmEnd;
		return TRUE;
	}
	else return FALSE;
}

void CTimer::CloseTimer() 
{
	m_enTmState = eTmEnd;
}
