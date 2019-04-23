#pragma once
#include<Windows.h>
#include<math.h>

//basic Att table
struct STBasicATTTable
{
	//HP
	DWORD dwBasicHP=200; DWORD dwHPGrowth=10;
	//MP
	DWORD dwBasicMP=100; DWORD dwMPGrowth=10;
	//Attack
	DWORD dwBasicAtk=15; DWORD dwAtkGrowth=10;
	//Defence
	DWORD dwBasicDef=10; DWORD dwDefGrowth=5;
	//Critical Rate
	float fBasicCri=0.1; float fCriGrowth=0.01;
	//Evade Rate
	float fBasicEva=0.1; float fEvaGrowth=0.01;
	//Speed
	DWORD dwMoveSpeed = 50; DWORD dwMoveGrowth = 0;
	//attack speed
	float fAtkrate = 0.5; float AtkRateGrowth = -0.005;

	float fAtkInterval = 0.5; float fAtkIntervalGrowth = -0.005;

	//Attack Distance
	DWORD dwAtkDis = 30; 
	//Detect Distance
	DWORD dwDetectDis = 200;
	

	void SetBasicATT(DWORD HP, DWORD MP, DWORD Atk, DWORD Def, float Crt, float Eva)
	{
		//HP
		dwBasicHP = HP;
		//MP
		dwBasicMP = MP;
		//Attack
		dwBasicAtk = Atk;
		//Defence
		dwBasicDef = Def;
		//Critical Rate
		fBasicCri = Crt;
		//Evade Rate
		fBasicEva = Eva;
	}
	void SetATTGrowth(DWORD grow_HP, DWORD grow_MP, DWORD grow_Atk, DWORD grow_Def, float grow_Crt, float grow_Eva)
	{
		//HP
		dwHPGrowth = grow_HP;
		//MP
		dwMPGrowth = grow_MP;
		//Attack
		dwAtkGrowth = grow_Atk;
		//Defence
		dwDefGrowth = grow_Def;
		//Critical Rate
		fCriGrowth = grow_Crt;
		//Evade Rate
		fEvaGrowth = grow_Eva;
	}

	void SetMoveSpeed(DWORD MoveSpeed,DWORD  grow_MoveSpeed)
	{
		dwMoveSpeed = MoveSpeed;
		dwMoveGrowth = grow_MoveSpeed;
	}
	void SetAttackSpeed(float Atkrate,float grow_AtkRate,float AtkInterval,float grow_AtkInterval) 
	{
		fAtkrate = Atkrate;
		AtkRateGrowth = grow_AtkRate;
		fAtkInterval = AtkInterval;
		fAtkIntervalGrowth = grow_AtkInterval;	
	}

	void SetDistance(DWORD AtkDis, DWORD DetectDis)
	{
		dwAtkDis = AtkDis;
		dwDetectDis = DetectDis;
	}

	DWORD GetHP(DWORD level) { return dwBasicHP + dwHPGrowth*level; }
	DWORD GetMP(DWORD level) { return dwBasicMP + dwMPGrowth*level; }
	DWORD GetAttack(DWORD level) { return dwBasicAtk + dwAtkGrowth*level;}
	DWORD GetDefence(DWORD level) { return dwBasicDef + dwDefGrowth*level; }
	float GetCriticalRate(DWORD level) { return fBasicCri + fCriGrowth*level;}
	float GetEvadeRate(DWORD level) { return fBasicEva + fEvaGrowth*level; }

	DWORD GetMoveSpeed(DWORD level) { return dwMoveSpeed + dwMoveGrowth*level; }
	float GetAttackRate(DWORD level) { return fAtkrate + AtkRateGrowth*level; }
	float GetAttackInterval(DWORD level) { return fAtkInterval + fAtkIntervalGrowth*level; }

};

struct STHP 
{
	DWORD dwBasicHP;
	DWORD dwMAXHP;
	int nCurHP;

	void Init(STBasicATTTable table,DWORD level)
	{
		dwBasicHP = table.GetHP(level);
		dwMAXHP = table.GetHP(level);
	}

	void Update() 
	{
		if (nCurHP > dwMAXHP) 
		{
			nCurHP = dwMAXHP;
		}
		if (nCurHP < 0) 
		{
			nCurHP = 0;
		}
	}
	
	void Reset() 
	{
		dwMAXHP = dwBasicHP;
	}

};

struct STMP
{
	DWORD dwBasicMP;
	DWORD dwMAXMP;
	int nCurMP;

	void Init(STBasicATTTable table, DWORD level)
	{
		dwBasicMP = table.GetMP(level);
		dwMAXMP = table.GetMP(level);
	}

	void Update()
	{
		if (nCurMP > dwMAXMP)
		{
			nCurMP = dwMAXMP;
		}
		if (nCurMP < 0)
		{
			nCurMP = 0;
		}
	}
	void Reset()
	{
		dwMAXMP = dwBasicMP;
	}

};

struct STAtk
{
	DWORD dwBasicAtk;
	int nRealAtk;
	int nBonus = 0;

	void Init(STBasicATTTable table, DWORD level)
	{
		dwBasicAtk = table.GetAttack(level);
		nRealAtk = dwBasicAtk + nBonus;
	}

	void Update()
	{
		if (nRealAtk < 0) nRealAtk = 0;
	}

	void Reset()
	{
		nRealAtk = dwBasicAtk;
	}
};

struct STDef
{
	DWORD dwBasicDef;
	int nRealDef;
	int nBonus = 0;

	void Init(STBasicATTTable table, DWORD level)
	{
		dwBasicDef = table.GetDefence(level);
		nRealDef = dwBasicDef + nBonus;
	}

