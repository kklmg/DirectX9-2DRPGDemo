#pragma once
#include <windows.h>
#include <mmsystem.h>
#include "DxMng.h"
#pragma comment( lib,"winmm.lib" )
#ifndef __FPSMNG_H__
#define __FPSMNG_H__          

class CFpsMng
{
public:
	CFpsMng();						// 디폴트는 지연시간 없음
	virtual ~CFpsMng();

	void UpdateDeltaTime();

	void  SetFPS(DWORD dwFPS = 0);	// dwFPS == 0, 지연시간 없음
	DWORD GetFPS(void);				// 현재 FPS를 얻는다

	void  FrameWaiting(void);		// 설정한 프레임수로 조절 ( Polling  )
	void  FrameWaitingEx(void);		// 설정한 프레임수로 조절 ( Blocking )

private:
	DWORD m_dwFramesRendered;
	DWORD m_dwFps;

	double m_dOneSecLastTime;		// 1 second
	double m_dOneSecDeltaTime;		// 1 second
	double m_dMPF;					// Millisecond Per Frame
	double m_dLastTime;				// Timer

	double m_dProcTime;				// 1 Frame 처리시간
};

#endif