#include"collide.h"
#include"RectMath.h"
#include"EnumGroup.h"

//Calculate Distance
float CalculateDistance(POINTFLOAT p1, POINTFLOAT p2)
{
	float x = p1.x - p2.x;
	float y = p1.y - p2.y;

	return sqrt(x*x + y*y);
}

DWORD CalculateDistance(POINT p1, POINT p2)
{
	float x = p1.x - p2.x;
	float y = p1.y - p2.y;

	return sqrt(x*x + y*y);
}

bool Collide::RECTtoPoint(RECT& rect, POINT& point)
{
	if(	  point.x > rect.left
		&&point.x < rect.right
		&&point.y>rect.top
		&&point.y < rect.bottom)
		return TRUE;
	else return FALSE;
}


/////////////////
//rect collide//
////////////////
bool Collide::RECTtoRECT(RECT rect1, RECT rect2)
{
	POINT rc1 = GetRectCenter(rect1);
	POINT rc2 = GetRectCenter(rect2);


	int width1 = GetRectWidth(rect1);
	int width2 = GetRectWidth(rect2);
	int height1 = GetRectHeight(rect1);
	int height2 = GetRectHeight(rect2);

	if (abs(rc1.x - rc2.x) <= (width1 / 2 + width2 / 2)
		&& abs(rc1.y - rc2.y) <= (height1 / 2 + height2 / 2))
		return TRUE;
	else return FALSE;

}
//
bool Collide::RECTtoRECT_Offset(RECT rect1, RECT rect2, float offset)
{
	POINT rc1 = GetRectCenter(rect1);
	POINT rc2 = GetRectCenter(rect2);


	int width1 = GetRectWidth(rect1);
	int width2 = GetRectWidth(rect2);
	int height1 = GetRectHeight(rect1);
	int height2 = GetRectHeight(rect2);

	if (abs(rc1.x - rc2.x) <= (width1 / 2 + width2 / 2) + offset
		&& abs(rc1.y - rc2.y) <= (height1 / 2 + height2 / 2) + offset)
		return TRUE;
	else return FALSE;
}

bool Collide::IsRectIntersect(RECT rect_main, RECT rect_sub, float percentage)
{
	if (RECTtoRECT(rect_main, rect_sub))
	{
		int x = GetRectWidth(rect_main) - GetRectWidth(rect_main);
		int y = GetRectHeight(rect_main)- GetRectHeight(rect_sub);

		int area_intersect = abs(x)*abs(y);
		int area_main= GetRectWidth(rect_main) * GetRectHeight(rect_main);

		float percent = (float)area_intersect / (float)area_main;
	
		if (percent >= percentage)return TRUE;
		else return FALSE;
	}
	else return FALSE;
}

STCollideData Collide::RECTtoRECT_EX(RECT rect1, RECT rect2)
{
	STCollideData ColInfo;

	//Get rect center
	POINT rc1 = GetRectCenter(rect1);
	POINT rc2 = GetRectCenter(rect2);

	//Get rect size
	int width1 = GetRectWidth(rect1);
	int width2 = GetRectWidth(rect2);
	int height1 = GetRectHeight(rect1);
	int height2 = GetRectHeight(rect2);

	//distance between two rect center
	int vx = rc1.x - rc2.x;
	int vy = rc1.y - rc2.y;


	//check collision
	if (abs(vx) < (width1 / 2 + width2 / 2)
		&& abs(vy) < (height1 / 2 + height2 / 2)) //check if rect1 collide with rect2
	{
		ColInfo.IsCollide = TRUE;

		//Get OverLap Data
		ColInfo.OverlapX = (width1 / 2 + width2 / 2) - abs(vx);
		ColInfo.OverlapY = (height1 / 2 + height2 / 2) - abs(vy);
	
		//figure out the place collision happend
		if (ColInfo.OverlapX >= ColInfo.OverlapY) // x�࿡�� �浹 �߻� 
		{
			if (vy > 0) //top
			{
				ColInfo.direction = eUp;
				if (rc1.x>rc2.x)
					ColInfo.detailDir = eRight;
				else ColInfo.detailDir = eLeft;
			}
			else if (vy < 0)//bottom		
			{
				ColInfo.direction = eDown;
				if (rc1.x>rc2.x)
					ColInfo.detailDir = eRight;
				else ColInfo.detailDir = eLeft;
			}
		}
		else // y�࿡�� �浹 �߻� 
		{
			if (vx > 0) //left
			{
				ColInfo.direction = eLeft;
				if (rc1.y > rc2.y)
					ColInfo.detailDir = eDown;
				else ColInfo.detailDir = eUp;
			}
			else if (vx < 0)//right
			{
				ColInfo.direction = eRight;
				if (rc1.y > rc2.y)
					ColInfo.detailDir = eDown;
				else ColInfo.detailDir = eUp;
			}

		}
	}
	else ColInfo.IsCollide = FALSE;

	return ColInfo;
}


