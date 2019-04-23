#pragma once
#include<Windows.h>


class CTimeMng
{
public:
	CTimeMng();
	~CTimeMng();

	void Init();

	void TimeUpdate();

	void StopTime();

	void RecoverTime();

	double GetCurTime()const;

	double GetCurTime_s()const;

	double GetDeltaTime()const;


private:



	DWORD m_dwBeginTime;

	DWORD m_dwCurTime;

	DWORD m_dwOneFrameDeltaTime;

	double m_dOneSecDeltaTime;

	DWORD m_dwLastTime;

	DWORD m_dwPreTime;

	DWORD m_dwStopBeginTime;

	DWORD m_dwStopLastTime;
	
	bool m_bTimeStop;

	bool m_bIsNewLoop;

	int m_nLoopCount;
};

