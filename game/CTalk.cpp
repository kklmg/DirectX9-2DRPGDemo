#include"CTalk.h"


CTalk::CTalk()
{
	m_vectorDialog.reserve(5);
}
CTalk::~CTalk()
{
}

void CTalk::SetDialog(CDialogChain* dialog)
{
	m_vectorDialog.push_back(dialog);
}
void CTalk::ClearDialog()
{
	m_vectorDialog.clear();
}

CDialogChain* CTalk::GetDialog(int i)
{
	if (i >= m_vectorDialog.size())return NULL;
	else return m_vectorDialog[i];
}
CDialogChain* CTalk::GetDialogRand()
{
	if (m_vectorDialog.size() == 0)return NULL;

	int index = rand() % m_vectorDialog.size();

	return m_vectorDialog[index];
}