bool Collide::CircleToPoint(_CIRCLE circle, POINTFLOAT point)
{
	float Distance= CalculateDistance(point, circle.point);
	
	if (Distance > circle.radius)	return FALSE;
	else return TRUE;
}

bool Collide::CircleToRect(_CIRCLE circle, RECT rect)
{
	if (CalculateDistance(circle.point, { (float)rect.left,(float)rect.top }) < circle.radius
		|| CalculateDistance(circle.point, { (float)rect.right,(float)rect.top }) < circle.radius
		|| CalculateDistance(circle.point, { (float)rect.left,(float)rect.bottom }) < circle.radius
		|| CalculateDistance(circle.point, { (float)rect.right,(float)rect.bottom }) < circle.radius)
		return TRUE;
	else return FALSE;
}




bool Collide::IsRectInvolvedinRect(RECT outrect, RECT inrect)
{
	if (outrect.left <= inrect.left&&
		outrect.right >= inrect.right&&
		outrect.top <= inrect.top&&
		outrect.bottom >= inrect.bottom)
		return TRUE;
	else return FALSE;
}

///////////////////
//Circle collide//
//////////////////

bool Collide::CircletoCircle(_CIRCLE circleA, _CIRCLE circleB)
{
	DWORD Distance = CalculateDistance(circleA.point, circleB.point);

	if (Distance < circleA.radius + circleB.radius)return TRUE;
	else return FALSE;
}

STCollideData CircletoCircleEX(_CIRCLE circleA, _CIRCLE circleB) 
{
	STCollideData colData;


	
	return colData;

}





//////////////////////////
//Rect to Circle collide//
/////////////////////////
bool Collide::IsCircleInvolvedinRect(_CIRCLE circle, RECT rect)
{
	if (rect.left<(circle.point.x - circle.radius) &&
		rect.right>(circle.point.x + circle.radius) &&
		rect.top > (circle.point.y - circle.radius) &&
		rect.bottom > (circle.point.y + circle.radius))
		return TRUE;
	else return FALSE;
}

bool Collide::IsRectInvolvedinCircle(POINT point, float radius, RECT rect)
{
	return FALSE;
}

bool Collide::AvoidCollideWithMapEdge(CObject *obj, CTileMap *_map)
{
	RECT ObjRect = obj->GetDrawRect();

	RECT MapRect = _map->GetMapRect();

	//left wall collide
	if (ObjRect.left < MapRect.left)
	{
		//Figure Out Intercect Length
		float offset = MapRect.left - ObjRect.left;

		//Adjust position to avoid collision
		obj->SetX(obj->GetX()+ offset);

		//mark collision
		obj->MarkHasBeenCollide();
	}

	//top wall collide
	if (ObjRect.top < MapRect.top)
	{
		//Figure Out Intercect Length
		float offset = MapRect.top - ObjRect.top;

		//Adjust position to avoid collision
		obj->SetY(obj->GetY() + offset);

		//mark collision
		obj->MarkHasBeenCollide();
	}

	//right wall collide
	if (ObjRect.right > MapRect.right)
	{
		//Figure Out Intercect Length
		float offset = ObjRect.right - MapRect.right;

		//Adjust position to avoid collision
		obj->SetX(obj->GetX() - offset);

		//mark collision
		obj->MarkHasBeenCollide();
	}

	//bottom wall collide
	if (ObjRect.bottom > MapRect.bottom)
	{
		//Figure Out Intercect Length
		float offset = ObjRect.bottom - MapRect.bottom;

		//Adjust position to avoid collision
		obj->SetY(obj->GetY() - offset);

		//mark collision
		obj->MarkHasBeenCollide();
	}
	return TRUE;
}

//bool Collide::AreTwoObjCollide(CObject *obj1, CObject *obj2)
//{
//	return RECTtoRECT_Offset(obj1->GetCollideRect(), obj2->GetCollideRect(),2);
//}



////////////////////////////
//Object Collide With Map//
///////////////////////////
bool Collide::ReachMapEdge(CLivingObj *lifeobj, CTileMap *_map)
{	
	RECT rect = lifeobj->GetDrawRect();
	RECT MapRect = _map->GetMapRect();

	if (rect.left <= MapRect.left) return TRUE;
	if (rect.right >= MapRect.right) return TRUE;
	if (rect.top <= MapRect.top) return TRUE;
	if (rect.bottom >= MapRect.bottom) return TRUE;

	return FALSE;
}


