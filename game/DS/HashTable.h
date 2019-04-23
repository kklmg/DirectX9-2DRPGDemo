#pragma once
#include"LinkList.h"

//size_t
template<typename TValue=int>
struct STSlot
{
	STSlot() :m_nKey(0) {}

	STSlot(int key, TValue value) :m_nKey(key), m_Value(value) {}

	void operator=(STSlot &slot)
	{
		m_nKey = slot.m_nKey;
		m_Value = slot.m_Value;
	}

	DWORD m_nKey;
	TValue m_Value;
};



template<typename TValue,int size>
class CHashTable
{
public:
	CHashTable() :m_fptrHashFun(__DefaultHashFun) {}

	CHashTable(DWORD(*HashFun)(DWORD)):m_fptrHashFun(HashFun)
	{
		m_Table = new CLinkList<STSlot<TValue>>[Volume];
	}
	~CHashTable() {}
	//template <>
	bool Insert(DWORD key, TValue value) 
	{
		if (Search(key)) return FALSE;	//key duplicated

		int HashKey= m_fptrHashFun(key);
		
		STSlot<TValue> slot(key, value);

		m_pTable[HashKey].Insert(slot);

		return true;
	}

	bool Delete(DWORD key)
	{
		STSlot<TValue> slot;
		int HashKey = m_fptrHashFun(key);//calculate hash key

		if (m_pTable[HashKey].RemoveIf<DWORD>(IsSameKey, key, slot))
		{
			return TRUE;
		}
		else return FALSE;
	}

	bool Delete(DWORD key, TValue &GetValue)
	{
		STSlot<TValue> slot;
		int HashKey = m_fptrHashFun(key);//calculate hash key

		if (m_pTable[HashKey].RemoveIf<int>(IsSameKey, key, slot)) 
		{
			GetValue = slot.m_Value;
			return TRUE;
		}
		else return FALSE;
	}

	bool Search(DWORD key)
	{
		STSlot<TValue> slot;

		int HashKey = m_fptrHashFun(key);//calculate hash key

		if (m_pTable[HashKey].First(slot))
		{
			if (slot.m_nKey == key)
			{
				return TRUE;
			}
			while (m_pTable[HashKey].Next(slot))
			{
				if (slot.m_nKey == key)
				{
					return TRUE;
				}
			}
		}
		return FALSE;
	}

	bool Search(DWORD key,TValue &GetValue)
	{
		STSlot<TValue> slot;

		int HashKey = m_fptrHashFun(key);//calculate hash key
		
		if (m_pTable[HashKey].First(slot))
		{
			if (slot.m_nKey == key)
			{
				GetValue = slot.m_Value;
				return TRUE;
			}
			while (m_pTable[HashKey].Next(slot))
			{
				if (slot.m_nKey == key)
				{
					GetValue = slot.m_Value;
					return TRUE;
				}
			}
		}
		return FALSE;
	}

	void Traversal(void(*doSomething)(STSlot<TValue>))
	{
		for (int i = 0; i < size; i++) 
		{
			m_pTable[i].Traversal(doSomething);	
		}
	}


	static DWORD  __DefaultHashFun(DWORD key)
	{
		return key % size;
	}

	static bool IsSameKey(STSlot<TValue> slot, DWORD key)
	{
		if (slot.m_nKey == key)return TRUE;
		else return FALSE;
	}

private:
	DWORD GetHK(int key) 
	{
		return m_fptrHashFun(key);
	}

	//STSlot<int> a[10];
	CLinkList<STSlot<TValue>> m_pTable[size];
	CLinkList<STSlot<int>> m_pTable1[10];

	DWORD (*m_fptrHashFun)(DWORD key);
};

