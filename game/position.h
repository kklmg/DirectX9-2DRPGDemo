#pragma once
#include<d3d9.h>
#include<d3dx9.h>

class CPosition
{
public:
	CPosition(); 
	CPosition(float x, float y);
	virtual ~CPosition();

	void SetPosition(float x, float y);
	void SetPosition(D3DXVECTOR2 V2);
	void SetPosition(POINTFLOAT pos);

	void SetX(float x);
	void SetY(float y);

	float	 GetX(void) const;
	float	 GetY(void) const;

	POINTFLOAT	GetPosition(void) const;
	
	D3DXVECTOR2 GetVector(void) const;
	D3DXVECTOR2& GetVectorRef(void);

protected:
	D3DXVECTOR2 m_V2Pos;
};