///////////////////////
//Area Group collide//
//////////////////////
bool Collide::AreAreaGroupCollideWithPoint(CCollideArea *area, POINT point) 
{
	if (area == NULL)return FALSE;

	DWORD RectCount = area->GetRectCount();

	DWORD CircleCount = area->GetCircleCount();


	//check if rect collide with rect

	for (int i = 0; i < RectCount; i++)
	{
		if (RECTtoPoint(area->GetRect(i), point))
		{
			return TRUE;
		}
	}

	//check if circle collide with rect	
	for (int i = 0; i < CircleCount; i++)
	{
		if (CircleToPoint(area->GetCircle(i), {(float)point.x, (float)point.y}))
		{
			return TRUE;
		}
	}

	return FALSE;

}

bool Collide::AreAreaGroupCollideWithRect(CCollideArea *area, RECT rect) 
{
	if (area == NULL)return FALSE;

	DWORD RectCount = area->GetRectCount();

	DWORD CircleCount = area->GetCircleCount();


	//check if rect collide with rect
	
	for (int i = 0; i < RectCount; i++)
	{
		if (RECTtoRECT(rect, area->GetRect(i)))
		{
			return TRUE;
		}
	}
		
	//check if circle collide with rect	
	for (int i = 0; i < CircleCount; i++)
	{
		if (CircleToRect(area->GetCircle(i), rect))
		{
			return TRUE;
		}
	}

	return FALSE;
}

bool Collide::AreAreaGroupCollideWithCircle(CCollideArea *area, _CIRCLE circle)
{
	if (area == NULL)return FALSE;

	DWORD RectCount = area->GetRectCount();

	DWORD CircleCount = area->GetCircleCount();

		//check if rect collide with rect
		for (int i = 0; i < RectCount; i++)
		{ 
			if (CircleToRect(circle, area->GetRect(i)))
			{
				return TRUE;
			}
		}
		//check if circle collide with circle	
		for (int i = 0; i < CircleCount; i++)
		{
			if (CircletoCircle(area->GetCircle(i), circle))
			{
				return TRUE;
			}
		}
		
	return FALSE;
}

bool Collide::AreAreaGroupInvolvedInRect(CCollideArea *area, RECT rect)
{
	if (area == NULL)return FALSE;

	DWORD RectCount = area->GetRectCount();

	DWORD CircleCount = area->GetCircleCount();


	//check if rect involved in rect		
	for (int i = 0; i < RectCount; i++) 
	{		
		if (!IsRectInvolvedinRect(rect, area->GetRect(i)))return FALSE;
	}
		

	//check if circle involved in rect	
	for (int i = 0; i < CircleCount; i++)
	{
		if (!IsCircleInvolvedinRect(area->GetCircle(i), rect))return FALSE;
	}

	return TRUE;
}

STCollideData Collide::CheckTwoAreaCollision(CCollideArea *Area_A, CCollideArea *Area_B)
{
	STCollideData ColData;

	if (Area_A == NULL || Area_B == NULL) return ColData;

	if (Area_A->GetCollideAreaCount() == 0 || Area_B->GetCollideAreaCount() == 0) return ColData;

	if (Area_A->IsActiveCollide() == FALSE || Area_B->IsActiveCollide() == FALSE) return ColData;

	enPhysicalStrength ColLevel_A = Area_A->GetPhyStrength();
	enPhysicalStrength ColLevel_B = Area_A->GetPhyStrength();

	if (ColLevel_A == enPhysicalStrength::epyNoCollide
		|| ColLevel_B == enPhysicalStrength::epyNoCollide) return ColData;

	if (ColLevel_A == enPhysicalStrength::epyFixed
		|| ColLevel_B == enPhysicalStrength::epyFixed) return ColData;


	//get collide area count
	DWORD RectCount_A = Area_A->GetRectCount();
	DWORD RectCount_B = Area_B->GetRectCount();

	DWORD CircleCount_A = Area_A->GetCircleCount();
	DWORD CircleCount_B = Area_B->GetCircleCount();

	
	////////////////////////////
	//Check A rect with B Area//
	////////////////////////////

	for (int i = 0; i < RectCount_A; i++)
	{
		//check A rect with B rect
		for (int j = 0; j < RectCount_B; j++)
		{
			ColData = RECTtoRECT_EX(Area_A->GetRect(i), Area_B->GetRect(j));

			if(Area_A->GetPhyStrength())


			if (ColData.IsCollide == TRUE)return ColData;
		}

		//Check A rect Wirh B Circle
		for (int k = 0; k < CircleCount_B; k++) 
		{
			//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
			bool collide = CircleToRect(Area_B->GetCircle(k), Area_A->GetRect(i));
			if (collide) 
			{
				ColData.IsCollide = TRUE;
				return ColData;
			}
		}
	}
	
	///////////////////////////////
	//Check A Circle with B Area//
	//////////////////////////////

	for (int i = 0; i < CircleCount_A; i++)
	{
		//check A Circle with B rect
		for (int j = 0; j < RectCount_B; j++)
		{
			//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx Uncomplete
			bool collide = CircleToRect(Area_A->GetCircle(i), Area_B->GetRect(j));
			if (collide)
			{
				ColData.IsCollide = TRUE;
				return ColData;
			}
		}

		//Check A Circle Wirh B Circle
		for (int k = 0; k < CircleCount_B; k++)
		{
			//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx Uncomplete			
			bool collide = CircletoCircle(Area_A->GetCircle(i), Area_B->GetCircle(k));
			if (collide)
			{
				ColData.IsCollide = TRUE;
				return ColData;
			}
		}
	}

	return ColData;
}



