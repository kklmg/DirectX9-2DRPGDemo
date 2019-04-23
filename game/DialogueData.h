#pragma once
#include<ddraw.h>
#include<string>
#include"DialogueLexer.h"
#include<map>
#include<list>
#include<vector>

struct STDialogue
{
	STDialogue() {}
	STDialogue(LPCSTR Character, LPCSTR Dialog)
	{
		m_strCharacter = Character;
		m_strDiag = Dialog;
	}
	~STDialogue()
	{
		m_strCharacter.clear();
		m_strDiag.clear();
	}

	std::string m_strCharacter;
	std::string m_strDiag;
};

class CDialogueChain 
{
public:
	CDialogueChain();
	~CDialogueChain();

	void Insert(STDialogue*);

	STDialogue* getDialogue(int index);

	int GetSize() const;

private:
	std::vector<STDialogue*> m_vectorData;
};

class CDialogueDataMng
{
public:
	CDialogueDataMng();
	~CDialogueDataMng();

	bool Parsing(LPCSTR str);

	CDialogueChain* GetDialogueChain(LPCSTR);

private:
	bool NextToken();
	bool NextBlockStart();
	bool NextBlockEnd();
	bool FindToken(int FindToken);
	bool FindToken_InBlock(int FindToken);

private:
	int m_nbkcount;

	DIALOGUE::CLexer *m_plex;

	char m_acGetString[256];
	int m_nTokenType;

	

	std::map<std::string, CDialogueChain*>m_MapDigData;
};
