#pragma once
#include<ddraw.h>
#include<string>
#include"DialogLexer.h"
#include<map>
#include<list>
#include<vector>

struct STDialog
{
	STDialog() {}
	STDialog(LPCSTR Character, LPCSTR Dialog)
	{
		m_strCharacter = Character;
		m_strDiag = Dialog;
	}
	~STDialog() 
	{
		m_strCharacter.clear();
		m_strDiag.clear();
	}

	std::string m_strCharacter;
	std::string m_strDiag;
};

class CDialogChain 
{
public:
	CDialogChain();
	~CDialogChain();

	void Insert(STDialog*);

	STDialog* getDialog(int index);

	int GetSize() const;

private:
	std::vector<STDialog*> m_vectorData;
};

class CDialogDataMng
{
public:
	CDialogDataMng();
	~CDialogDataMng();

	bool Parsing(LPCSTR str);

	CDialogChain* GetDialogChain(LPCSTR);

private:
	bool NextToken();
	bool NextBlockStart();
	bool NextBlockEnd();
	bool FindToken(int FindToken);
	bool FindToken_InBlock(int FindToken);

private:
	int m_nbkcount;

	DIALOG::CLexer *m_plex;

	char m_acGetString[256];
	int m_nTokenType;

	

	std::map<std::string, CDialogChain*>m_MapDigData;
};
