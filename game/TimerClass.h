#pragma once
#include<Windows.h>
#include"TimeManager.h"

enum enTimerState
{
	eTmdefault,
	eTmGoing,
	eTmEnd,
	eTmStop,
};

class CTimer
{
public:
	CTimer(bool GetGlobalTime = TRUE);
	CTimer(enTimerState eState, bool GetGlobalTime = TRUE);
	~CTimer();

	void TimeUpdate();

	void ResetTimer();
	void stopTimer();

	void CloseTimer();

	bool LoopTimer(float Rate);
	bool OnceTimer(float dEndTime);

	
private:
	enTimerState m_enTmState;

	double m_dStartTime;
	double m_dLastTime;
	double m_dEndTime;
	double m_dStopTime;

	bool m_bGetGlobalTime;
};