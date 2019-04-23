#pragma once
#include<ddraw.h>
#include"TileMap.h"
#include"EnumGroup.h"
#include"ObjectClass.h"

struct STLine
{
	STLine() :pt_A({ 0,0 }), pt_B({ 0,0 }) {}
	POINT pt_A;
	POINT pt_B;
};

union UNLinkShape
{
	UNLinkShape(RECT &_rect) :rect(_rect) {}
	UNLinkShape(STLine &_line) :line(_line) {}
	STLine line;
	RECT rect;
};




struct STSceneDesc 
{
	RECT Entrance;
	enScene Scene;
	RECT Dest;
};

//------------------------------------------
//Class Scene Link Base
//------------------------------------------
class CSceneLink
{
	//Constructor & Destroyer
	//------------------------
public:
	CSceneLink(enShape LinkType,enScene Cur, enScene Dest, enDirection EntDir);
	virtual	~CSceneLink();

	

	//Active Link
	void ActiveLink();
	void DisActiveLink();
	bool IsActive()const;

	//scene setting
	void SetDestScene(enScene scene);
	void SetCurScene(enScene Scene);
	enScene GetCurScene() const;
	enScene GetDestScene() const;

	enShape GetLinkType()const;

	//Create opposite Link
	virtual CSceneLink* CreateOPPLinkAuto(bool Active) = 0;

	//Draw Link
	virtual	bool DisplayLinkInfo(LPDIRECTDRAWSURFACE7 suf) = 0;

	virtual bool IsObjTouchLink(CObject *obj) = 0;

	virtual bool IsInvolvedInrect(RECT &rect) = 0;

	virtual UNLinkShape GetDest() = 0;

	virtual bool IsTheSameAsLink(CSceneLink* Link) = 0;

	virtual POINT GetDestPos(CObject *obj) = 0;


protected:
	bool m_bIsActive;

	bool m_bHasOppLink;

	enScene m_enCurScene;
	enScene m_enDestScene;

	enDirection m_enEntDir;

	enShape m_enLinkType;
};







//Scene Link Rect
class CRectSLink :public CSceneLink 
{

public:
	CRectSLink(RECT Entrance, enScene Scene, RECT Dest, enDirection EntDir=enDirection::eUp);
	CRectSLink(STSceneDesc);

	CRectSLink(const CRectSLink & c);
	void operator=(const CRectSLink & c);

	~CRectSLink();


	//Init link
	void InitLink(RECT Entrance,RECT DestRect);

	//Link Setting
	void SetTriggerRect(RECT rect);
	void SetDestRect(RECT Dset);



	//Create opposite Link
	//Original Link: A -> B,	opposite Link B -> A
	virtual CSceneLink* CreateOPPLinkAuto(bool Active);

	//Draw Link
	virtual	bool DisplayLinkInfo(LPDIRECTDRAWSURFACE7 suf) ;

	virtual bool IsObjTouchLink(CObject *obj);

	virtual bool IsInvolvedInrect(RECT &rect);

	virtual bool IsTheSameAsLink(CSceneLink* Link);

	virtual UNLinkShape GetDest();

	virtual POINT GetDestPos(CObject *obj);

	CObject *m_pObj = NULL;

protected:
	RECT m_rtEntrance;
	RECT m_rtDest;

	RECT m_rtEnt_CZ;
	RECT m_rtDest_CZ;
};



class CLineSLink
{
public:
	CLineSLink();
	~CLineSLink();

private:

};




////------------------------------------------
////Class Scene Link
////------------------------------------------
//class CSceneLink
//{
////Constructor & Destroyer
////------------------------
//public:
//	CSceneLink();
//	CSceneLink(RECT Entrance, enScene Scene, RECT Dest);
//	CSceneLink(STSceneDesc);
//	
//	CSceneLink(const CSceneLink & c);
//	void operator=(const CSceneLink & c);
//
//	~CSceneLink();
//
//
//	//Init link
//	void InitLink(RECT Entrance, enScene Scene , RECT DestRect);
//
//	//Active Link
//	void ActiveLink();
//	void DisActiveLink();
//	bool IsActive()const;
//
//	//Link Setting
//	void SetEntranceRect(RECT rect);
//	void SetTransportPos(RECT Dset);
//	void SetTaransportScene(enScene scene);
//
//	void SetCurScene(enScene);
//
//
//
//	//Draw Link
//	bool DisplayLinkInfo(LPDIRECTDRAWSURFACE7 suf);
//	
//	//Get Data
//	enScene GetCurScene() const;
//	enScene GetDestScene() const;
//
//	//Create opposite Link
//	CSceneLink* CreateOPPLinkAuto(bool Active);
//
//
//
//	RECT m_rtEntrance;
//	RECT m_rtDest;
//	POINT m_ptTriggerLink;
//	DWORD m_dwLinkType;
//
//
//
//	bool IsObjNeary(CObject *obj);
//
//	bool IsObjInLink(CObject *obj);
//
//	bool IsObjNearyLarge(CObject *obj);
//
//	CObject *m_pObj = NULL;
//
//private:
//	bool m_bIsActive;
//
//	bool m_bMakeOpp;
//
//	enScene m_enCurScene;
//
//	enScene m_enDesScene;
//
//	
//};
//


//struct STSceLink
//{
//	STSceLink(enScene _scene, GRID& _pos) :Scene(_scene),gdPos(_pos)
//	{
//	
//	}
//	enScene Scene;
//	CTileMap* pmap;
//	GRID gdPos;
//
//	STSceLink*pOppLink;
//};
//
//
//
//
//class CLinkMng
//{
//
//
//	RECT m_rtRectPos;
//	
//	void CreateLink(enScene FromSce,GRID FromGD, enScene ToSce, GRID ToGD)
//	{
//
//		STSceLink* linkFrom = new STSceLink(FromSce,FromGD);
//		STSceLink* linkTo = new STSceLink(ToSce, ToGD);
//		linkFrom->pOppLink = linkTo;
//		linkTo->pOppLink = linkFrom;
//	}
//
//
//};