#pragma once
#include<Windows.h>
#include<string>
#include<map>
#include<algorithm>
#include<list>

template<typename T>
void deleteMapValue(std::pair<std::string, T*> Data)
{
	if (Data.second != NULL)
	{
		delete Data.second;
		Data.second = NULL;
	}
}


template<typename T>
class CDataContainer
{
public:
	CDataContainer()
	{
	}
	~CDataContainer()
	{
		ClearData();
	}

	void AddData(LPCSTR StrKey, T* ValueData)
	{
		m_MAP.insert(std::pair<std::string, T*>(StrKey, ValueData));
	}

	T* GetData(LPCSTR StrKey)
	{
		std::map<std::string, T*>::iterator iter = m_MAP.find(StrKey);
		if (iter == m_MAP.end())return NULL;
		else return (*iter).second;
	}
	DWORD GetSize() const
	{
		return m_MAP.size();
	}


	void ClearData()
	{
		for_each(m_MAP.begin(), m_MAP.end(), deleteMapValue<T>);
		m_MAP.clear();
	}

	void GetAllKey(std::list<std::string>&Getlist)
	{
		std::map<std::string, T*>::iterator iter = m_MAP.begin();
		while (iter != m_MAP.end()) 
		{
			Getlist.push_back((*iter).first);
			++iter;
		}
	}
	void GetAllValue(std::list<T*>&Getlist)
	{
		std::map<std::string, T*>::iterator iter = m_MAP.begin();
		while (iter != m_MAP.end())
		{
			Getlist.push_back((*iter).second);
			++iter;
		}
	}


private:
	std::map<std::string, T*> m_MAP;
};
