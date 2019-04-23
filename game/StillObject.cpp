#include"StillObject.h"
#include"ObjectMng.h"

CStillObject::CStillObject() 
{
	CreateCollideArea(FALSE);

	g_list_pStillObject.push_back(this);
}

CStillObject::~CStillObject() 
{

}