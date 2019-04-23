#pragma once
#include<ddraw.h>
#include"SceneLink.h"
#include<vector>
#include"RectMath.h"


class CBuildingImg 
{
public:
	CBuildingImg(LPCSTR bmpfile, COLORREF key);
	~CBuildingImg();

	bool LoadBitmapFile(LPCSTR bmpfile, COLORREF key);

	void AddLinkRect(RECT);

	void AddCollideRect(RECT);

	void CreateCollideRectAuto(RECT);

	STRectData* GetTransformedColRect(RECT DesRect);

	STRectData* GetTransformedSceRect(RECT DesRect);

	WORD GetCommonTop();

	LPDIRECTDRAWSURFACE7 m_pSuf;

	DWORD m_dwBmpWidth;
	DWORD m_dwBmpHeight;

	WORD m_wCmTop;

	RECT m_rtImg;

	//link rect
	RECT m_rtEntrance;

	CRECTNode* m_pRootNode;

	
	vector<RECT>m_vector_LinkRect;

	vector<RECT>m_vector_ColRect;
};



