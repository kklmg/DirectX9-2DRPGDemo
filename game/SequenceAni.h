#pragma once
//----------------------------------------------
//My Head File
//----------------------------------------------
#include"Animation.h"


//----------------------------------------------
//StructAnimation desc 
//----------------------------------------------
struct STAnimationDesc 
{	
	LPDIRECTDRAWSURFACE7 lpDDS = NULL;

	int MaxFrame = 0;

	DWORD Row = 0;
	DWORD Column = 0;

	int Vscale = 1;
	int Hscale = 1;


	void SetAniSeqDesc(LPDIRECTDRAWSURFACE7 surface, int maxframe, float vscale, float hscale)
	{
		lpDDS = surface;
		MaxFrame = maxframe;

		Vscale = vscale;
		Hscale = hscale;	
	}

	void SetAniSheetDesc(LPDIRECTDRAWSURFACE7 surface, int maxframe,DWORD row,DWORD column, float vscale, float hscale) 
	{
		lpDDS = surface;
		MaxFrame = maxframe;

		Row = row;
		Column = column;

		Vscale = vscale;
		Hscale = hscale;
	}
	
};


//----------------------------------------------
//Class Sequence Animation
//----------------------------------------------
class CAni_Sequence :public CAnimation
{
public:
	CAni_Sequence();

	CAni_Sequence(STAnimationDesc desc);

	CAni_Sequence(LPCSTR BitmapFile, int maxFrame, float Vscale, float Hscale, COLORREF key);

	virtual ~CAni_Sequence();

	//Create Bitmap Surface
	bool LoadBitmapFile(LPCSTR FileName, int maxFrame, float Vscale, float Hscale, COLORREF key);

	//Display Animation
	virtual bool DisplayAnimation(RECT desRect, LPDIRECTDRAWSURFACE7 DDSDest, double Rate, bool repeat, bool ActiveScale);

	virtual bool RepeatAnimation(RECT desRect, LPDIRECTDRAWSURFACE7 DDSDest, double Rate, DWORD repeatcount, bool ActiveScale);

	virtual bool DisplayAnimation(POINT point, LPDIRECTDRAWSURFACE7 DDSDest, double Rate, bool repeat, bool ActiveScale);

	virtual bool DisplayOneFrame(POINT point, LPDIRECTDRAWSURFACE7 DDSDest, DWORD showFrame, bool ActiveScale);

	virtual bool DisplayOneFrame(RECT desRect, LPDIRECTDRAWSURFACE7 DDSDest, DWORD showFrame, bool ActiveScale);

	//Init Source Rect
	bool InitSourceRect();

	RECT GetSourceRect(DWORD frame);

	bool SetSourceRect(DWORD index, RECT rect);

	//Init manual
	void Init(LPDIRECTDRAWSURFACE7 lpdds, int maxFrame, float Vscale, float Hscale,
		DWORD Width, DWORD Height, DWORD CellWidth, DWORD CellHeight);

	//Get Surface
	const LPDIRECTDRAWSURFACE7 GetSurface();

	//Clear Surface /*/ not Delete
	void ClearSurfaceAdress();

private:
	LPDIRECTDRAWSURFACE7 m_pDDSAnimation;

	DWORD m_dwHeight;

	DWORD m_dwWidth;

	DWORD m_dwCellWidth;

	DWORD m_dwCellHeight;

	RECT* m_pRect;

	bool m_bCopyCreate;
};
