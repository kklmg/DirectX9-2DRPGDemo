#pragma once
#include<ddraw.h>
#include"TileMap.h"
#include"TimerClass.h"

enum enAniState 
{
	eAniDefault,
	eAniGoing,
	eAniEnd,
	eAniStop,
};

class CAnimation
{
	//-----------------------------------------------------------------
	//Animation Class Basic
	//-----------------------------------------------------------------
public:
	CAnimation();
	virtual ~CAnimation();
	




	const DWORD GetCurFrame();

	bool InitAnimation();

	void ResetAnimation();

	bool IsAnimationEnd();

	void SetScale(float Hscale, float Vscale);

	RECT GetScaleRect(RECT DestRect);

	//draw sprite
	virtual bool DisplayAnimation(RECT desRect, LPDIRECTDRAWSURFACE7 DDSDest, double Rate, bool repeat, bool ActiveScale) = 0;

	virtual bool DisplayAnimation(POINT point, LPDIRECTDRAWSURFACE7 DDSDest, double Rate, bool repeat, bool ActiveScale) = 0;

	virtual bool DisplayOneFrame(POINT point, LPDIRECTDRAWSURFACE7 DDSDest, DWORD showFrame, bool ActiveScale) = 0;

	virtual bool DisplayOneFrame(RECT desRect, LPDIRECTDRAWSURFACE7 DDSDest, DWORD showFrame, bool ActiveScale) = 0;


protected:

	void SetMaxFrame(DWORD frame);

	void UpdateFrame(float rate, bool repeat);

	void UpdateFrame(float rate, DWORD repeatcount);

protected:

	enAniState m_enAniState;

	DWORD m_dwCurFrame;

	DWORD m_dwMaxFrame;

	float m_fHScale;

	float m_fVScale;	

	CTimer m_Timer;

	DWORD m_dwRepeatCount;
};



//class CAni_FrameFile:public CAnimation
//{
//public:
//	CAni_FrameFile();
//	virtual ~CAni_FrameFile();
//
//private:
//
//};
//
//CAni_FrameFile::CAni_FrameFile()
//{
//}
//
//CAni_FrameFile::~CAni_FrameFile()
//{
//}
//









//class ANI2
//{
//public:
//	LPDIRECTDRAWSURFACE7 *ppAniSuf = NULL;
//	int Rate;
//
//	double dStartTime;
//	RECT *sizeRect;
//	int *Width;
//	int *Height;
//	short Anistate;
//
//	ANI2();
//	ANI2(int Frame);
//	ANI2(LPCSTR BitmapFile, int Frame, COLORREF key);
//	~ANI2();
//
//	bool __LoadBitmap(LPCSTR __Bitmap, COLORREF key);
//	bool __LoadBitmap(LPCSTR __Bitmap, int _maxFrame, COLORREF key);
//	bool __LoadBitmap(LPCSTR BitmapFile, int Frame, float Vscale, float Hscale, COLORREF key);
//	RECT Output(LPDIRECTDRAWSURFACE7 pSuf, double Rate, int maxFrame);
//	bool DisAni(RECT desRect, double Rate, bool adjust);
//	void resetFrame();
//
//};
//
//class ANI3
//{
//public:
//	//surface
//	LPDIRECTDRAWSURFACE7 pAniSuf = NULL;
//	int nRow;
//	int nColumn;
//
//	//per Frame size
//	int nHeight;
//	int nWidth;
//
//	short Anistate;
//	//timer
//	__timer TIMER1;
//
//	RECT *resRect;
//
//
//
//	ANI3();
//	ANI3(LPCSTR BitmapFile, int Frame, COLORREF key);
//	~ANI3();
//
//	bool __LoadAniSheet(LPCSTR BitmapFile, int __Row, int __Column, COLORREF key);
//	bool __LoadBitmap(LPCSTR BitmapFile, int Frame, float Vscale, float Hscale, COLORREF key);
//	bool DisAni(RECT desRect, int disRow, double Rate, bool Scale);
//	bool DisAni(RECT desRect, int disRow, double Rate, bool Scale, LPDIRECTDRAWSURFACE7 dissuf);
//
//	bool DisOneFrame(RECT desRect, int disRow, int disColumn, bool Scale);
//	bool DisOneFrame(RECT desRect, int disRow, int disColumn, bool Scalebool, LPDIRECTDRAWSURFACE7 dissuf);
//	void resetFrame();
//};