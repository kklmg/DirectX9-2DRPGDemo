//-----------------------------------------------------------
//Include Head File
//-----------------------------------------------------------
#include <stdarg.h>



//-----------------------------------------------------------
//Include My Head File
//-----------------------------------------------------------
#include"blit.h"
#include"CollideArea.h"
#include"Global_data.h"


//------------------------------------------------------------------------------------------------
//Create CollideArea
//------------------------------------------------------------------------------------------------
CCollideArea::CCollideArea(D3DXVECTOR2 &pointf)
	:m_refPointf(pointf), 
	m_enPhyStr(enPhysicalStrength::epyMiddle),
	m_bHasBeenCollide(FALSE),
	m_bActiveCollide(TRUE),
	m_bIsDynamic(TRUE) 
{}

CCollideArea::~CCollideArea() 
{
}



//------------------------------------------------------------------------------------------------
//Area State
//------------------------------------------------------------------------------------------------
bool CCollideArea::IsActiveCollide() const
{
	return m_bActiveCollide;
}

void CCollideArea::SetAreaDynamic(bool _b)
{
	m_bIsDynamic = _b;
}

void CCollideArea::ResetCollideState() 
{
	m_bHasBeenCollide = FALSE;
}

DWORD CCollideArea::GetRectCount() const
{
	return m_vectorRect.size();
}

DWORD CCollideArea::GetCircleCount() const
{
	return m_vectorCircle.size();
}

DWORD CCollideArea::GetCollideAreaCount() const
{
	return m_vectorCircle.size() + m_vectorRect.size();
}

void CCollideArea::SwtichActiveState()
{
	m_bActiveCollide = !m_bActiveCollide;
}

void CCollideArea::SetCollideStrength(enPhysicalStrength enStr)
{
	m_enPhyStr = enStr;
}



//------------------------------------------------------------------------------------------------
//Area Attribute
//------------------------------------------------------------------------------------------------
enPhysicalStrength CCollideArea::GetPhyStrength()  const
{
	return m_enPhyStr;
}

void CCollideArea::SetCollideRect(int Count, ...)
{
	m_vectorRect.resize(Count);

	va_list ap;
	va_start(ap, Count);

	for (int i = 0; i < Count; i++) 
	{
		m_vectorRect.push_back(va_arg(ap, STRect_Relate));
	}
	va_end(ap);
}

void CCollideArea::SetCollideCircle(int Count, ...)
{
	m_vectorCircle.resize(Count);

	va_list ap;
	va_start(ap, Count);

	for (int i = 0; i < Count; i++)
	{
		m_vectorCircle[i] = va_arg(ap, _CIRCLE);
	}

	va_end(ap);
}

//const vector<RECT>*  CCollideArea::GetRectVector() const
//{
//	return &m_vectorRect;
//}
//
//const vector<_CIRCLE>*  CCollideArea::GetCircleVector() const
//{
//	return &m_vectorCircle;
//}
void CCollideArea::AddCollideRect(RECT rect)
{
	if (m_bIsDynamic == FALSE && g_pCurMap != NULL)
	{
		if(g_pCurMap)
		g_pCurMap->MarkObstacle(rect);
	}
	m_vectorRect.push_back(STRect_Relate(m_refPointf, rect));
}

void CCollideArea::AddCollideRect(STRect_Relate rect) 
{
	if (m_bIsDynamic == FALSE && g_pCurMap!=NULL)
	{
		rect.UpdateRect(m_refPointf);
	
		g_pCurMap->MarkObstacle(rect.rect);
	}

	m_vectorRect.push_back(rect);
}

void CCollideArea::AddCollideCircle(_CIRCLE circle) 
{
	m_vectorCircle.push_back(circle);
}

RECT CCollideArea::GetRect(DWORD index) const
{
	if (index > m_vectorRect.size())return{ 0 };
	else
	{
		return m_vectorRect[index].rect;
	}
}

_CIRCLE CCollideArea::GetCircle(DWORD index) const
{
	if (index > m_vectorCircle.size()) return _CIRCLE() ;
	else
	{
		return m_vectorCircle[index];
	}
}

bool CCollideArea::CCollideArea::IsDynamic()
{
	return m_bIsDynamic;
}



//------------------------------------------------------------------------------------------------
//Area Update & Display
//------------------------------------------------------------------------------------------------
//POINT CCollideArea::FigureOutOffset()
//{
//	POINT offset;
//
//	offset.x = m_refPointf.x - m_PointfOrg.x;
//	offset.y = m_refPointf.y - m_PointfOrg.y;
//
//	return offset;
//}


bool CCollideArea::UpdateRect(DWORD index, RECT rect) 
{
	if (index > m_vectorRect.size())return FALSE;
	else
	{
		m_vectorRect[index].UpdateRect(m_refPointf);
		return TRUE;
	}

}

bool CCollideArea::UpdateCircle(DWORD index, _CIRCLE circle) 
{
	if (index > m_vectorCircle.size())return FALSE;
	else
	{
		m_vectorCircle[index] = circle;
		return TRUE;
	}
}

void  CCollideArea::UpdateArea() 
{
	if (!m_bIsDynamic) 
		return;

//	POINT offset = FigureOutOffset();

	
	vector<STRect_Relate>::iterator iter_R;

	for (iter_R = m_vectorRect.begin(); iter_R != m_vectorRect.end(); iter_R++)
	{
		(*iter_R).UpdateRect(m_refPointf);
	}

	//vector<_CIRCLE>::iterator iter_C;

	//for (iter_C = m_vectorCircle.begin(); iter_C != m_vectorCircle.end(); iter_C++)
	//{
	//	(*iter_C).point.x += offset.x;
	//	(*iter_C).point.y += offset.y;
	//}
}


//-----------------------------------------------------------------------------------------------------UnComplete!!
bool CCollideArea::DrawCollideArea(LPDIRECTDRAWSURFACE7 suf)
{
	vector<STRect_Relate>::iterator iter;


	for (iter = m_vectorRect.begin(); iter != m_vectorRect.end(); iter++)
	{
		_FillRectEx(suf, &(*iter).rect, RGB(255, 255, 255));
	}
	return TRUE;
}