	void Reset()
	{
		nRealDef = dwBasicDef;
	}
};

struct STCrt
{
	float fBasicCrt;
	float fRealCrt;
	float fBonus = 0;

	void Init(STBasicATTTable table, DWORD level)
	{
		fBasicCrt = table.GetCriticalRate(level);
		fRealCrt = fBasicCrt + fBonus;
	}

	void Update()
	{
		if (fBasicCrt < 0) fBasicCrt = 0;
		if (fBasicCrt > 1) fBasicCrt = 1;
	}

	void Reset()
	{
		fRealCrt = fBasicCrt;
	}
};

struct STEva
{
	float fBasicEva;
	int fRealEva;
	float fBonus = 0;

	void Init(STBasicATTTable table, DWORD level)
	{
		fBasicEva = table.GetCriticalRate(level);
		fRealEva= fBasicEva + fBonus;
	}

	void Update()
	{
		if (fBasicEva < 0) fBasicEva = 0;
		if (fBasicEva > 1) fBasicEva = 1;
	}

	void Reset()
	{
		fRealEva = fBasicEva;
	}
};


struct STAttackSpeed 
{
	float fBasicAtkRate;
	float fRealAtkRate;

	float fBasicAtkInterval;
	float fRealAtkInterval;

	float fBonus = 0;



	void Init(STBasicATTTable table, DWORD level)
	{
		fBasicAtkRate = table.GetAttackRate(level);
		fBasicAtkInterval = table.GetAttackInterval(level);

		fRealAtkRate = fBasicAtkRate + fBonus;
		fRealAtkInterval = fBasicAtkInterval + fBonus;
	}

	void Update()
	{
		if (fRealAtkRate < 0.05) fRealAtkRate = 0.05;
		if (fRealAtkInterval < 0.05) fRealAtkInterval = 0.05;
	}

	void Reset()
	{
		fRealAtkRate = fBasicAtkRate + fBonus;
		fRealAtkInterval = fBasicAtkInterval + fBonus;
	}



};

struct STMoveSpeed 
{
	DWORD dwBasicSpeed;
	int nRealSpeed;

	int nWalkSpeed, nWalkSpeed_Tilt;
	int nRunSpeed, nRunSpeed_Tilt;

	int nBonus = 0;

	void UpdateTiltSpeed() 
	{
		nWalkSpeed_Tilt = nWalkSpeed* sqrt(2)/2;
		nRunSpeed_Tilt = nRunSpeed*sqrt(2)/2;
	}

	void Init(STBasicATTTable table, DWORD level)
	{
		dwBasicSpeed = table.GetMoveSpeed(level);
		nRealSpeed = dwBasicSpeed + nBonus;

		nWalkSpeed = nRealSpeed;
		nRunSpeed = nRealSpeed*2;

		UpdateTiltSpeed();
	}

	void Update()
	{
		if (nRealSpeed < 0) 
		{
			nRealSpeed = 0;	
		}

		nWalkSpeed = nRealSpeed;
		nRunSpeed = nRealSpeed * 2;

		UpdateTiltSpeed();
	}

	void Reset()
	{
		nBonus = 0;
		nRealSpeed = dwBasicSpeed;

		nWalkSpeed = nRealSpeed;
		nRunSpeed = nRealSpeed * 2;
	}

};

class CAttribute
{
public:
	CAttribute();
	~CAttribute();

	bool Init(STBasicATTTable *AttTable,DWORD level);

	void Update();

	void Reset();

	void UpdateBasicATT(DWORD level);

private:
	void LevelUp();

public:

	//set attribute
	void SetAttribute(DWORD hp, DWORD mp,DWORD atk, DWORD def, float cri, float eva,
		DWORD walkSpeed, DWORD runSpeed,float atkrate, DWORD atkrng,DWORD detectrng);


	//HP Function
	void SetHP(DWORD hp);
	int GetCurHp() const;
	DWORD GetMaxHp()const;
	void RecoverAllHP();

	//MP Function
	void SetMP(DWORD mp);
	int GetCurMp()const;
	DWORD GetMaxMp()const;
	void RecoverAllMP();


	//Attack Function
	void SetAtk(DWORD atk);
	DWORD GetAtk()const;

	float GetAtkRate()const;

	float GetAtkInterval()const;

	//Defence Function
	void SetDef(DWORD def);
	DWORD GetDef()const;


	//Critical Function
	void SetCri(float crt);
	float GetCri()const;


	//Evade Function
	void SetEva(float eva);
	float GetEva()const;


	//Move Function
	DWORD GetWalkSpeed()const;
	DWORD GetWalkSpeed_Tilt()const;

	DWORD GetRunSpeed()const;
	DWORD GetRunSpeed_Tilt()const;

	void SetAttackRate(float);

	void SetAttackDistance(DWORD);
	DWORD GetAttackDistance()const;


	void SetDetectDistance(DWORD);
	DWORD GetDetectDistance()const;



protected:
	//refer Table
	STBasicATTTable *m_prAttTable;


	//Level
	DWORD m_dwLevel;

	DWORD m_dwCurExp;

	DWORD m_dwMaxExp;

	//-----------------
	//Basic Attribute
	//-----------------

	//HP
	STHP m_stHP;
	//MP
	STMP m_stMP;
	//Atk
	STAtk m_stAtk;
	//Def
	STDef m_stDef;
	//Crtical rate
	STCrt m_stCrt;
	//Evade rate
	STEva m_stEva;
	//Move Speed
	STMoveSpeed m_stMoveSpeed;
	//Attack Speed
	STAttackSpeed m_stAttackSpeed;
	//Attack Distance
	DWORD m_dwAtkDis;
	//Detect Distance
	DWORD m_dwDetectDis;

};