//////////////////
//Object Collide//
/////////////////
//collide with point
bool Collide::IsObjectCollideAreaCollideWithPoint(CObject *obj, POINT point)
{
	if (obj == NULL)return FALSE;
	if (obj->GetCollideArea() == NULL)return FALSE;

	return AreAreaGroupCollideWithPoint(obj->GetCollideArea(),point);
}

//collide with rect
bool Collide::IsObjectCollideAreaCollideWithRect(CObject *obj, RECT rect) 
{
	if (obj == NULL)return FALSE;
	if (obj->GetCollideArea() == NULL)return FALSE;

	return AreAreaGroupCollideWithRect(obj->GetCollideArea(), rect);
}

bool Collide::IsObjectBeAttackAreaCollideWithRect(CLivingObj *obj, RECT rect)
{
	if (obj == NULL)return FALSE;
	if (obj->GetBeAttackArea() == NULL)return FALSE;

	return AreAreaGroupCollideWithRect(obj->GetBeAttackArea(), rect);
}

bool Collide::IsObjectCollideAreaInvolvedInRect(CObject *obj, RECT rect) 
{
	if (obj == NULL)return FALSE;
	if (obj->GetCollideArea() == NULL)return FALSE;
		
	return AreAreaGroupInvolvedInRect(obj->GetCollideArea(), rect);
}

bool Collide::IsObjectBeAttackAreaInvolvedInRect(CLivingObj *obj, RECT rect)
{
	if (obj == NULL)return FALSE;
	if (obj->GetBeAttackArea() == NULL)return FALSE;

	return AreAreaGroupInvolvedInRect(obj->GetBeAttackArea(), rect);
}


//circle
bool Collide::IsObjectCollideAreaCollideWithCircle(CObject *obj, _CIRCLE circle)
{
	if (obj == NULL)return FALSE;
	if (obj->GetCollideArea() == NULL)return FALSE;

	return (AreAreaGroupCollideWithCircle(obj->GetCollideArea(), circle));
}

bool Collide::IsObjectBeAttackAreaCollideWithCircle(CLivingObj *obj, _CIRCLE circle)
{
	if (obj == NULL)return FALSE;
	if (obj->GetCollideArea() == NULL)return FALSE;

	return (AreAreaGroupCollideWithCircle(obj->GetBeAttackArea(), circle));
}

bool Collide::AreTwoObjCollide(CObject *obj1, CObject *obj2) 
{
	if (obj1 == NULL || obj2 == NULL)return FALSE;
	STCollideData CollideData;

	//get collide area data
	CCollideArea *Area_A = obj1->GetCollideArea();
	CCollideArea *Area_B = obj2->GetCollideArea();

	if (Area_A == NULL || Area_B == NULL)return FALSE;

	CollideData = CheckTwoAreaCollision(Area_A, Area_B);

	return CollideData.IsCollide;
}

