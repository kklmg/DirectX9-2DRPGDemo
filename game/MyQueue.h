#pragma once
//--------------------------------------
//Head File
//--------------------------------------
#include<Windows.h>



//--------------------------------------
//class Node
//--------------------------------------
template<typename T>
class CNode
{
public:
	CNode(CNode*front, CNode*next, T Data)
		:m_pFront(front), m_pNext(next), m_Data(Data) {}
	~CNode() { m_pFront = NULL; m_pNext=NULL}

	CNode* m_pFront;

	CNode* m_pNext;

	T GetData()const 
	{
		return m_Data;
	}
private:
	T m_Data;
};




 //--------------------------------------
 //class Queue
 //--------------------------------------
template<typename T>
class CQueue 
{
public:
	CQueue() :m_nDataCount(0) {}
	virtual~CQueue() {}

	virtual bool IsEmpty() = 0;

	virtual bool Enque(T data) = 0;

	virtual bool Deque(T &GetData) = 0;

	virtual bool Peek(T &GetData) const= 0;

private:
	int m_nDataCount;
};


//-------------------------------------------------
//class Array Queue
//-------------------------------------------------
template<typename T>
class CArrQueue :public CQueue<T>
{
public:
	CArrQueue(DWORD volume) :m_pArr(NULL) : m_nfront(0), m_nRear(0)
	{
		m_pArr = new T[volume + 1];
		m_nDataCount = volume + 1;
	}

	~CArrQueue()
	{
		if (m_pArr != NULL)
		{
			delete[] m_pArr;
			m_pArr = NULL;
		}
	}

	virtual bool IsEmpty()
	{
		if (m_nRear == m_nfront)return true;
		else return false;
	}

	virtual bool Enque(T data)
	{
		if (NextPosIdx(m_nRear) == m_nfront)
			return false;	//array full!!

		m_nRear = NextPosIdx(m_nRear);	//move rear
		m_pArr[m_nRear] = data;			// save Data

		return true;
	}

	virtual bool Deque(T &GetData)
	{
		if (IsEmpty())	return false;//array empty
		else
		{
			GetData= m_pArr[m_nfront];		//get value		
			m_nfront= NextPosIdx(m_nRear);	//Move front
			return true;	
		}
	}

	virtual bool Peek(T &GetData) const
	{	
		if (IsEmpty())	return false;//array empty
		else
		{
			GetData = m_pArr[m_nfront];		//get value		
			return true;
		}
	}

private:
	//move to next 
	int NextPosIdx(int curPos) 
	{
		if (curPos + 1 == m_nDataCount)
			return 0;
		else return curPos + 1;
	}

	T* m_pArr;

	

	int m_nfront;
	int m_nRear;
};


//-------------------------------------------------
//class LinkList Queue
//-------------------------------------------------
template<typename T>
class CListQueue :public CQueue<T>
{
public:
	CListQueue()
		:m_pFront(NULL), m_pRear(NULL) {}
	virtual~CListQueue() 
	{
		while (Deque);//deque all data

	}

	virtual bool IsEmpty() 
	{
		if (m_pFront == NULL)return true;
		else return false;
	}

	virtual bool Enque(T data) 
	{
		if (IsEmpty()) 
		{
			CNode<T> *newNode = new CNode<T>(NULL, NULL, data);

			m_pFront = newNode;
			m_pRear = newNode;
		}
		else 
		{
			CNode<T> *newNode = new CNode<T>(m_pRear, NULL, data);

			m_pFront->m_pNext = newNode;		

			m_pRear = newNode;		
		}
	}

	virtual bool Deque(T &GetData)
	{
		if (IsEmpty())	return false;//array empty
		else
		{		
			GetData = m_pFront->GetData();		//get value	

			CNode<T> *delNode = m_pFront;		//mark delete node

			m_pFront = m_pFront->m_pNext;		//front move to next

			delete delNode;						//Delete Node

			return true;
		}
	}

	virtual bool Peek(T &GetData) const
	{
		if (IsEmpty())	return false;			//array empty
		else
		{
			GetData = m_pFront->GetData();		//get value		
			return true;
		}
	}

private:
	CNode<T> *m_pFront;
	CNode<T> *m_pRear;
};



//-------------------------------------------------
//Deque : Double-ended Queue
//Base	: Doublely linked List
//-------------------------------------------------
template<typename T>
class CDeque
{
public:
	CDeque() 
	: m_pHead(NULL), m_pTail(NULL){}

	virtual~CDeque() 
	{
		while (RemoveFirst);
	}

	bool IsEmpty() 
	{
		if (m_pHead == NULL)return TRUE;
		else return FALSE;
	}

	bool AddFirst(T data) 
	{	
		CNode<T> *newNode = new CNode<T>(NULL, m_pHead, data);
		if (IsEmpty()) 
		{	
			m_pHead = newNode;
			m_pTail = newNode;				
		}
		else
		{
			m_pHead->m_pFront = newNode;
			m_pHead = newNode;
		}

	
		return TRUE;
	}

	bool AddLast(T data) 
	{
		CNode<T> *newNode = new CNode<T>(NULL, NULL, data);

		if (IsEmpty())
		{
			m_pHead = newNode;
			m_pTail = newNode;
		}
		else
		{
			newNode->m_pFront = m_pTail;

			m_pTail->m_pNext = newNode;

			m_pTail = newNode;
		}

		return TRUE;



	}

	bool RemoveFirst(T &data)
	{
		if (IsEmpty())return FALSE;		//deque empty!!

		data = m_pHead.GetData();		//get Data
			
		CNode<T>* delNode = m_pHead;	//mark delete node	
	
		m_pHead = m_pHead->m_pNext;		//move head

		delete delNode;					//delete node

		if (m_pHead == NULL)			//container empty 
		{
			m_pTail = NULL;				
		}
		else
		{
			m_pHead->m_pFront = NULL;	//next node's front node set to null
		}
	}

	bool RemoveLast(T &data)
	{
		if (IsEmpty())return FALSE;		//deque empty!!

		data = m_pTail.GetData();		//get Data

		CNode<T>* delNode = m_pTail;	//mark delete node	

		m_pTail = m_pTail->m_pFront;	//move tail

		delete delNode;					//delete node

		if (m_pTail == NULL)			//container empty 
		{
			m_pHead = NULL;
		}
		else
		{
			m_pTail->m_pNext = NULL;	//next node's front node set to null
		}

	}

	bool GetFirst(T &data)const  
	{
		if (IsEmpty)return FALSE;

		data = m_pHead.GetData();		//get Data

		return TRUE;
	}

	bool GetLast(T &data)const 
	{
		if (IsEmpty)return FALSE;

		data = m_pTail.GetData();		//get Data

		return TRUE;

	}
private:
	CNode<T> *m_pHead;
	CNode<T> *m_pTail;
};



//-------------------------------------------------
//Heap 
//-------------------------------------------------
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


//-------------------------------------------------
//Priority queue 
//-------------------------------------------------
template<typename T>
class CPriorityQueue
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