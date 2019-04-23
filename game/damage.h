#pragma once
#include"LivingObjectClass.h"
#include"TimerClass.h"
#include"TileMap.h"
#include"TextSurface.h"

#include<list>

using namespace std;

enum enDamageType
{
	eDmgNormal,
	eDmgCritical,
	eDmgMiss,
	eDmgDot,
	eDmgMagic,
	eDmgSkill,
	eDmgNone,
};

class CDamageData
{
public:
	CDamageData();
	~CDamageData();

	float m_fx, m_fy;
	
	enDamageType m_enDmgType;

	enObjectForce m_enDmgFrom;

	int m_nDamage;

	CTimer m_Timer;

	CTextSuf *m_pTextsuf;
};

static list<CDamageData*> g_LIst_DmgData;

CDamageData* __CalDamage(CLivingObj *attacker, CLivingObj *target);

bool __DisplayDamage(LPDIRECTDRAWSURFACE7 suf, float second, float speed);

void __ClearDamageData();