//ultimate collision function
bool Collide::PerformTwoObjectsCollision(CObject *obj1, CObject *obj2) 
{
	if (obj1 == NULL || obj2 == NULL)return FALSE;

	enObjectForce ObjForce_A = obj1->GetObjForce();
	enObjectForce ObjForce_B = obj2->GetObjForce();

	//special 
	switch (ObjForce_A)
	{
		case enObjectForce::eForFriend: 
		{
			if (ObjForce_B == enObjectForce::eForFriend)return FALSE;
			
		}break;
		case enObjectForce::eForHostile:
		{	
			if (ObjForce_B == enObjectForce::eForHostile)return FALSE;
		}break;

		default:
		break;
	}

	//Get collide area data
	CCollideArea *Area_A = obj1->GetCollideArea();
	CCollideArea *Area_B = obj2->GetCollideArea();

	if (Area_A == NULL || Area_B == NULL) return FALSE;
	if (Area_A->IsActiveCollide() == FALSE || Area_B->IsActiveCollide() == FALSE) return FALSE;


	//deal with phy level
	enPhysicalStrength PhyLevel_A = Area_A->GetPhyStrength();
	enPhysicalStrength PhyLevel_B = Area_A->GetPhyStrength();

	if (PhyLevel_A == enPhysicalStrength::epyNoCollide
		|| PhyLevel_B == enPhysicalStrength::epyNoCollide) return FALSE;

	if (PhyLevel_A == enPhysicalStrength::epyFixed
		&& PhyLevel_B == enPhysicalStrength::epyFixed) return FALSE;


	//get collide area count
	DWORD RectCount_A = Area_A->GetRectCount();
	DWORD RectCount_B = Area_B->GetRectCount();

	DWORD CircleCount_A = Area_A->GetCircleCount();
	DWORD CircleCount_B = Area_B->GetCircleCount();


	////////////////////////////
	//Check A rect with B Area//
	////////////////////////////

	for (int i = 0; i < RectCount_A; i++)
	{
		//check A rect with B rect
		for (int j = 0; j < RectCount_B; j++)
		{
			STCollideData ColData= RECTtoRECT_EX(Area_A->GetRect(i), Area_B->GetRect(j));
			if (ColData.IsCollide == TRUE) 
			{
				//deal with collide
				DealWithCollision(obj1, obj2, ColData);	

				//Mark Collision
				obj1->MarkHasBeenCollide();
				obj2->MarkHasBeenCollide();
			}
		}

		//Check A rect Wirh B Circle
		for (int k = 0; k < CircleCount_B; k++)
		{
			//Uncomplete !!!!!!!!!!!!!!!!!!!!!!
			bool collide = CircleToRect(Area_B->GetCircle(k), Area_A->GetRect(i));
			if (collide)
			{
				//ColData.IsCollide = TRUE;
				//return ColData;
			}
		}
	}

	///////////////////////////////
	//Check A Circle with B Area//
	//////////////////////////////

	for (int i = 0; i < CircleCount_A; i++)
	{
		//check A Circle with B rect
		for (int j = 0; j < RectCount_B; j++)
		{
			//Uncomplete !!!!!!!!!!!!!!!!!!!!!!
			bool collide = CircleToRect(Area_A->GetCircle(i), Area_B->GetRect(j));
			if (collide)
			{
				//ColData.IsCollide = TRUE;
				//return ColData;
			}
		}

		//Check A Circle Wirh B Circle
		for (int k = 0; k < CircleCount_B; k++)
		{
			//Uncomplete !!!!!!!!!!!!!!!!!!!!!!	
			bool collide = CircletoCircle(Area_A->GetCircle(i), Area_B->GetCircle(k));
			if (collide)
			{
				//ColData.IsCollide = TRUE;
				//return ColData;
			}
		}
	}


	return FALSE;
}

