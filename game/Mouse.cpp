#include"Mouse.h"

//--------------------------------------------
//Class Mouse State
//--------------------------------------------
CMouseState::CMouseState() 
	:m_DownPos({ 0,0 }), m_UpPos({ 0,0 }),
	m_bDown(FALSE), m_bPress(FALSE), m_bUp(FALSE){}
CMouseState::~CMouseState() {}

bool CMouseState::IsClickEffective(RECT& rect)
{
	//Check If left Button Pressed
	if (m_bPress == TRUE)
	{
		//Check If Mouse Click Position In Rect
		if (Collide::RECTtoPoint(rect, m_DownPos))
		{
			//Check If Mouse Up
			if (m_bUp == TRUE)
			{
				//Check If Mouse Up Position In Rect
				if (Collide::RECTtoPoint(rect, m_UpPos))
					return TRUE;
			}
		}
	}
	return FALSE;
}

bool CMouseState::IsKeyDown() const 
{
	return m_bDown;
}

bool CMouseState::IsKeyUp() const 
{
	return m_bUp;
}
bool CMouseState::IsKeyPress() const 
{
	return m_bPress;
}


void CMouseState::KeyDown(POINT& pos)
{
	m_bDown = TRUE;
	//m_bPress = TRUE;
	m_DownPos = pos;
}

void CMouseState::KeyUp(POINT& pos)
{
	m_bUp = TRUE;
	//m_bPress = FALSE;
	m_UpPos = pos;
}

void CMouseState::ReSetState()
{
	if(m_bUp==TRUE)m_bPress = FALSE;
	if(m_bDown == TRUE)m_bPress = TRUE;

	m_bDown = FALSE;
	m_bUp = FALSE;
}


//--------------------------------------------
//Class Mouse 
//--------------------------------------------
CMouse::CMouse()
{
}
CMouse::~CMouse()
{
}

void CMouse::UpdataMouse(POINT GetPos)
{
	m_MousePos = GetPos;

	//reset State
	m_MLeft.ReSetState();
	m_MRight.ReSetState();
}

bool CMouse::IsLeftDown()
{
	return m_MLeft.IsKeyDown();
}
bool CMouse::IsLeftPress()
{
	return m_MLeft.IsKeyPress();
}

bool CMouse::IsLeftClickEffective(RECT& rect)
{
	return m_MLeft.IsClickEffective(rect);
}

void CMouse::LeftKeyDown()
{
	m_MLeft.KeyDown(m_MousePos);
}
void CMouse::LeftKeyUp()
{
	m_MLeft.KeyUp(m_MousePos);
}

void CMouse::RightKeyDown()
{
	m_MRight.KeyDown(m_MousePos);
}
void CMouse::RightKeyUp()
{
	m_MRight.KeyUp(m_MousePos);
}

POINT CMouse::GetMousePos()const
{
	return m_MousePos;
}