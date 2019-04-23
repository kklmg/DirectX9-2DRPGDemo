#include"NPCClass.h"
void CNPC::UpdateAll() 
{
	CObject::UpdateObject();
	CLivingObj::UpdateLivingObj();
}