bool Collide::DealWithCollision(CObject *obj1, CObject *obj2, STCollideData CollideData) 
{
	if (obj1 == NULL)return FALSE;
	if (obj2 == NULL)return FALSE;

	if(CollideData.IsCollide == FALSE) return TRUE;

	enPhysicalStrength phyLevel_A = obj1->GetCollideArea()->GetPhyStrength();
	enPhysicalStrength phyLevel_B = obj2->GetCollideArea()->GetPhyStrength();

	if (phyLevel_A == enPhysicalStrength::epyNoCollide
		|| phyLevel_B == enPhysicalStrength::epyNoCollide) return TRUE;

	if (phyLevel_A == enPhysicalStrength::epyFixed
		&& phyLevel_B == enPhysicalStrength::epyFixed) return TRUE;


	//collision happened in rect top area
	if (CollideData.direction == eDown) //top
	{
		if (phyLevel_A == enPhysicalStrength::epyFixed
			|| phyLevel_B == enPhysicalStrength::epyFixed) 
		{
			if (phyLevel_A == enPhysicalStrength::epyFixed) 
			{
				obj2->SetY(obj2->GetY() + CollideData.OverlapY);
			}
			if(phyLevel_B == enPhysicalStrength::epyFixed)
			{				
				obj1->SetY(obj1->GetY() - CollideData.OverlapY);
			}
		}
		else if (phyLevel_A < phyLevel_B) 
		{	
			obj1->SetY(obj1->GetY() - CollideData.OverlapY);
		}
		else if (phyLevel_A > phyLevel_B)
		{
			obj2->SetY(obj2->GetY() + CollideData.OverlapY);
		}
		else if (phyLevel_A == phyLevel_B)
		{
			obj1->SetY(obj1->GetY() - CollideData.OverlapY/2);
			obj2->SetY(obj2->GetY() + CollideData.OverlapY/2);
		}
	}

	else if (CollideData.direction == eUp) //top
	{
		if (phyLevel_A == enPhysicalStrength::epyFixed
			|| phyLevel_B == enPhysicalStrength::epyFixed)
		{
			if (phyLevel_A == enPhysicalStrength::epyFixed)
			{
				obj2->SetY(obj1->GetY() - CollideData.OverlapY);
			}
			if (phyLevel_B == enPhysicalStrength::epyFixed)
			{
				obj1->SetY(obj1->GetY() + CollideData.OverlapY);
			}
		}
		else if (phyLevel_A < phyLevel_B)
		{
			obj1->SetY(obj1->GetY() + CollideData.OverlapY);
		}
		else if (phyLevel_A > phyLevel_B)
		{
			obj2->SetY(obj2->GetY() - CollideData.OverlapY);
		}
		else if (phyLevel_A == phyLevel_B)
		{
			obj1->SetY(obj1->GetY() + CollideData.OverlapY / 2);
			obj2->SetY(obj2->GetY() - CollideData.OverlapY / 2);
		}
	}

	else if (CollideData.direction == eLeft) //top
	{
		if (phyLevel_A == enPhysicalStrength::epyFixed
			|| phyLevel_B == enPhysicalStrength::epyFixed)
		{
			if (phyLevel_A == enPhysicalStrength::epyFixed)
			{			
				obj2->SetX(obj2->GetX() - CollideData.OverlapX);
			}
			if (phyLevel_B == enPhysicalStrength::epyFixed)
			{
				obj1->SetX(obj1->GetX() + CollideData.OverlapX);
			}
		}
		else if (phyLevel_A < phyLevel_B)
		{
			obj1->SetX(obj1->GetX() + CollideData.OverlapX);
		}
		else if (phyLevel_A > phyLevel_B)
		{
			obj2->SetX(obj2->GetX() - CollideData.OverlapX);
		}
		else if (phyLevel_A == phyLevel_B)
		{
			obj1->SetX(obj1->GetX() + CollideData.OverlapX/2);
			obj2->SetX(obj2->GetX() - CollideData.OverlapX/2);
		}
	}

	else if (CollideData.direction == eRight) //top
	{
		if (phyLevel_A == enPhysicalStrength::epyFixed
			|| phyLevel_B == enPhysicalStrength::epyFixed)
		{
			if (phyLevel_A == enPhysicalStrength::epyFixed)
			{
				obj2->SetX(obj2->GetX() + CollideData.OverlapX);
			}
			if (phyLevel_B == enPhysicalStrength::epyFixed)
			{
				obj1->SetX(obj1->GetX() - CollideData.OverlapX);
			}
		}
		else if (phyLevel_A < phyLevel_B)
		{
			obj1->SetX(obj1->GetX() - CollideData.OverlapX);
		}
		else if (phyLevel_A > phyLevel_B)
		{
			obj2->SetX(obj2->GetX() + CollideData.OverlapX);
		}
		else if (phyLevel_A == phyLevel_B)
		{
			obj1->SetX(obj1->GetX() - CollideData.OverlapX/2);
			obj2->SetX(obj2->GetX() + CollideData.OverlapX/2);
		}
	}
	
	return TRUE;
}







//bool Collide::ReachMapEdgeLeft(CObject *obj, CTileMap *_map)
//{
//	if (_unit->posArea.left <= _map->m_ptCornerLT.x) return TRUE;
//	else return FALSE;
//}
//bool Collide::ReachMapEdgeRight(CObject *obj, CTileMap *_map)
//{
//	if (_unit->posArea.right >= _map->m_ptCornerRB.x) return TRUE;
//	return FALSE;
//}
//bool Collide::ReachMapEdgeTop(CObject *obj, CTileMap *_map)
//{
//	if (_unit->posArea.top <= _map->m_ptCornerLT.y) return TRUE;
//	return FALSE;
//}
//bool Collide::ReachMapEdgeBottom(CObject *obj, CTileMap *_map)
//{
//	if (_unit->posArea.bottom >= _map->m_ptCornerRB.y) return TRUE;
//	return FALSE;
//}

//////////////////
//Collide Action//
//////////////////

bool Collide::AdjustPosition(POINT &point, STCollideData ColInfo)
{
	if (ColInfo.IsCollide == FALSE)return FALSE;
	if (ColInfo.IsCollide == TRUE) 
	{

		if (ColInfo.direction == eUp) //top
		{
			point.y += ColInfo.OverlapY;
		}
		else if (ColInfo.direction == eDown)//bottom
		{
			point.y -= ColInfo.OverlapY;			
		}
		else if (ColInfo.direction == eLeft) //left
		{
			point.x += ColInfo.OverlapX;			
		}
		else if (ColInfo.direction == eRight)//right
		{
			point.x -= ColInfo.OverlapX;
		}
	}


	return TRUE;

}

