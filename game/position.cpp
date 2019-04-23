#include"position.h"

CPosition::CPosition()
{
	m_V2Pos = { 0,0 };
}

CPosition::~CPosition()
{
}

CPosition::CPosition(float x, float y)
{
	m_V2Pos = { x,y };
}

void CPosition::SetPosition(float x, float y)
{
	m_V2Pos = { x,y };
}

void CPosition::SetPosition(D3DXVECTOR2 V2) 
{
	m_V2Pos = V2;
}

void CPosition::SetPosition(POINTFLOAT pos) 
{
	m_V2Pos.x = pos.x;
	m_V2Pos.y = pos.y;
}

void CPosition::SetX(float x) 
{
	m_V2Pos.x = x;
}

void CPosition::SetY(float y) 
{
	m_V2Pos.y = y;
}

float CPosition::GetX(void) const
{
	return m_V2Pos.x;
}

float CPosition::GetY(void) const 
{
	return m_V2Pos.y;
}

POINTFLOAT CPosition::GetPosition(void) const
{
	return{ m_V2Pos.x,m_V2Pos.y};
}


D3DXVECTOR2 CPosition::GetVector(void) const
{
	return m_V2Pos;
}

D3DXVECTOR2& CPosition::GetVectorRef(void)
{
	return m_V2Pos;
}