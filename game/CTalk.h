#pragma once
#include"DialogData.h"
#include<vector>

class CTalk
{
public:
	CTalk();
	~CTalk();

	CDialogChain* GetDialog(int i=0);
	CDialogChain* GetDialogRand();

	void SetDialog(CDialogChain* dialog);
	void ClearDialog();

private:
	std::vector<CDialogChain*>m_vectorDialog;
};	

