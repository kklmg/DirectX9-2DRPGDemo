#pragma once
#include<Windows.h>



typedef struct STCircle
{
	STCircle() :point({0,0}), radius(0)
	{
	}
	STCircle(POINTFLOAT __point, float __radius): point(__point), radius(__radius)
	{
	}
	STCircle(POINT __point, float __radius) : point({ float(__point.x),  float(__point.y)}), radius(__radius)
	{
	}
	
	POINTFLOAT point;
	float radius;
}_CIRCLE;

