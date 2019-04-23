#pragma once
#include"MyNode.h"

template<typename T>
class CLinkList
{
public:
	CLinkList():m_pHead(nullptr),m_pCur(nullptr), m_pBefore(nullptr) ,m_nNumData(0), m_fptrCompare(nullptr)
	{
		m_pHead = new STNode_OC<T>;
	
		m_pHead->m_pChild = nullptr;
	
	}
	~CLinkList() 
	{
		Destroy();	
	}

	//----------------------------------------------------
	//Insert Data 
	//----------------------------------------------------
	void Insert(T Data) 
	{
		if (m_fptrCompare == nullptr) 
		{	
			Insert_front(Data);
		}
		else 
		{
			Insert_sort(Data);
		}
		++m_nNumData;
	}

	//----------------------------------------------------
	//Get First Data
	//----------------------------------------------------
	bool First()
	{
		//If No any Data,return false
		if (m_pHead->m_pChild == nullptr)
			return false;

		m_pBefore = m_pHead;
		m_pCur = m_pHead->m_pChild;

		return true;
	}

	bool First(T &GetData)
	{
		//If No any Data,return false
		if (m_pHead->m_pChild == nullptr)
				return false;
		

		m_pBefore = m_pHead;
		m_pCur = m_pHead->m_pChild;

		GetData = m_pCur->GetData();

		return true;
	}

	//----------------------------------------------------
	//Get Next Data
	//----------------------------------------------------
	bool Next()
	{
		if (m_pCur->m_pChild == nullptr)return false;

		m_pBefore = m_pCur;
		m_pCur = m_pCur->m_pChild;

		return true;
	}
	bool Next(T &GetData)
	{
		if (m_pCur->m_pChild == nullptr)return false;

		m_pBefore = m_pCur;
		m_pCur = m_pCur->m_pChild;

		GetData = m_pCur->GetData();

		return true;
	}

	//----------------------------------------------------
	//Remove Data
	//----------------------------------------------------
	bool Remove(T Data)
	{
		ResetPtr();

		T CurData;

		while (Next(CurData))
		{		
			if (CurData == Data)
			{
				STNode_OC<T> *DeleteNode = m_pCur;

				m_pBefore->m_pChild = m_pCur->m_pChild;

				m_pCur = m_pBefore;

				delete DeleteNode;

				return TRUE;
			}
		}
		return FALSE;
	}
	bool Remove(T Data,T &GetData)
	{
		ResetPtr();

		while (Next(GetData))
		{
			if (GetData == Data)
			{
				STNode_OC<T> *DeleteNode = m_pCur;

				m_pBefore->m_pChild = m_pCur->m_pChild;

				m_pCur = m_pBefore;

				delete DeleteNode;

				return TRUE;
			}
		}
		return FALSE;
	}

	template<typename T2>
	bool RemoveIf(bool (*condition)(T Data1, T2 Data2),T2 CompData,T &GetData)
	{
		ResetPtr();

		while (Next(GetData))
		{
			if (condition(GetData, CompData))
			{
				STNode_OC<T> *DeleteNode = m_pCur;

				m_pBefore->m_pChild = m_pCur->m_pChild;

				m_pCur = m_pBefore;

				delete DeleteNode;

				return TRUE;
			}
		}
		return FALSE;	
	}

	void Traversal(void(*DoSomething)(T data)) 
	{
		ResetPtr();
		while (Next())
		{
			DoSomething(m_pCur->GetData());
		}	
	}

	void Destroy() 
	{
		ResetPtr();

		while (Next())
		{	
			STNode_OC<T> *DeleteNode = m_pCur;

			m_pBefore->m_pChild = m_pCur->m_pChild;

			m_pCur = m_pBefore;

			delete DeleteNode;
		}
		
		delete m_pHead;
	}
	//----------------------------------------------------
	//Get num of data
	//----------------------------------------------------
	int GetCount()
	{
		return m_nNumData;
	}

	//----------------------------------------------------
	//Set Sort Rule
	//----------------------------------------------------
	void SetSortRule(bool(*Compare)(T, T)) 
	{
		m_fptrCompare = Compare;
	}

	void DoSomeThing(void(*DoSome)(T Data)) 
	{
		ResetPtr();
		T CurData;
		while (Next(CurData))
		{
			DoSome(CurData);
		}	
	}

private:
	//----------------------------------------------------
	//ptr refer first Data
	//----------------------------------------------------
	void ResetPtr() 
	{
		m_pBefore = m_pHead;
		m_pCur = m_pHead;
	}

	//----------------------------------------------------
	//Insert Data At front of list
	//----------------------------------------------------
	void Insert_front(T Data)
	{
		STNode_OC<T> *NewNode = new STNode_OC<T>(Data);

		NewNode->m_pChild = m_pHead->m_pChild;
		m_pHead->m_pChild = NewNode;
	}

	//----------------------------------------------------
	//Insert Data ,Keep Sort Rule
	//----------------------------------------------------
	void Insert_sort(T Data)
	{
		STNode_OC<T> *NewNode = new STNode_OC<T>(Data);

		STNode_OC<T> *preNode = m_pHead;

		while (preNode->m_pChild!=nullptr && m_fptrCompare(Data, preNode->m_pChild->GetData()))
		{
			preNode = preNode->m_pChild;
		}
		NewNode->m_pChild = preNode->m_pChild;
		preNode->m_pChild = NewNode;
	}

private:

	unsigned int m_nNumData;

	bool(*m_fptrCompare)(T Data_A, T Data_B);

	STNode_OC<T> *m_pHead;
	STNode_OC<T> *m_pCur;
	STNode_OC<T> *m_pBefore;
};

