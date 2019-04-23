#pragma once
#include"LivingObjectClass.h"

class CHero :public CLivingObj
{
public:
	CHero();
	virtual~CHero();

protected:
	void UpdateHero();
};
