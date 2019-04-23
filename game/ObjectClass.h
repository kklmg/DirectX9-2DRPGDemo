#pragma once
//-----------------------------------------------------------
//Include Head File
//-----------------------------------------------------------
#include<ddraw.h>
#include<string>


//-----------------------------------------------------------
//Include My Head File
//-----------------------------------------------------------
#include"position.h"
#include"CollideArea.h"
#include"EnumGroup.h"


//-----------------------------------------------------------
//Object State struct
//-----------------------------------------------------------
struct STObjState 
{
	STObjState() :bHasBeenCollide(FALSE), bHasBeenAttack(FALSE) {}

	void ResetState() 
	{
		bHasBeenCollide = FALSE;
		bHasBeenAttack = FALSE;
	}

	bool bHasBeenCollide;
	bool bHasBeenAttack;
};




//-----------------------------------------------------------
//Object Class
//-----------------------------------------------------------
class CObject : public CPosition
{
	//---------------------------------------------------------
	//Creatior & Destroyer
	//---------------------------------------------------------
public:
	CObject();
	virtual ~CObject();

	//Init 
	void Init(int x, int y, DWORD width, DWORD Height);


	//---------------------------------------------------------
	//Object Basic
	//---------------------------------------------------------	
protected:
	string m_strName;				//Object Name

	//Draw Rect
	DWORD m_dwWidth;				//Draw Width
	DWORD m_dwHeight;				//Draw Height

	RECT m_rectDraw;				//Draw Rect

public:
	LPCSTR GetName()const;				

	void SetName(string);


	//---------------------------------------------------------
	//Collision
	//---------------------------------------------------------
public:
	CCollideArea* GetCollideArea()const;

protected:
	void CreateCollideArea(bool IsDynamic = FALSE);
	void DestroyCollideArea();

	//collision Area
	CCollideArea *m_pCollideArea;

	
	//---------------------------------------------------------
	//Check object State 
	//---------------------------------------------------------
public:
	bool IsAlive()const;

	bool IsMoveAble()const;

	bool IsObjNearBy(CObject*obj)const;
	

	//---------------------------------------------------------
	//object State
	//---------------------------------------------------------
public:
	void MarkHasBeenCollide();

	void MarkHasBeenAttack();

	//Reset State
	void ResetState();

protected:
	STObjState m_stObjState;		//Object State


	//---------------------------------------------------------
	//object Attribute 
	//---------------------------------------------------------
public:
	enObjectType GetObjType() const;

	STObjState GetObjState() const;

	enObjectForce GetObjForce() const;

	POINTFLOAT GetPatrolPoint()const;

	RECT GetDrawRect()const;


protected:
	enObjectType m_enObjType;		//Object Type

	bool m_bIsAlive;				//Object alive

	bool m_bControlAble;			//Object Controlable

	enObjectForce m_enForce;		//Object force

	bool m_bMoveAble;				//Object Moveable
	
	POINTFLOAT m_pointfPatrol;		//patrol Position
	
public:
	bool m_bDrawFirst;				//Always Draw First

	//---------------------------------------------------------
	//Object Direction
	//---------------------------------------------------------
public:
	enDirection GetCurDir() const;

protected:
	bool SetObjectDir(enDirection);

	void LockDir();
	void UnLockDir();

private:
	enDirection m_enDir;		//Object direction

	bool m_bLockDir;			//Lock Direction


	//---------------------------------------------------------
	//Static Member
	//---------------------------------------------------------
private:
	static DWORD s_dwObjCount;	//object count

public:
	DWORD  static GetObjectCount();
	
	//---------------------------------------------------------
	//Object Update
	//---------------------------------------------------------
public:
	void UpdateObject();

	void UpdatePatrolPoint();


	//---------------------------------------------------------
	//Pure Virtual Function
	//---------------------------------------------------------
public:
	//Update All
	virtual void UpdateAll() = 0;

	virtual HRESULT DrawObject(LPDIRECTDRAWSURFACE7 DestSuf, DWORD DrawSetting) = 0;
};

