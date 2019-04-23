#include"Camera.h"
#include"Global_data.h"

Camera::Camera()
{
	fCamera_x = 0;
	fCamera_y = 0;
}



bool Camera::focusCamera(float x, float y)
{	
	fCamera_x = x;
	fCamera_y = y;	
	return TRUE;

}



bool Camera::showCamera(LPDIRECTDRAWSURFACE7 suf, map *__map) {
	__map->DisMap_LT(suf, fCamera_x, fCamera_y);
	//__map->DisMap_LT(suf, fCamera_x, fCamera_y);
	return TRUE;

}

bool Camera::ReachEdgeLeft(map *__map) {

	if (__map->CornerLT.x>=0)return TRUE;
	else return FALSE;

}
bool Camera::ReachEdgeRight(map *__map) {

	if (__map->CornerRB.x <= HRES)return TRUE;
	else return FALSE;

}
bool Camera::ReachEdgeTop(map *__map) {

	if (__map->CornerLT.y >= 0)return TRUE;
	else return FALSE;

}
bool Camera::ReachEdgeBottom(map *__map) {

	if (__map->CornerRB.y <= VRES)return TRUE;
	else return FALSE;

}
bool Camera::moveCameraLeft() {
	
	fCamera_x++;
	return TRUE;
}
bool Camera::moveCameraRight() {

	fCamera_x--;
	return TRUE;
}
bool Camera::moveCameraUp() {

	fCamera_y++;
	return TRUE;
}
bool Camera::moveCameraDown() {

	fCamera_y--;
	return TRUE;
}
bool moveCamera()
{
	
	return TRUE;


}
bool Camera::operateCamera(map *__map)
{	//moveleft
		if (::GetKeyState(VK_NUMPAD4) & 0x80 && !ReachEdgeLeft(__map)) { moveCameraLeft(); };
		//moveright
		if (::GetKeyState(VK_NUMPAD6) & 0x80 && !ReachEdgeRight(__map)) { moveCameraRight(); };
		//moveup
		if (::GetKeyState(VK_NUMPAD8) & 0x80 && !ReachEdgeTop(__map))  { moveCameraUp(); };
		//movedown
		if (::GetKeyState(VK_NUMPAD2) & 0x80 && !ReachEdgeBottom(__map)) { moveCameraDown(); };
		//zoom in
		if (::GetKeyState(VK_ADD) & 0x80) {};
		//zoom out
		if (::GetKeyState(VK_SUBTRACT) & 0x80) {};
			
	return TRUE;
}

void Camera:: lockhero(CHA *_cha, map *__map)
{	//left top
	if (_cha->posCenter.x<HRES / 2&&_cha->posCenter.y<VRES / 2)
	{
		GameScreen->DisGameScreen(0, 0);

	}
	//right top
	else if (_cha->posCenter.x>__map->nWidth - HRES / 2 && _cha->posCenter.y<VRES / 2)
	{
		GameScreen->DisGameScreen(-__map->nWidth + HRES, 0);
	}
	//left bottom
	else if (_cha->posCenter.x<HRES / 2 && _cha->posCenter.y>__map->nHeight - VRES / 2)
	{
		GameScreen->DisGameScreen(0, -__map->nHeight + VRES);		
	}
	//right bottom
	else if (_cha->posCenter.x>__map->nWidth - HRES / 2 && _cha->posCenter.y>__map->nHeight - VRES / 2)
	{
		GameScreen->DisGameScreen(-__map->nWidth + HRES, -__map->nHeight + VRES);
	}
	// lock left
	else if (_cha->posCenter.x<HRES / 2)
	{
		GameScreen->DisGameScreen(0, (VRES / 2 - _cha->posCenter.y));
	}
	//lock right
	else if (_cha->posCenter.x>__map->nWidth-HRES/2)
	{
		GameScreen->DisGameScreen(-__map->nWidth + HRES, (VRES / 2 - _cha->posCenter.y));
	}
	//lock up
	else if (_cha->posCenter.y<VRES / 2)
	{		
		GameScreen->DisGameScreen(HRES / 2 - _cha->posCenter.x,0);
	}
	//lock down
	else if (_cha->posCenter.y>__map->nHeight - VRES / 2)
	{
		GameScreen->DisGameScreen(HRES / 2 - _cha->posCenter.x,-__map->nHeight + VRES);
	}
	//normal
	else 
	GameScreen->DisGameScreen((HRES / 2 - _cha->posCenter.x),(VRES / 2 - _cha->posCenter.y));
}
void Camera::lockhero(CHA2 *_cha, map *__map)
{	//left top
	if (_cha->posCenter.x<HRES / 2 && _cha->posCenter.y<VRES / 2)
	{
		__map->DisMap_LT(g_pDDSBack, 0, 0);
	}
	//right top
	else if (_cha->posCenter.x>__map->nWidth - HRES / 2 && _cha->posCenter.y<VRES / 2)
	{
		__map->DisMap_LT(g_pDDSBack, -__map->nWidth + HRES, 0);
	}
	//left bottom
	else if (_cha->posCenter.x<HRES / 2 && _cha->posCenter.y>__map->nHeight - VRES / 2)
	{
		__map->DisMap_LT(g_pDDSBack, 0, -__map->nHeight + VRES);
	}
	//right bottom
	else if (_cha->posCenter.x>__map->nWidth - HRES / 2 && _cha->posCenter.y>__map->nHeight - VRES / 2)
	{
		__map->DisMap_LT(g_pDDSBack, -__map->nWidth + HRES, -__map->nHeight + VRES);
	}
	// lock left
	else if (_cha->posCenter.x<HRES / 2)
	{
		__map->DisMap_LT(g_pDDSBack, 0, (VRES / 2 - _cha->posCenter.y));
	}
	//lock right
	else if (_cha->posCenter.x>__map->nWidth - HRES / 2)
	{
		__map->DisMap_LT(g_pDDSBack, -__map->nWidth + HRES, (VRES / 2 - _cha->posCenter.y));
	}
	//lock up
	else if (_cha->posCenter.y<VRES / 2)
	{
		__map->DisMap_LT(g_pDDSBack,
			HRES / 2 - _cha->posCenter.x,
			0);
	}
	//lock down
	else if (_cha->posCenter.y>__map->nHeight - VRES / 2)
	{
		__map->DisMap_LT(g_pDDSBack,
			HRES / 2 - _cha->posCenter.x,
			-__map->nHeight + VRES);
	}
	//normal
	else  __map->DisMap_LT(g_pDDSBack,
		(HRES / 2 + _cha->posCenter.x),
		(VRES / 2 + _cha->posCenter.y));
}