#pragma once
#include<Windows.h>
#include<ddraw.h>
#include"TileMap.h"
#include"ObjectClass.h"


class CCamera
{
public:
	//creatot & Destroyer
	CCamera(CTileMap *map);

	CCamera(DWORD width,DWORD height,CTileMap *map);

	~CCamera();

public:
	//Get Function
	RECT GetCameraRect() const;

	DWORD GetWidth() const;
	DWORD GetHalfWidth() const;

	DWORD GetHeight() const;
	DWORD GetHalfHeight() const;

	//public function
	bool InitCamera(int width, int height, CTileMap* map);	

	void MoveCamera(int speed);

	void Update();

	bool LookAtObject(CObject* obj);

	void DisLockCamera();



	
private:
	CTileMap* m_pCurMap;

	RECT m_rtCamera;

	DWORD m_dwWidth;
	DWORD m_dwHeight;

	DWORD m_dwHalfWidth;
	DWORD m_dwHalfHeight;

	void AvoidOverBound();

	bool LockCameraAtObject();

	POINT m_xyCenter;

	CObject *m_pLookObj;
};


