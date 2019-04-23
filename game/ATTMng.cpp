#include"ATTMng.h"
#include"EnumGroup.h"

CATTMng::CATTMng()
{
	m_vector_ATTTable.resize(0);
}

CATTMng::~CATTMng()
{
}

void CATTMng::Init() 
{
	m_vector_ATTTable.resize(10);

	//Common
	m_vector_ATTTable[eCommon].SetBasicATT(	/*HP*/100,	/*MP*/50,	/*ATK*/10,	/*DEF*/ 5,	/*CRT*/ 0.1,  /*EVA*/ 0.1);		  
	m_vector_ATTTable[eCommon].SetATTGrowth(/*HP*/10,	/*MP*/10,	/*ATK*/6,	/*DEF*/4,	/*CRT*/0.01,  /*EVA*/ 0.01);
	m_vector_ATTTable[eCommon].SetAttackSpeed(/*AtkRate*/0.2,/*Grow_AtkRate*/-0.0002,/*AtkInterval*/0.18, /*Grow_AtkInterval*/-0.0002);
	m_vector_ATTTable[eCommon].SetMoveSpeed(/*MoveSpeed*/100,/*grow_MoveSpeed*/2);
	m_vector_ATTTable[eCommon].SetDistance(/*AtkDis*/40,/*DetectDis*/200);

	//Hero Knight
	m_vector_ATTTable[eHeroKnight].SetBasicATT(/*HP*/150, /*MP*/50, /*ATK*/8, /*DEF*/6, /*CRT*/0.1, /*EVA*/0.1);
	m_vector_ATTTable[eHeroKnight].SetATTGrowth(/*HP*/12, /*MP*/8, /*ATK*/6, /*DEF*/10, /*CRT*/0.01, /*EVA*/0.01);
	m_vector_ATTTable[eHeroKnight].SetAttackSpeed(/*AtkRate*/0.12,/*Grow_AtkRate*/-0.0002,/*AtkInterval*/0.125, /*Grow_AtkInterval*/-0.0002);
	m_vector_ATTTable[eHeroKnight].SetMoveSpeed(/*MoveSpeed*/100,/*grow_MoveSpeed*/2);
	m_vector_ATTTable[eHeroKnight].SetDistance(/*AtkDis*/55,/*DetectDis*/200);

	//Hero Archer
	m_vector_ATTTable[eHeroArcher].SetBasicATT(/*HP*/120, /*MP*/70, /*ATK*/9, /*DEF*/7, /*CRT*/0.2, /*EVA*/0.2);
	m_vector_ATTTable[eHeroArcher].SetATTGrowth(/*HP*/8, /*MP*/10, /*ATK*/8, /*DEF*/5, /*CRT*/0.05, /*EVA*/0.05);
	m_vector_ATTTable[eHeroArcher].SetAttackSpeed(/*AtkRate*/0.15,/*Grow_AtkRate*/-0.0002,/*AtkInterval*/0.15, /*Grow_AtkInterval*/-0.0001);
	m_vector_ATTTable[eHeroArcher].SetMoveSpeed(/*MoveSpeed*/100,/*grow_MoveSpeed*/2);
	m_vector_ATTTable[eHeroArcher].SetDistance(/*AtkDis*/40,/*DetectDis*/200);

	//Hero Assasin
	m_vector_ATTTable[eHeroAssasin].SetBasicATT(/*HP*/100, /*MP*/80, /*ATK*/10, /*DEF*/6, /*CRT*/0.33, /*EVA*/0.33);
	m_vector_ATTTable[eHeroAssasin].SetATTGrowth(/*HP*/7, /*MP*/9, /*ATK*/7, /*DEF*/3, /*CRT*/0.02, /*EVA*/0.03);
	m_vector_ATTTable[eHeroAssasin].SetAttackSpeed(/*AtkRate*/0.1,/*Grow_AtkRate*/-0.0005,/*AtkInterval*/0.1, /*Grow_AtkInterval*/-0.0002);
	m_vector_ATTTable[eHeroAssasin].SetMoveSpeed(/*MoveSpeed*/100,/*grow_MoveSpeed*/2);
	m_vector_ATTTable[eHeroAssasin].SetDistance(/*AtkDis*/40,/*DetectDis*/200);

	//Hero Magician
	m_vector_ATTTable[eHeroMagician].SetBasicATT(/*HP*/80, /*MP*/120, /*ATK*/10, /*DEF*/5, /*CRT*/0.15, /*EVA*/0.2);
	m_vector_ATTTable[eHeroMagician].SetATTGrowth(/*HP*/12, /*MP*/8, /*ATK*/6, /*DEF*/10, /*CRT*/0.02, /*EVA*/0.01);
	m_vector_ATTTable[eHeroMagician].SetAttackSpeed(/*AtkRate*/0.16,/*Grow_AtkRate*/-0.0002,/*AtkInterval*/0.18, /*Grow_AtkInterval*/-0.0002);
	m_vector_ATTTable[eHeroMagician].SetMoveSpeed(/*MoveSpeed*/100,/*grow_MoveSpeed*/2);
	m_vector_ATTTable[eHeroMagician].SetDistance(/*AtkDis*/40,/*DetectDis*/200);

	//Hero Warrior
	m_vector_ATTTable[eHeroWarrior].SetBasicATT(/*HP*/200, /*MP*/40, /*ATK*/15, /*DEF*/8, /*CRT*/0.1, /*EVA*/0.01);
	m_vector_ATTTable[eHeroWarrior].SetATTGrowth(/*HP*/15, /*MP*/5, /*ATK*/8, /*DEF*/8, /*CRT*/0.02, /*EVA*/0.01);
	m_vector_ATTTable[eHeroWarrior].SetAttackSpeed(/*AtkRate*/0.14,/*Grow_AtkRate*/-0.0002,/*AtkInterval*/0.13, /*Grow_AtkInterval*/-0.0001);
	m_vector_ATTTable[eHeroWarrior].SetMoveSpeed(/*MoveSpeed*/100,/*grow_MoveSpeed*/2);
	m_vector_ATTTable[eHeroWarrior].SetDistance(/*AtkDis*/40,/*DetectDis*/200);

	//Monster Rabbit
	m_vector_ATTTable[eMonRabbit].SetBasicATT(/*HP*/50, /*MP*/20, /*ATK*/6, /*DEF*/4, /*CRT*/0.05, /*EVA*/0.2);
	m_vector_ATTTable[eMonRabbit].SetATTGrowth(/*HP*/5, /*MP*/5, /*ATK*/5, /*DEF*/5, /*CRT*/0.01, /*EVA*/0.03);
	m_vector_ATTTable[eMonRabbit].SetAttackSpeed(/*AtkRate*/0.2,/*Grow_AtkRate*/-0.0002,/*AtkInterval*/0.5, /*Grow_AtkInterval*/-0.0002);
	m_vector_ATTTable[eMonRabbit].SetMoveSpeed(/*MoveSpeed*/100,/*grow_MoveSpeed*/2);
	m_vector_ATTTable[eMonRabbit].SetDistance(/*AtkDis*/40,/*DetectDis*/200);

}

DWORD CATTMng::GetMaxExp(DWORD level) 
{
	return level * 100;
}

STBasicATTTable CATTMng::GetATTTable(enLOBJ index)
{
	return m_vector_ATTTable[index];
}