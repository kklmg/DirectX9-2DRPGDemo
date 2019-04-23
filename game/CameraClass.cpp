#pragma once
#include"CameraClass.h"
#include"Global_data.h"
#include"initDD.h"
#include"blit.h"
#include"RectMath.h"
#include"InputClass.h"


CCamera::CCamera(CTileMap* map)
{
	m_pLookObj = NULL;

	if (map->GetMapWidth()>HRES && map->GetMapHeight()>VRES)
	{
		InitCamera(HRES, VRES, map);
	}
	else if (map->GetMapWidth()<HRES && map->GetMapHeight()<VRES)
	{
		InitCamera(map->GetMapWidth(), map->GetMapHeight(), map);
	}
	else if (map->GetMapWidth()<HRES)
	{
		InitCamera(map->GetMapWidth(), VRES, map);
	}
	else if (map->GetMapHeight() < VRES) 
	{	
		InitCamera(HRES, map->GetMapHeight(), map);	
	}
}

CCamera::CCamera(DWORD width, DWORD height, CTileMap *map) 
{
	m_pLookObj = NULL;
	InitCamera(width, height, map);
}



CCamera::~CCamera() 
{
	m_pLookObj = NULL;
}

bool CCamera::InitCamera(int width, int height, CTileMap* map) 
{
	m_pLookObj = NULL;

	m_pCurMap = map;

	m_dwWidth = width;
	m_dwHeight = height;

	m_dwHalfWidth = m_dwWidth / 2;;
	m_dwHalfHeight = m_dwHeight / 2;
		
	m_xyCenter.x = m_dwWidth / 2;
	m_xyCenter.y = m_dwHeight / 2;

	m_rtCamera = CreateRect(m_xyCenter, m_dwWidth, m_dwHeight);


	return TRUE;
}

void CCamera::Update()
{
//	if (m_bIsLockHorizontal && m_bIsLockVertical) return;

	if (m_pLookObj == NULL) 
	{
		MoveCamera(2);		
		AvoidOverBound();
	}
	else
	{		
		LockCameraAtObject();
		AvoidOverBound();
	}	
	m_rtCamera = CreateRect(m_xyCenter, m_dwWidth, m_dwHeight);
}

//used in avoid camera over bound
void CCamera::AvoidOverBound()
{
	if (m_xyCenter.x < m_dwHalfWidth
		&& m_xyCenter.y < m_dwHalfHeight) //lock to left-top corner
	{
		m_xyCenter.x = m_dwHalfWidth;
		m_xyCenter.y = m_dwHalfHeight;
	}
	else if (m_xyCenter.x < m_dwHalfWidth
		&& m_xyCenter.y > m_pCurMap->GetMapHeight() - m_dwHalfHeight) //lock to left-bottom corner
	{
		m_xyCenter.x = m_dwHalfWidth;
		m_xyCenter.y = m_pCurMap->GetMapHeight() - m_dwHalfHeight;
	}
	else if (m_xyCenter.x > m_pCurMap->GetMapWidth() - m_dwHalfWidth
		&& m_xyCenter.y < m_dwHalfHeight)  //lock to right-top corner
	{
		m_xyCenter.x = m_pCurMap->GetMapWidth() - m_dwHalfWidth;
		m_xyCenter.y = m_dwHalfHeight;
	}
	else if (m_xyCenter.x > m_pCurMap->GetMapWidth() - m_dwHalfWidth
		&& m_xyCenter.y > m_pCurMap->GetMapHeight() - m_dwHalfHeight)  //lock to right-bottom corner
	{
		m_xyCenter.x = m_pCurMap->GetMapWidth() - m_dwHalfWidth;
		m_xyCenter.y = m_pCurMap->GetMapHeight() - m_dwHalfHeight;
	}
	else
	{
		if (m_xyCenter.x < m_dwHalfWidth)//lock to left edge
		{
			m_xyCenter.x = m_dwHalfWidth;
		}
		else if (m_xyCenter.x > m_pCurMap->GetMapWidth() - m_dwHalfWidth)//lock to right edge
		{
			m_xyCenter.x = m_pCurMap->GetMapWidth() - m_dwHalfWidth;
		}
		else if (m_xyCenter.y < m_dwHalfHeight)
		{
			m_xyCenter.y = m_dwHalfHeight;
		}
		else if (m_xyCenter.y > m_pCurMap->GetMapHeight() - m_dwHalfHeight)
		{
			m_xyCenter.y = m_pCurMap->GetMapHeight() - m_dwHalfHeight;
		}
	}
}

bool CCamera::LookAtObject(CObject* obj) 
{
	if (obj == NULL)return FALSE;
	else
	{
		m_pLookObj = obj;
		return TRUE;
	}
}

//used in lock camera at the object
bool CCamera::LockCameraAtObject()
{
	if (m_pLookObj == NULL) return FALSE;
	else 
	{
		m_xyCenter.x = m_pLookObj->GetX();
		m_xyCenter.y = m_pLookObj->GetY();
		return TRUE;
	}
}

void CCamera::DisLockCamera()
{
	m_pLookObj = NULL;
}

void CCamera::MoveCamera(int speed) 
{
	if (DIKEYDOWN(DIK_NUMPAD4)) m_xyCenter.x -= speed;	
	if (DIKEYDOWN(DIK_NUMPAD6)) m_xyCenter.x += speed;
	if (DIKEYDOWN(DIK_NUMPAD8)) m_xyCenter.y -= speed;
	if (DIKEYDOWN(DIK_NUMPAD2)) m_xyCenter.y += speed;
}

RECT CCamera::GetCameraRect() const
{
	return m_rtCamera;
}

DWORD CCamera::GetWidth() const
{
	return m_dwWidth;
}
DWORD CCamera::GetHalfWidth() const
{
	return m_dwHalfWidth;

}

DWORD CCamera::GetHeight() const
{
	return m_dwHeight;
}
DWORD CCamera::GetHalfHeight() const
{
	return m_dwHalfHeight;
}

