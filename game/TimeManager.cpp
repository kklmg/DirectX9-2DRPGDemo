#include"TimeManager.h"

CTimeMng::CTimeMng() :m_dwBeginTime(0), m_dwLastTime(0), m_dwCurTime(0),
m_dwPreTime(0), m_dwStopLastTime(0), m_dwStopBeginTime(0), m_nLoopCount(0), m_bIsNewLoop(FALSE),m_bTimeStop(FALSE)
{
}

CTimeMng::~CTimeMng()
{

}

void CTimeMng::Init()
{
	m_dwBeginTime = timeGetTime();

	m_dwCurTime = m_dwBeginTime;

	m_dwPreTime = m_dwBeginTime;
}


void CTimeMng::TimeUpdate()
{
	//save Pre Time
	m_dwPreTime = m_dwCurTime;

	//Get Cur Time
	m_dwCurTime = timeGetTime();

	//Get One Frame Delta Time;
	m_dwOneFrameDeltaTime = m_dwCurTime - m_dwPreTime;

	//Get One sec Delta time
	//m_dOneSecDeltaTime = 1.0 / ((double)m_dwOneFrameDeltaTime*0.001);

	//if time over 47.71
	if (m_dwCurTime < m_dwBeginTime)
	{
		m_bIsNewLoop = TRUE;
		m_nLoopCount++;

		m_dwLastTime = m_dwCurTime - m_dwBeginTime + (~m_dwBeginTime) + 1;

		m_dwBeginTime = m_dwCurTime;
	}

	//last time
	m_dwLastTime = m_dwCurTime - m_dwBeginTime;

	if (m_bTimeStop)
	{
		m_dwStopLastTime = (m_dwCurTime - m_dwStopBeginTime);
	}
}

double CTimeMng::GetDeltaTime()const
{
	return (double)m_dwOneFrameDeltaTime*0.001;
}

double CTimeMng::GetCurTime()const
{
	return (m_dwCurTime - m_dwStopLastTime)*0.001;
}

double CTimeMng::GetCurTime_s()const
{
	return m_dwCurTime*0.001;
}

void CTimeMng::StopTime()
{
	m_bTimeStop = !m_bTimeStop;

	if(m_bTimeStop) m_dwStopBeginTime = m_dwCurTime;

	
}

void CTimeMng::RecoverTime()
{
	m_bTimeStop = FALSE;
}