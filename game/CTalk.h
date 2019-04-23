#pragma once
#include"DialogueData.h"
#include<vector>

class CTalk
{
public:
	CTalk();
	~CTalk();

	CDialogueChain* GetDialogue(int i=0);
	CDialogueChain* GetDialogueRand();

	void SetDialogue(CDialogueChain* dialog);
	void ClearDialogue();

private:
	std::vector<CDialogueChain*>m_vectorDialogue;
};	

