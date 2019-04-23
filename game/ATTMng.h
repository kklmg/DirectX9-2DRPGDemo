#pragma once
#include"Attribute.h"
#include<vector>
#include"EnumGroup.h"
using namespace std;

class CATTMng
{
public:
	CATTMng();
	~CATTMng();

	void Init();

	DWORD GetMaxExp(DWORD level);
	
	STBasicATTTable GetATTTable(enLOBJ index);

private:
	vector<STBasicATTTable>m_vector_ATTTable;
};

