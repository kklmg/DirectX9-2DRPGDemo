#pragma once
#include"windows.h"
#include"ddraw.h"
#include"TileMap.h"
#include"LivingObjectClass.h"
#include"BuildingClass.h"
#include"ObjectClass.h"
#include"MyStruct.h"
#include"CollideArea.h"

typedef struct STCollideData
{
	bool IsCollide = FALSE;
	short direction;
	short detailDir;
	int OverlapX;
	int OverlapY;

	D3DXVECTOR2 V2Offset;

} CollideInfo;


float CalculateDistance(POINTFLOAT p1,POINTFLOAT p2);

DWORD CalculateDistance(POINT p1, POINT p2);

namespace Collide
{
	//////////////////
	//Basic Collide//
	/////////////////
	bool RECTtoPoint(RECT& rect1, POINT& point);

	//RECT to RECT collide____________________________
	bool RECTtoRECT(RECT rect1, RECT rect2);

	bool RECTtoRECT_Offset(RECT rect1, RECT rect2,float offset);

	bool IsRectIntersect(RECT rect_main, RECT rect_sub,float percentage);

	STCollideData RECTtoRECT_EX(RECT rect1, RECT rect2);

	bool IsRectInvolvedinRect(RECT outrect, RECT inrect);


	bool CircleToPoint(_CIRCLE circle, POINTFLOAT point);

	//Circle to Circle collide_________________________
	bool CircletoCircle(_CIRCLE circleA, _CIRCLE circleB);

	STCollideData CircletoCircleEX(_CIRCLE circleA, _CIRCLE circleB);

	int Getr(_CIRCLE circleA, _CIRCLE circleB);

	//Rect to Circle collide____________________________ 
	bool CircleToRect(_CIRCLE circle,RECT rect);

	bool IsCircleInvolvedinRect(_CIRCLE circle, RECT rect);

	bool IsRectInvolvedinCircle(POINT point, float radius, RECT rect);

	///////////////////////
	//Area Group collide//
	//////////////////////
	bool AreAreaGroupCollideWithPoint(CCollideArea *area, POINT point);

	bool AreAreaGroupCollideWithRect(CCollideArea *area, RECT rect);

	bool AreAreaGroupCollideWithCircle(CCollideArea *area, _CIRCLE rect);

	bool AreAreaGroupInvolvedInRect(CCollideArea *area,RECT rect);

	STCollideData CheckTwoAreaCollision(CCollideArea *area1, CCollideArea *area2);
	
	//////////////////
	//Object Collide//
	/////////////////

	//point
	bool IsObjectCollideAreaCollideWithPoint(CObject *obj, POINT rect);
	//Rect
	bool IsObjectCollideAreaCollideWithRect(CObject *obj, RECT rect);

	bool IsObjectBeAttackAreaCollideWithRect(CLivingObj *obj, RECT rect);

	bool IsObjectCollideAreaInvolvedInRect(CObject *obj, RECT rect);

	bool IsObjectBeAttackAreaInvolvedInRect(CLivingObj *obj, RECT rect);

	//circle
	bool IsObjectCollideAreaCollideWithCircle(CObject *obj, _CIRCLE rect);

	bool IsObjectBeAttackAreaCollideWithCircle(CLivingObj *obj, _CIRCLE rect);

	//are two object collide
	bool AreTwoObjCollide(CObject *obj1, CObject *obj2);

	bool PerformTwoObjectsCollision(CObject *obj1, CObject *obj2);

	bool DealWithCollision(CObject *obj1, CObject *obj2, STCollideData CollideData);
	//Check If collide Area involved In rect



	






	//collide with MAP EDGE__________________________________	
	bool ReachMapEdge(CLivingObj *lifeobj, CTileMap *_map);
	bool ReachMapEdgeLeft(CObject *obj, CTileMap *_map);
	bool ReachMapEdgeRight(CObject *obj, CTileMap *_map);
	bool ReachMapEdgeTop(CObject *obj, CTileMap *_map);
	bool ReachMapEdgeBottom(CObject *obj, CTileMap *_map);

	bool AvoidCollideWithMapEdge(CObject *obj, CTileMap *_map);

	//are two object collide
	bool AreTwoObjCollide(CObject *obj1, CObject *obj2); 

	//collide with Tile_________________________________________
	void AvoidcollideTile(CTileMap*_map, CObject *obj);

	//collide action____________________________________________
	bool AvoidCollide(CLivingObj *lifeobj,CBuilding *building);

	bool AdjustPosition(CObject *obj, STCollideData ColInfo);

	bool AdjustPosition(POINT &point, STCollideData ColInfo);

	bool AdjustPosition(RECT &rect, STCollideData ColInfo);




//	bool Collide_UNIT(UNIT *_unit);
//	STCollideData Collide_UNIT_EX(UNIT *_unit);
//	bool AvoidCollide_UNIT(UNIT *_unit);

	//bool Unit_Hit(CObject *_unit, short _nUniType);
	
	//bool AdjustDirection(Monster *_mon, STCollideData ColInfo);

	

}
