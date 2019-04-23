#include"DialogueData.h"
#include"DialogueToken.h"

CDialogueChain::CDialogueChain()
{
}

CDialogueChain::~CDialogueChain()
{
	std::vector<STDialogue*>::iterator iter = m_vectorData.begin();
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

void CDialogueChain::Insert(STDialogue* Dialogue)
{
	m_vectorData.push_back(Dialogue);
}

STDialogue* CDialogueChain::getDialogue(int index)
{
	if (index >= m_vectorData.size())return NULL;
	else return m_vectorData[index];
}

int CDialogueChain::GetSize() const
{
	return m_vectorData.size();
}

CDialogueDataMng::CDialogueDataMng() :m_nTokenType(-1)
{
}

CDialogueDataMng::~CDialogueDataMng()
{
	std::map<std::string, CDialogueChain*>::iterator iter = m_MapDigData.begin();

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

CDialogueChain* CDialogueDataMng::GetDialogueChain(LPCSTR str)
{
	std::map<std::string, CDialogueChain*>::iterator iter = m_MapDigData.find(str);
	if (iter != m_MapDigData.end()) 
	{
		return (*iter).second;
	}
	else return NULL;
}

bool CDialogueDataMng::Parsing(LPCSTR str)
{
	m_plex = new DIALOGUE::CLexer(str);

	while (FindToken(enDiagueToken::eDigTokend_Scene))
	{
		//string SceneName = m_acGetString; //get Scene Name

		NextBlockStart();

		while (FindToken_InBlock(enDiagueToken::eDigTokend_Dialogue))
		{
			CDialogueChain *DialogueC = new CDialogueChain;

			NextToken();
			std::string NpcName = m_acGetString; //get Scene Name

			NextBlockStart();

			while (FindToken_InBlock(eDigTokend_Character))
			{
				STDialogue *Dialogue = new STDialogue;

				NextToken();
				Dialogue->m_strCharacter = m_acGetString;

				NextToken();
				Dialogue->m_strDiag = m_acGetString;

				DialogueC->Insert(Dialogue);
			}
			m_MapDigData.insert(make_pair(NpcName, DialogueC));
		}
	}

	delete m_plex;
	return TRUE;
}

bool CDialogueDataMng::NextToken()
{
	m_nTokenType = m_plex->GetToken(m_acGetString);
	if (m_nTokenType == TOKEND_END)	return FALSE;
	return TRUE;
}
bool CDialogueDataMng::NextBlockStart()
{
	while (m_nTokenType != TOKEND_BLOCK_START)
	{
		NextToken();
		if (m_nTokenType == TOKEND_END)	return FALSE;
	}
	++m_nbkcount;
	return TRUE;
}
bool CDialogueDataMng::NextBlockEnd()
{
	while (m_nTokenType != TOKEND_BLOCK_END)
	{
		NextToken();
		if (m_nTokenType == TOKEND_END)	return FALSE;
	}
	--m_nbkcount;
	return TRUE;
}
bool CDialogueDataMng::FindToken(int FindToken)
{
	while (m_nTokenType != FindToken)
	{
		NextToken();
		if (m_nTokenType == TOKEND_END)	return FALSE;
	}
	return TRUE;
}
bool CDialogueDataMng::FindToken_InBlock(int FindToken)
{
	while (m_nTokenType != FindToken)
	{
		NextToken();
		if (m_nTokenType == TOKEND_BLOCK_END)return FALSE;
		if (m_nTokenType == TOKEND_END)	return FALSE;
	}
	return TRUE;
}




