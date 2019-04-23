#pragma once
#include<ddraw.h>
#include<cmath>
#include<vector>
#include<d3dx9math.h>
#include<string>
#include"MyStruct.h"

using namespace std;


//math
float CalculateDistance(float x1, float y1, float x2, float y2);

float CalculateDistance(D3DXVECTOR2 &v2_a, D3DXVECTOR2 &v2_b);

bool AreTheSameRect(RECT rectA, RECT rectB);

int GetRectWidth(RECT &_rect);
int GetRectHeight(RECT &_rect);
int GetRectOblique(RECT &_rect);

POINT GetRectCenter(RECT &_rect);

RECT GetScaledRect(RECT rect, float Scale_x, float Scale_y);

RECT CreateRect(POINT rectCT, int width, int height);

RECT GetHalfRect(RECT &rect);

_CIRCLE GetCircleInvRect(RECT &rect);

struct STRect_Relate
{
	STRect_Relate() :offset_x(0), offset_y(0), Width(0), Height(0) {}
	
	STRect_Relate(POINT relate, RECT &__rect)
		:Width(GetRectWidth(__rect)), Height(GetRectHeight(__rect))
		,rect(__rect)
	{
		POINT ct = GetRectCenter(__rect);
		
		offset_x = relate.x - ct.x;
		offset_y = relate.y - ct.y;
	}
	STRect_Relate(POINTFLOAT relate, RECT &__rect)
		:Width(GetRectWidth(__rect)), Height(GetRectHeight(__rect))
		, rect(__rect)
	{
		POINT ct = GetRectCenter(__rect);

		offset_x = relate.x - ct.x;
		offset_y = relate.y - ct.y;
	}
	STRect_Relate(D3DXVECTOR2 relate, RECT &__rect)
		:Width(GetRectWidth(__rect)), Height(GetRectHeight(__rect))
		, rect(__rect)
	{
		POINT ct = GetRectCenter(__rect);

		offset_x = relate.x - ct.x;
		offset_y = relate.y - ct.y;
	}


	STRect_Relate(int ox, int oy, RECT &__rect)
		:offset_x(ox), offset_y(oy), rect(__rect)
	{
		Width = GetRectWidth(__rect);
		Height = GetRectHeight(__rect);
	}

	STRect_Relate(int ox, int oy, WORD _width, WORD _Height)
		:offset_x(ox), offset_y(oy), Width(_width), Height(_Height)
	{
		rect = CreateRect({ 0,0 }, Width, Height);
	}

	void UpdateRect(POINT &pt)
	{
		rect = CreateRect({ pt.x - offset_x,pt.y - offset_y }, Width, Height);
	}
	void UpdateRect(POINTFLOAT &ptf)
	{
		int x = ptf.x;
		int y = ptf.y;
		rect = CreateRect({ x - offset_x,y - offset_y }, Width, Height);
	}
	void UpdateRect(D3DXVECTOR2 &ptf)
	{
		int x = ptf.x;
		int y = ptf.y;
		rect = CreateRect({ x - offset_x,y - offset_y }, Width, Height);
	}

	int offset_x, offset_y;
	WORD Width, Height;
	RECT rect;
};

struct STRectEx
{
	STRectEx(RECT rt)
	{
		rect = rt;
		UpdateRectState();
	}

	RECT rect;
	DWORD width;
	DWORD Height;
	POINTFLOAT RectCenter;
private:
	void UpdateRectState()
	{
		width = GetRectWidth(rect);
		Height = GetRectHeight(rect);
		RectCenter.x = rect.left + width / 2;
		RectCenter.y = rect.top + Height / 2;
	}
};

struct STRectData
{
	~STRectData()
	{
		if (pRect != NULL)
		{
			delete[]pRect;
			pRect = NULL;
		}
	}

	RECT* pRect = NULL;

	int nRectCount = 0;
};

typedef struct STGrid
{
	STGrid() :wRow(0), wColumn(0) {}
	STGrid(WORD Row, WORD Column) :wRow(Row), wColumn(Column) {}

	WORD wRow;
	WORD wColumn;
}GRID;


class CRECTNode
{
public:
	CRECTNode();
	CRECTNode(RECT rect, LPCSTR str="temp");
	~CRECTNode();

	void AddRect(CRECTNode *rectnode);

	void AddRect(RECT rect, LPCSTR str = "tempsub");

	RECT FindRect(LPCSTR str);

	void TraversalNode(void(*ptrf)(RECT rect));

	void Init();

	void TransformRect(float Hscale, float Vscale, int offset_x, int offset_y);

	void TransformRect(RECT rect);

	void DestroyChildren();

	STRectData* GetChildrenRect();

	RECT m_RECT;

	const RECT m_RECTORG;

	DWORD m_dwWidth;

	DWORD m_dwHeight;

	int m_nOffset_x;

	int m_nOffset_y;

	vector<CRECTNode*>m_vector_child;

	string m_strName;

	CRECTNode *m_pParent;
};

//RECT GetScaledRectEX(RECT Rootrect,RECT Sub) 
//{
//	CRECTNode node(rect);
//
//	node.AddRect(rect);
//}


//class CRECTGroup
//{
//	CRECTGroup();
//	CRECTGroup(RECT rect);
//	~CRECTGroup();
//
//	
//	void Update(RECT rect);
//
//	void ResetRect();
//
//	CRECTNode *m_pRectRoot;
//	RECT m_RectOrg;
//};


//RECT* GetRidOfRect(RECT Bigrect, RECT Smallrect)
//{
//	RECT* pRect = NULL;
//
//	rect.left
//
//
//
//}




//RECT GetSurfaceSize(LPDIRECTDRAW7 *suf);


