#pragma once
#include"SequenceAni.h"

class CAni_Sheet 
{
public:
	CAni_Sheet();

	CAni_Sheet(LPCSTR BitmapFile, int row, int column, float Vscale, float Hscale, COLORREF key);

	virtual ~CAni_Sheet();

	bool LoadAniSheet(LPCSTR FileName, DWORD row, DWORD column, float Vscale, float Hscale, COLORREF key);

	bool DisplayAnimation(RECT DestRect, LPDIRECTDRAWSURFACE7 DestSuf,DWORD DrawRow, double Rate, bool repeat, bool ActiveScale);
	
	bool DisplayAnimation(POINT DestPoint, LPDIRECTDRAWSURFACE7 DestSuf, DWORD DrawRow, double Rate, bool repeat, bool ActiveScale);

	bool DisplayOneFrame(RECT DestRect, LPDIRECTDRAWSURFACE7 DestSuf, DWORD DrawRow, DWORD DrawFrame, bool ActiveScale);

	bool DisplayOneFrame(POINT DestPoint, LPDIRECTDRAWSURFACE7 DestSuf, DWORD DrawRow, DWORD DrawFrame, bool ActiveScale);

	void ResetALLAnimation();

	bool IsAnimaitionEnd(DWORD row);

	DWORD GetCurFrame(DWORD row);

	RECT GetSourceRect(DWORD row, DWORD column);

	void ClearALLSurface();


private:
	LPDIRECTDRAWSURFACE7 m_pDDS_AniSheet;

	CAni_Sequence *m_pAni;

	DWORD m_dwRow;
	DWORD m_dwColumn;

	DWORD m_dwDrawRow;
	DWORD m_dwDrawColumn;

	DWORD m_dwSheetWidth;
	DWORD m_dwSheetHeight;

	DWORD m_dwCellHeight;//per frame height
	DWORD m_dwCellWidth;//per frame width

};