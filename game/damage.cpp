#include"Damage.h"
#include"Text.h"

CDamageData::CDamageData() : m_pTextsuf(NULL)
{
}

CDamageData::~CDamageData()
{
	if (m_pTextsuf != NULL)
	{
		delete m_pTextsuf;
		m_pTextsuf = NULL;
	}
}

CDamageData* __CalDamage(CLivingObj *attacker, CLivingObj *target)
{
	CDamageData *DmgData = new CDamageData();

	DmgData->m_fx = target->GetX();
	DmgData->m_fy = target->GetY();
	DmgData->m_enDmgFrom = attacker->GetForce();


	if (rand() % 100 + 1 <= target->GetEva() * 100)//target avoided attack 
	{
		DmgData->m_enDmgType = eDmgMiss;
		DmgData->m_nDamage = 0;
	}
	else
	{
		if (rand() % 100 + 1 <= attacker->GetCri() * 100)//Criticla Damage
		{
			DmgData->m_enDmgType = eDmgCritical;
			DmgData->m_nDamage = attacker->GetAtk()*1.5 - target->GetDef()*0.5;
		}
		else
		{
			DmgData->m_enDmgType = eDmgNormal;//Normal Damage
			DmgData->m_nDamage = attacker->GetAtk() - target->GetDef()*0.5;
		}
		
		//No damage
		if (DmgData->m_nDamage <= 0 && DmgData->m_enDmgType != eDmgMiss)
		{
			DmgData->m_enDmgType = enDamageType::eDmgNone;
			DmgData->m_nDamage = 0;//damage can't less than 0
		}
	}

	//set text color
	enColor enDrawColor = enColor::eCrBlack;
	switch (DmgData->m_enDmgFrom)
	{
		case enObjectForce::eForFriend: 
		{
			switch (DmgData->m_enDmgType)
			{
				case eDmgNormal: enDrawColor = enColor::eCrBlue;
					break;
				case eDmgCritical: enDrawColor = enColor::eCrRed;
					break;
				case eDmgNone:
				case eDmgMiss:enDrawColor= enColor::eCrGray;
					break;	
				default:
					break;
			}

		} break;
		case enObjectForce::eForHostile: 
		{
			switch (DmgData->m_enDmgType)
			{
				case eDmgNormal:enDrawColor = enColor::eCrBlack;
					break;
				case eDmgCritical:enDrawColor = enColor::eCrPurple;
					break;
				case eDmgMiss:
				case eDmgNone: enDrawColor = enColor::eCrGray;
					break;
				default:
					break;
			}
		}break;

	default:
		break;
	}


	//create text surface
	switch (DmgData->m_enDmgType)
	{
		case enDamageType::eDmgNone:
		{
			DmgData->m_pTextsuf = new CTextSuf("NO DAMAGE", 15, 0, g_pUIMng->GetTextSheet(), enDrawColor);
		}break;

		case enDamageType::eDmgCritical:
		{
			char buffer[50];

			sprintf(buffer, "Critical-%d", DmgData->m_nDamage);

			DmgData->m_pTextsuf = new CTextSuf(buffer, 8, 0, g_pUIMng->GetTextSheet(), enDrawColor);
		}break;

		case enDamageType::eDmgMiss:
		{
			DmgData->m_pTextsuf = new CTextSuf("MISS", 15, 0, g_pUIMng->GetTextSheet(), enDrawColor);
		}break;

		case enDamageType::eDmgNormal:
		{
			char buffer[50];

			sprintf(buffer, "-%d", DmgData->m_nDamage);

			DmgData->m_pTextsuf = new CTextSuf(buffer, 15, 0, g_pUIMng->GetTextSheet(), enDrawColor);

		}break;

	default:
		break;
	}

	g_LIst_DmgData.push_back(DmgData);//save damage data 

	return DmgData;
}

bool __DisplayDamage(LPDIRECTDRAWSURFACE7 suf,float second,float speed)
{
	list<CDamageData*>::iterator iter;

	for (iter = g_LIst_DmgData.begin();iter != g_LIst_DmgData.end(); )
	{
		//timer 
		if ((*iter)->m_Timer.OnceTimer(second))//text life
		{
			delete (*iter);
			(*iter) = NULL;

			iter = g_LIst_DmgData.erase(iter);//delete text
		}
		else 
		{
			(*iter)->m_pTextsuf->DrawTextSuf((*iter)->m_fx, (*iter)->m_fy, suf);

			(*iter)->m_fy -= speed*__GetDeltaTime;//text go up 
			++iter;
		}
	}
	return TRUE;
}

void __ClearDamageData() 
{
	list<CDamageData*>::iterator iter;

	for (iter = g_LIst_DmgData.begin(); iter != g_LIst_DmgData.end(); iter++)
	{
		if ((*iter) != NULL) 
		{
			delete (*iter);
			(*iter) = NULL;
		}
	}
	g_LIst_DmgData.clear();
}
