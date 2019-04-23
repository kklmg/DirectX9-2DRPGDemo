#include"Attribute.h"
#include"Global_data.h"

CAttribute::CAttribute()
{
	SetAttribute
	(
		100,	//HP
		50,		//MP
		15,		//atk
		5,		//def
		0.3,	//cri
		0.1,	//eva
		100,	//walk speed
		5,		//run speed
		2,		//atk rate
		30,		//atkrng
		250		//detectrng		
	);
	m_dwLevel = 1;
	m_dwCurExp = 0;
	m_dwMaxExp = 1000;
}

CAttribute::~CAttribute()
{
}

bool CAttribute::Init(STBasicATTTable *AttTable, DWORD level) 
{
	if (AttTable == NULL) return FALSE;
	m_prAttTable = AttTable;

	UpdateBasicATT(level);
	RecoverAllHP();
	RecoverAllMP();

	return TRUE;
}

void CAttribute::Update() 
{
	m_stHP.Update();
	m_stMP.Update();

	m_stAtk.Update();
	//m_stDef.Update();

	m_stCrt.Update();
	m_stEva.Update();

	

	LevelUp();
}

void CAttribute::Reset() 
{
	m_stHP.Reset();
	m_stMP.Reset();

	m_stAtk.Reset();
	m_stDef.Reset();

	m_stCrt.Reset();
	m_stEva.Reset();

	m_stAttackSpeed.Reset();
	m_stMoveSpeed.Reset();
}

void CAttribute::LevelUp() 
{
	if (m_dwCurExp >= m_dwMaxExp) 
	{
		m_dwLevel++;

		m_dwCurExp -= m_dwMaxExp;
	
		m_dwMaxExp = g_pAttMng->GetMaxExp(m_dwLevel);

		if (m_prAttTable != NULL)	UpdateBasicATT(m_dwLevel);
	}
}

void CAttribute::UpdateBasicATT(DWORD level) 
{
	m_stHP.Init(*m_prAttTable, level);
	m_stMP.Init(*m_prAttTable, level);

	m_stAtk.Init(*m_prAttTable, level);
	m_stDef.Init(*m_prAttTable, level);
	
	m_stCrt.Init(*m_prAttTable, level);
	m_stEva.Init(*m_prAttTable, level);

	m_stAttackSpeed.Init(*m_prAttTable, level);
	m_stMoveSpeed.Init(*m_prAttTable, level);

	m_dwAtkDis = m_prAttTable->dwAtkDis;
	m_dwDetectDis = m_prAttTable->dwDetectDis;
}


void CAttribute::SetHP(DWORD hp)
{
	m_stHP.nCurHP = hp;
}
int CAttribute::GetCurHp()const 
{
	return m_stHP.nCurHP;
}
DWORD CAttribute::GetMaxHp() const 
{
	return m_stHP.dwMAXHP;
}
void CAttribute::RecoverAllHP()
{
	if (m_stHP.nCurHP < m_stHP.dwMAXHP) 
	{
		m_stHP.nCurHP = m_stHP.dwMAXHP;
	}
}



void CAttribute::SetMP(DWORD mp)
{
	m_stMP.nCurMP = mp;
}
int CAttribute::GetCurMp()const 
{
	return m_stMP.nCurMP;
}
DWORD CAttribute::GetMaxMp()const  
{
	return m_stMP.dwMAXMP;
}
void CAttribute::RecoverAllMP()
{
	if (m_stMP.nCurMP < m_stMP.dwMAXMP)
	{
		m_stMP.nCurMP = m_stMP.dwMAXMP;
	}
}



void CAttribute::SetAtk(DWORD atk)
{
	m_stAtk.nRealAtk = atk;
}

DWORD CAttribute::GetAtk() const
{
	return m_stAtk.nRealAtk;
}

float CAttribute::GetAtkRate() const
{
	return m_stAttackSpeed.fRealAtkRate;
}

float CAttribute::GetAtkInterval() const
{
	return m_stAttackSpeed.fBasicAtkInterval;
}



void CAttribute::SetDef(DWORD def)
{
	m_stDef.nRealDef = def;
}
DWORD CAttribute::GetDef()const
{
	return m_stDef.nRealDef;
}

void CAttribute::SetCri(float crt)
{
	m_stCrt.fRealCrt = crt;
}
float CAttribute::GetCri()const
{
	return m_stCrt.fRealCrt;
}

void CAttribute::SetEva(float eva)
{
	m_stEva.fRealEva = eva;
}
float CAttribute::GetEva()const
{
	return m_stEva.fRealEva;
}

void CAttribute::SetAttribute(DWORD hp, DWORD mp, DWORD atk, DWORD def, float cri, float eva,
	DWORD walkSpeed, DWORD runSpeed, float atkrate, DWORD atkrng, DWORD detectrng)
{
	m_stHP.nCurHP = hp;
	m_stMP.nCurMP = mp;

	m_stAtk.nRealAtk = atk;
	m_stDef.nRealDef = def;

	m_stCrt.fRealCrt = cri;
	m_stEva.fRealEva = eva;


	m_stMoveSpeed.nRealSpeed = walkSpeed;

	m_stMoveSpeed.nRunSpeed = runSpeed;

	m_stAttackSpeed.fRealAtkRate = atkrate;

	m_dwAtkDis = atkrng;

	m_dwDetectDis = detectrng;
}


void CAttribute::SetDetectDistance(DWORD distance)
{
	m_dwDetectDis = distance;
}
DWORD CAttribute::GetDetectDistance() const
{
	return m_dwDetectDis;
}


void CAttribute::SetAttackDistance(DWORD distance) 
{
	m_dwAtkDis = distance;
}
DWORD CAttribute::GetAttackDistance()const 
{
	return m_dwAtkDis;
}


DWORD CAttribute::GetWalkSpeed()const
{
	return m_stMoveSpeed.nWalkSpeed;
}

DWORD CAttribute::GetWalkSpeed_Tilt()const
{
	return m_stMoveSpeed.nWalkSpeed_Tilt;
}

DWORD CAttribute::GetRunSpeed()const
{
	return m_stMoveSpeed.nRunSpeed;
}

DWORD CAttribute::GetRunSpeed_Tilt()const
{
	return m_stMoveSpeed.nRunSpeed_Tilt;
}



void CAttribute::SetAttackRate(float rate) 
{
	m_stAttackSpeed.fRealAtkRate = rate;
}