bool Collide::AdjustPosition(CObject *obj, STCollideData ColInfo)
{
	if (ColInfo.IsCollide == FALSE)return FALSE;
	if (ColInfo.IsCollide == TRUE)
	{

		if (ColInfo.direction == eUp) //top
		{
			obj->SetY(obj->GetY() + ColInfo.OverlapY);		
		}
		else if (ColInfo.direction == eDown)//bottom
		{
			obj->SetY(obj->GetY() - ColInfo.OverlapY);
		}
		else if (ColInfo.direction == eLeft) //left
		{
			obj->SetX(obj->GetX() + ColInfo.OverlapX);		
		}
		else if (ColInfo.direction == eRight)//right
		{
			obj->SetX(obj->GetX() - ColInfo.OverlapX);
		}
	}
	return TRUE;
}


bool Collide::AdjustPosition(RECT &rect, STCollideData ColInfo)
{
	if (ColInfo.IsCollide == FALSE)return FALSE;
	if (ColInfo.IsCollide == TRUE)
	{

		if (ColInfo.direction == eUp) //top
		{
			rect.top += ColInfo.OverlapY;
			rect.bottom += ColInfo.OverlapY;
		}
		else if (ColInfo.direction == eDown)//bottom
		{
			rect.top -= ColInfo.OverlapY;
			rect.bottom -= ColInfo.OverlapY;
		}
		else if (ColInfo.direction == eLeft) //left
		{
			rect.left += ColInfo.OverlapX;
			rect.right += ColInfo.OverlapX;
		}
		else if (ColInfo.direction == eRight)//right
		{
			rect.left -= ColInfo.OverlapX;
			rect.right -= ColInfo.OverlapX;
		}
	}
	return TRUE;
}
	
//bool Collide::Collide_UNIT(UNIT *_unit)
//{
//	list<UNIT*>::iterator iter;
//
//	for (iter = UNITlist.begin();
//		iter != UNITlist.end(); ++iter)
//	{
//		if ((*iter) == _unit) continue;
//		if (RECTtoRECT(_unit->CollideArea, (*iter)->CollideArea))
//			return TRUE;
//	}
//
//	return FALSE;
//}
//STCollideData Collide::Collide_UNIT_EX(UNIT *_unit)
//{
//	list<UNIT*>::iterator iter;
//	STCollideData colInfo;
//
//	for (iter = UNITlist.begin();
//		iter != UNITlist.end(); ++iter)
//	{
//		if ((*iter) == _unit) continue;
//		colInfo=RECTtoRECT_EX(_unit->CollideArea, (*iter)->CollideArea);
//		if (colInfo.IsCollide == TRUE)return colInfo;		
//	}
//
//	return  colInfo;
//}
//bool Collide::AvoidCollide_UNIT(UNIT *_unit)
//{
//	list<UNIT*>::iterator iter;
//	CollideInfo ColInfo;
//
//	for (iter = UNITlist.begin();
//		iter != UNITlist.end(); ++iter)
//	{
//		if ((*iter) == _unit) continue;
//		ColInfo = RECTtoRECT_EX(_unit->CollideArea, (*iter)->CollideArea);
//		AdjustPosition(_unit->posCenter, ColInfo);
//	}	
//	return TRUE;
//}













