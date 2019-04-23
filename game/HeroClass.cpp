#include"HeroClass.h"
#include"InputClass.h"
#include"RectMath.h"
#include"ObjectMng.h"
#include"Global_data.h"

CHero::CHero() 
{
	m_enObjType = enObjectType::eOTHero;

	g_list_pHero.push_back(this);
}

CHero::~CHero() 
{
	std::list<CHero*>::iterator iter;

	for (iter = g_list_pHero.begin(); iter != g_list_pHero.end();) 
	{
		if ((*iter) == this)
		{
			iter = g_list_pHero.erase(iter);
			break;
		}
		else iter++;

	}
}

void CHero::UpdateHero() 
{
}

