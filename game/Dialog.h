#pragma once
#include<ddraw.h>
#include"TextSurface.h"
#include"TimerClass.h"
#include<list>
#include"DialogData.h"


//---------------------------------------------
//Struct Text Slot
//---------------------------------------------
struct STTextSlot
{
	STTextSlot(RECT txtBound, WORD LineCount, WORD LineInterval) :m_pRect(NULL), m_wLineCount(LineCount)
	{
		int height = GetRectHeight(txtBound);	
		int ivCount = LineCount - 1;
		height = height - LineInterval*ivCount;

		int CellHeight = height / LineCount;

		m_pRect = new RECT[LineCount];

		for (int i = 0; i < LineCount; ++i)
		{
			m_pRect[i].left = txtBound.left;
			m_pRect[i].right = txtBound.right;

			m_pRect[i].top = txtBound.top + (CellHeight*i) + (i*LineInterval);
			m_pRect[i].bottom = txtBound.top + (CellHeight*(1 + i)) + (i*LineInterval);
		}
	}
	~STTextSlot() 
	{
		if (m_pRect != NULL) 
		{
			delete[] m_pRect;
			m_pRect = NULL;
		}
	}

	RECT *m_pRect;
	WORD m_wLineCount;
};


//---------------------------------------------
//Class Dilog Manager
//----------------------------------------------
class CDialogMng 
{
	enum enDialogStage 
	{
		eTalkNone,
		eTalkInit,
		eTalking,
		eTalkingEnd,	
	};

public:
	CDialogMng();
	~CDialogMng();

	bool Init();

	//Draw  Text
	void DrawDialogText(LPDIRECTDRAWSURFACE7 Dest);
	void DrawDialogText_Dynamic(LPDIRECTDRAWSURFACE7 Dest);
	bool DrawDialog_Dynamic(LPDIRECTDRAWSURFACE7 Dest);

	//Draw Box
	HRESULT DrawDialogBox(LPDIRECTDRAWSURFACE7 Dest);
	HRESULT DrawDialogBox_AB(LPDIRECTDRAWSURFACE7 Dest);

	//Draw ALl
	bool DrawDialogEx(LPDIRECTDRAWSURFACE7 Dest);

	//Get Draw Text
	void LoadText(LPCSTR str);
	void LoadDialog(CDialogChain* dialog);

	bool NextPage();

	//Reset 
	void reset();

private:
	//Init 
	void InitIterator();

	//Dialog Update
	bool UpdateStr(float rate);
	bool UpdateDialog(float rate);
	void UpdateIter();

	
private:
	bool m_bInited;

	WORD m_wCharInRow;
	WORD m_wCharInColumn;

	//Get Dialog
	string m_strLoad;
	CDialogChain* m_pDialogLoad;


	//Update 
	enDialogStage m_enTalkStage;
	CTimer m_Timer;

	int m_nCurRef;
	int m_nTCount;

	string::iterator m_strIterBegin;
	string::iterator m_strIterEnd;
	string::iterator m_strIterCur;
	

	//Draw Destination
	RECT m_rtDestrect;
	RECT m_rtTextBound;

	STTextSlot *m_pstSlot;


	//Box Surface
	LPDIRECTDRAWSURFACE7 m_lpDDSDialogBox;
	LPDIRECTDRAWSURFACE7 m_lpDDSDialogBox_AB;
	void ClearABSurf();
};