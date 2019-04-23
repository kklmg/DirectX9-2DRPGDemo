#pragma once
#include<Windows.h>
#include"collide.h"

class CMouseState
{
public:
	CMouseState();
	~CMouseState();

	bool IsClickEffective(RECT& rect);

	bool IsKeyDown() const;
	bool IsKeyUp() const;
	bool IsKeyPress() const;


	void KeyDown(POINT& pos);
	void KeyUp(POINT& pos);

	void ReSetState();

private:
	POINT m_DownPos;	//Mouse Down Position
	POINT m_UpPos;		//Mouse Up Position

	bool m_bDown;	//Is Mouse Down
	bool m_bPress;	//Is Mouse Press
	bool m_bUp;		//Is Mouse Up
};

class CMouse
{
public:
	CMouse();
	~CMouse();

	void UpdataMouse(POINT GetPos);

	bool IsLeftDown();
	bool IsLeftPress();

	bool IsLeftClickEffective(RECT& rect);

	void LeftKeyDown();
	void LeftKeyUp();

	void RightKeyDown();
	void RightKeyUp();

	POINT GetMousePos()const;

private:
	POINT m_MousePos;//Mouse Pos

	CMouseState m_MLeft;
	CMouseState m_MRight;
};
