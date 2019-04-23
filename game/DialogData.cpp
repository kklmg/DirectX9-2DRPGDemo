#include"DialogData.h"
#include"DialogToken.h"

CDialogChain::CDialogChain()
{
}

CDialogChain::~CDialogChain()
{
	std::vector<STDialog*>::iterator iter = m_vectorData.begin();
	while (iter!= m_vectorData.end())
	{
		if (*iter != NULL)
		{
			delete (*iter);
			(*iter) = NULL;
		}
		++iter;
	}
	m_vectorData.clear();
}

void CDialogChain::Insert(STDialog* dialog)
{
	m_vectorData.push_back(dialog);
}

STDialog* CDialogChain::getDialog(int index)
{
	if (index >= m_vectorData.size())return NULL;
	else return m_vectorData[index];
}

int CDialogChain::GetSize() const
{
	return m_vectorData.size();
}

CDialogDataMng::CDialogDataMng() :m_nTokenType(-1)
{
}

CDialogDataMng::~CDialogDataMng()
{
	std::map<std::string, CDialogChain*>::iterator iter = m_MapDigData.begin();

	for (; iter != m_MapDigData.end();)
	{
		if ((*iter).second != NULL)
		{
			delete (*iter).second;
			(*iter).second = NULL;
		}
		m_MapDigData.erase(iter++);
	}
}

CDialogChain* CDialogDataMng::GetDialogChain(LPCSTR str)
{
	std::map<std::string, CDialogChain*>::iterator iter = m_MapDigData.find(str);
	if (iter != m_MapDigData.end()) 
	{
		return (*iter).second;
	}
	else return NULL;
}

bool CDialogDataMng::Parsing(LPCSTR str)
{
	m_plex = new DIALOG::CLexer(str);

	while (FindToken(enDiagToken::eDigTokend_Scene))
	{
		//string SceneName = m_acGetString; //get Scene Name

		NextBlockStart();

		while (FindToken_InBlock(enDiagToken::eDigTokend_Dialog))
		{
			CDialogChain *dialogC = new CDialogChain;

			NextToken();
			std::string NpcName = m_acGetString; //get Scene Name

			NextBlockStart();

			while (FindToken_InBlock(eDigTokend_Character))
			{
				STDialog *Dialog = new STDialog;

				NextToken();
				Dialog->m_strCharacter = m_acGetString;

				NextToken();
				Dialog->m_strDiag = m_acGetString;

				dialogC->Insert(Dialog);
			}
			m_MapDigData.insert(make_pair(NpcName, dialogC));
		}
	}

	delete m_plex;
	return TRUE;
}

bool CDialogDataMng::NextToken()
{
	m_nTokenType = m_plex->GetToken(m_acGetString);
	if (m_nTokenType == TOKEND_END)	return FALSE;
	return TRUE;
}
bool CDialogDataMng::NextBlockStart()
{
	while (m_nTokenType != TOKEND_BLOCK_START)
	{
		NextToken();
		if (m_nTokenType == TOKEND_END)	return FALSE;
	}
	++m_nbkcount;
	return TRUE;
}
bool CDialogDataMng::NextBlockEnd()
{
	while (m_nTokenType != TOKEND_BLOCK_END)
	{
		NextToken();
		if (m_nTokenType == TOKEND_END)	return FALSE;
	}
	--m_nbkcount;
	return TRUE;
}
bool CDialogDataMng::FindToken(int FindToken)
{
	while (m_nTokenType != FindToken)
	{
		NextToken();
		if (m_nTokenType == TOKEND_END)	return FALSE;
	}
	return TRUE;
}
bool CDialogDataMng::FindToken_InBlock(int FindToken)
{
	while (m_nTokenType != FindToken)
	{
		NextToken();
		if (m_nTokenType == TOKEND_BLOCK_END)return FALSE;
		if (m_nTokenType == TOKEND_END)	return FALSE;
	}
	return TRUE;
}