/////bool Collide::AdjustDirection(Monster *_mon, STCollideData ColInfo)
////{
////	if (ColInfo.IsCollide == FALSE)return FALSE;
////	if (ColInfo.IsCollide == TRUE) // �浹 �߻� 
////	{
////		if (ColInfo.direction == dirUp&&ColInfo.detailDir == dirRight)
////			_mon->ndir = dirRight;
////		else if (ColInfo.direction == dirUp&&ColInfo.detailDir == dirLeft)
////			_mon->ndir = dirLeft;
////		else if (ColInfo.direction == dirRight&&ColInfo.detailDir == dirUp)
////			_mon->ndir = dirUp;
////		else if (ColInfo.direction == dirRight&&ColInfo.detailDir == dirDown)
////			_mon->ndir = dirDown;
////		else if (ColInfo.direction == dirDown&&ColInfo.detailDir == dirLeft)
////			_mon->ndir = dirLeft;
////		else if (ColInfo.direction == dirDown&&ColInfo.detailDir == dirRight)
////			_mon->ndir = dirRight;
////		else if (ColInfo.direction == dirLeft&&ColInfo.detailDir == dirUp)
////			_mon->ndir = dirUp;
////		else if (ColInfo.direction == dirLeft&&ColInfo.detailDir == dirDown)
////			_mon->ndir = dirDown;
////	}
////	return TRUE;
////
////}
//
//
//
////
////bool Collide::Collide_UNIT(UNIT *_unit)
////{
////	list<UNIT*>::iterator iter;
////
////	for (iter = UNITlist.begin();
////		iter != UNITlist.end(); ++iter)
////	{
////		if ((*iter) == _unit) continue;
////		if (RECTtoRECT(_unit->CollideArea, (*iter)->CollideArea))
////			return TRUE;
////	}
////
////	return FALSE;
////}
////STCollideData Collide::Collide_UNIT_EX(UNIT *_unit)
////{
////	list<UNIT*>::iterator iter;
////	STCollideData colInfo;
////
////	for (iter = UNITlist.begin();
////		iter != UNITlist.end(); ++iter)
////	{
////		if ((*iter) == _unit) continue;
////		colInfo=RECTtoRECT_EX(_unit->CollideArea, (*iter)->CollideArea);
////		if (colInfo.IsCollide == TRUE)return colInfo;		
////	}
////
////	return  colInfo;
////}
////bool Collide::AvoidCollide_UNIT(UNIT *_unit)
////{
////	list<UNIT*>::iterator iter;
////	CollideInfo ColInfo;
////
////	for (iter = UNITlist.begin();
////		iter != UNITlist.end(); ++iter)
////	{
////		if ((*iter) == _unit) continue;
////		ColInfo = RECTtoRECT_EX(_unit->CollideArea, (*iter)->CollideArea);
////		AdjustPosition(_unit->posCenter, ColInfo);
////	}	
////	return TRUE;
////}
////
////bool Collide::Unit_Hit(UNIT *_unit,short _nUniType)
////{
////	list<UNIT*>::iterator iter;
////
////	for (iter = UNITlist.begin();
////		iter != UNITlist.end(); ++iter)
////	{
////		if ((*iter) == _unit) continue;
////		if ((*iter)->UnitType == _nUniType) continue;
////		if ((*iter)->UnitType == UNITObstacle) continue;
////		if (RECTtoRECT(_unit->AtkArea[_unit->ndir], (*iter)->posArea))
////		{
////			DamageInfo DmgInfo;
////			DmgInfo = getdamage(_unit, (*iter));
////			(*iter)->ATT.nCurHP -= DmgInfo.nDamage;
////			DmgInfolist.push_back(DmgInfo);
////		}
////	}
////	return FALSE;
////}
////

void Collide::AvoidcollideTile(CTileMap* map, CObject *obj)
{
	STCollideData collideData;

	CCollideArea *Area = obj->GetCollideArea();

	if (Area == NULL)return;

	DWORD RectCount = Area->GetRectCount();

	DWORD CircleCount = Area->GetCircleCount();


	//check if rect collide with rect		
	for (int i = 0; i < RectCount; i++)
	{
		//get rect
		RECT ColRect = Area->GetRect(i);
		//get map tile which collide with rect
		STMapPos mapPos = map->GetRectPos(ColRect);

		for (WORD i = mapPos.nTile_Top; i <= mapPos.nTile_Bottom; i++)
		{
			for (WORD j = mapPos.nTile_Left; j <= mapPos.nTile_Right; j++)
			{

				CTile* Tile = map->GetTile(i,j);
				if (Tile == NULL)continue;
				if (Tile->GetObstacleLevel() > 0)
				{
					//get tile rect
					RECT rectTile = map->GetGridRect(GRID(i, j));

					//check collide
					collideData = RECTtoRECT_EX(ColRect, rectTile);

					//avoid collide
					AdjustPosition(obj, collideData);

					if (collideData.IsCollide == TRUE) 
					{
						obj->MarkHasBeenCollide();
					}
				}
			}
		}
	}
}



//STCollideData Collide::AvoidcollideTile(map*_map, UNIT *_unit)
//{
//	STCollideData _collideinfo;
//	int left_tile = _unit->CollideArea.left / _map->nGridLength;
//	int right_tile = _unit->CollideArea.right / _map->nGridLength;
//	int top_tile = _unit->CollideArea.top / _map->nGridLength;
//	int bottom_tile = _unit->CollideArea.bottom / _map->nGridLength;
//
//	if (left_tile < 0) left_tile = 0;
//	if (right_tile > _map->mapGrid.nColumn-1) 
//		right_tile = _map->mapGrid.nColumn-1;
//	if (top_tile < 0) top_tile = 0;
//	if (bottom_tile > _map->mapGrid.nRow-1)
//		bottom_tile = _map->mapGrid.nRow-1;
//
//
//		for (WORD i = left_tile; i <= right_tile; i++)
//			{
//				for (WORD j = top_tile; j <= bottom_tile; j++)
//				{						
//						if (_map->grid[j][i].IsObstacle==TRUE)
//						{
//							RECT _rect = _map->GetCoord({ j, i });
//
//							_collideinfo = RECTtoRECT_EX(_unit->CollideArea, _rect);
//							AdjustPosition(_unit->posCenter, _collideinfo);
//						}
//				}
//			}
//		_collideinfo.IsCollide = FALSE;
//		return _collideinfo;
//}