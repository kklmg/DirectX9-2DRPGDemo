#pragma once
//-----------------------------------------------------------
//Include Head File
//-----------------------------------------------------------
#include<ddraw.h>
#include<vector>

using namespace std;


//-----------------------------------------------------------
//Include My Head File
//-----------------------------------------------------------
#include"EnumGroup.h"
#include"MyStruct.h"
#include"RectMath.h"

//-----------------------------------------------------------
//class CollideArea
//-----------------------------------------------------------
class CCollideArea
{
	//-----------------------------------------------------------
	//Create CollideArea
	//-----------------------------------------------------------
public:
	CCollideArea(D3DXVECTOR2 &pointf);
	virtual ~CCollideArea();

	//-----------------------------------------------------------
	//Area State
	//-----------------------------------------------------------
public:
	bool IsActiveCollide() const;
	void SetAreaDynamic(bool);

	void ResetCollideState();

	void SwtichActiveState();
private:
	bool m_bHasBeenCollide;

	bool m_bActiveCollide;

	//-----------------------------------------------------------
	//Area Attribute
	//-----------------------------------------------------------
public:
	DWORD GetRectCount() const;
	DWORD GetCircleCount() const;
	DWORD GetCollideAreaCount() const;

	RECT GetRect(DWORD index)const;
	_CIRCLE GetCircle(DWORD index)const;

	//save collide rect / circle
	//const vector<RECT>* GetRectVector()const;
	//const vector<_CIRCLE>* GetCircleVector()const;

	enPhysicalStrength GetPhyStrength()const;
	void SetCollideStrength(enPhysicalStrength enStr);

	void SetCollideRect(int Count, ...);
	void SetCollideCircle(int Count, ...);
	
	void AddCollideRect(RECT rect);
	void AddCollideRect(STRect_Relate rect);
	
	void AddCollideCircle(_CIRCLE circle);

	bool IsDynamic();

private:
	D3DXVECTOR2 &m_refPointf;

	enPhysicalStrength m_enPhyStr;

	//vector<RECT>m_vectorRect;
	vector<STRect_Relate>m_vectorRect;

	vector<_CIRCLE>m_vectorCircle;

	bool m_bIsDynamic;


	//-----------------------------------------------------------
	//Area Update & Display
	//-----------------------------------------------------------
private:
	//POINT FigureOutOffset();

public:
	void UpdateArea();

	bool UpdateRect(DWORD index, RECT rect);

	bool UpdateCircle(DWORD index, _CIRCLE circle);

	bool DrawCollideArea(LPDIRECTDRAWSURFACE7 suf);

	
};
