#include"CTalk.h"


CTalk::CTalk()
{
	m_vectorDialogue.reserve(5);
}
CTalk::~CTalk()
{
}

void CTalk::SetDialogue(CDialogueChain* dialog)
{
	m_vectorDialogue.push_back(dialog);
}
void CTalk::ClearDialogue()
{
	m_vectorDialogue.clear();
}

CDialogueChain* CTalk::GetDialogue(int i)
{
	if (i >= m_vectorDialogue.size())return NULL;
	else return m_vectorDialogue[i];
}
CDialogueChain* CTalk::GetDialogueRand()
{
	if (m_vectorDialogue.size() == 0)return NULL;

	int index = rand() % m_vectorDialogue.size();

	return m_vectorDialogue[index];
}