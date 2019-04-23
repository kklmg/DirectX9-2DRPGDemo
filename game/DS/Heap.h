#pragma once
#include"queue.h"
template<typename T>
class CHeap
{
public:
	CHeap(unsigned int Volume, bool(*fptrCompare)(T, T)) :m_pArr(NULL), m_nNumOfData(0), m_fptrCompare(fptrCompare)
	{
		m_pArr = new T[Volume];
	}
	~CHeap()
	{
		if (m_pArr != NULL)
		{
			delete[] m_pArr;
			m_pArr = NULL;
		}
	}

	bool IsEmpty()
	{
		if (m_nNumOfData == 0)return TRUE;
		else return FALSE;
	}

	bool DeleteData()
	{
		if (IsEmpty())	return FALSE;		//you can't delete data , the heap is empty 

		T LastData = m_pArr[m_nNumOfData];	//put the last data to root

		int ParentIndex = 1;				//parnet index init

		int ChildIndex;						//child index

		while (ChildIndex = GetPriChildIndex(ParentIndex))		//get the prior child
		{
			if (m_fptrCompare(m_pArr[ChildIndex], m_pArr[ParentIndex]))		//compare the priority
				break;		//the child's priority 

			m_pArr[ParentIndex] = m_pArr[ChildIndex];			//switch Data	

			ParentIndex = ChildIndex;								//Get 
		}
		m_pArr[ParentIndex] = LastData;

		--m_nNumOfData;
		return TRUE;
	}

	bool DeleteData(T &GetData)
	{
		if (IsEmpty())	return FALSE;		//you can't delete data , the heap is empty 

		GetData = m_pArr[1];				//get the data in root

		T LastData = m_pArr[m_nNumOfData];	//put the last data to root

		int ParentIndex = 1;				//parnet index init

		int ChildIndex;						//child index

		while (ChildIndex = GetPriChildIndex(ParentIndex))		//get the prior child
		{
			if (m_fptrCompare(m_pArr[ChildIndex], m_pArr[ParentIndex]))		//compare the priority
				break;		//the child's priority 

			m_pArr[ParentIndex] = m_pArr[ChildIndex];			//switch Data	

			ParentIndex = ChildIndex;								//Get 
		}
		m_pArr[ParentIndex] = LastData;

		--m_nNumOfData;
		return TRUE;
	}

	bool InsertData(T InsertData)
	{
		int Index = m_nNumOfData + 1;

		while (Index != 1)
		{

			if (m_fptrCompare(m_pArr[GetParentIndex(Index)], InsertData)) break;

			else
			{
				m_pArr[Index] = m_pArr[GetParentIndex(Index)];

				Index = GetParentIndex(Index);

			}
		}

		m_pArr[Index] = InsertData;

		++m_nNumOfData;
		return TRUE;
	}

	bool PeekData(T &GetData)
	{
		if (IsEmpty())return FALSE;
		else
		{
			GetData = m_pArr[1];
			return TRUE;
		}
	}

private:
	int GetParentIndex(int Index)
	{
		return Index / 2;
	}

	int GetChildIndex_Left(int Index)
	{
		return Index * 2;

	}

	int GetChildIndex_Right(int Index)
	{
		return (Index * 2) + 1;
	}

	int GetPriChildIndex(int Index)
	{
		int Index_Lc = GetChildIndex_Left(Index);	//Left chld index
		int Index_Rc = GetChildIndex_Right(Index);	//right child index


		if (Index_Lc > m_nNumOfData)return 0;	//can;t find child

		if (Index_Lc == m_nNumOfData)			//there is only one child and it's index equal to num of data
		{
			return Index_Lc;
		}
		else
		{
			if (m_fptrCompare(m_pArr[Index_Lc], m_pArr[Index_Rc]))	//compare priority and return 
				return Index_Lc;
			else return Index_Rc;
		}
	}

private:
	bool(*m_fptrCompare)(T Data_A, T Data_B);

	T* m_pArr;

	unsigned int m_nNumOfData;
};

template<typename T>
class CPriorityQueue: public CQueue<T>
{
public:
	CPriorityQueue(unsigned int Volume, bool(*fptrCompare)(T, T)) :m_heap(Volume, fptrCompare)
	{
	}
	virtual~CPriorityQueue() {}

	virtual bool IsEmpty()
	{
		return m_heap.IsEmpty();
	}

	virtual bool Enque(T data)
	{
		return m_heap.InsertData(data);
	}

	virtual bool Deque() 
	{
		return m_heap.DeleteData();
	}

	virtual bool Deque(T &GetData)
	{
		return m_heap.DeleteData(GetData);
	}

	virtual bool Peek(T &GetData)
	{
		return m_heap.PeekData(GetData);
	}

private:
	CHeap<T> m_heap;
};
