#pragma once
//--------------------------------------
//My Head File
//--------------------------------------
#include"MyNode.h"

//--------------------------------------
//class Queue
//--------------------------------------
template<typename T>
class CQueue
{
public:
	CQueue() :m_nDataCount(0) {}
	virtual~CQueue() {}

	//----------------------------------------------------------
	//IsEmpty(): Check If Queue Is Empty
	//----------------------------------------------------------
	virtual bool IsEmpty() = 0;

	//----------------------------------------------------------
	//Enque(T data): Insert Data to Queue
	//----------------------------------------------------------
	virtual bool Enque(T data) = 0;

	//----------------------------------------------------------
	//Deque(): Delete Data Froem Queue 
	//----------------------------------------------------------
	virtual bool Deque() = 0;

	//----------------------------------------------------------
	//Deque(T data): Delete Data Froem Queue and Get The Data
	//----------------------------------------------------------
	virtual bool Deque(T &GetData) = 0;
	
	//----------------------------------------------------------
	//Peek(T data): Get The Data From Queue
	//----------------------------------------------------------
	virtual bool Peek(T &GetData) = 0;

protected:
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

	//----------------------------------------------------------
	//IsEmpty(): Check If Queue Is Empty
	//----------------------------------------------------------
	virtual bool IsEmpty()
	{
		if (m_nRear == m_nfront)return true;
		else return false;
	}

	//----------------------------------------------------------
	//Enque(T data): Insert Data to Queue
	//----------------------------------------------------------
	virtual bool Enque(T data)
	{
		if (NextPosIdx(m_nRear) == m_nfront)
			return false;	//array full!!

		m_nRear = NextPosIdx(m_nRear);	//move rear
		m_pArr[m_nRear] = data;			// save Data

		return true;
	}

	//----------------------------------------------------------
	//Deque(): Delete Data Froem Queue 
	//----------------------------------------------------------
	virtual bool Deque()
	{
		if (IsEmpty())	return false;//array empty
		else
		{
			m_nfront = NextPosIdx(m_nRear);	//Move front
			return true;
		}
	}
	//----------------------------------------------------------
	//Deque(T data): Delete Data Froem Queue and Get The Data
	//----------------------------------------------------------
	virtual bool Deque(T &GetData)
	{
		if (IsEmpty())	return false;//array empty
		else
		{
			GetData = m_pArr[m_nfront];		//get value		
			m_nfront = NextPosIdx(m_nRear);	//Move front
			return true;
		}
	}

	//----------------------------------------------------------
	//Peek(T data): Get The Data From Queue
	//----------------------------------------------------------
	virtual bool Peek(T &GetData) 
	{
		if (IsEmpty())	return false;//array empty
		else
		{
			GetData = m_pArr[m_nfront];		//get value		
			return true;
		}
	}

private:
	//----------------------------------------------------------
	//int NextPosIdx(int curPos): Move Cur Position to next
	//----------------------------------------------------------
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
		while (Deque()) {}//deque all data
	}

	//-------------------------------------------
	//IsEmpty(): Check If Queue Is Empty
	//-------------------------------------------
	virtual bool IsEmpty()
	{
		if (m_pFront == NULL)return true;
		else return false;
	}

	//----------------------------------------------------------
	//Enque(T data): Insert Data to Queue
	//----------------------------------------------------------
	virtual bool Enque(T data)
	{
		if (IsEmpty())
		{
			STNode_OC<T> *newNode = new STNode_OC<T>(data);

			m_pFront = newNode;
			m_pRear = newNode;
			
		}
		else
		{
			STNode_OC<T> *newNode = new STNode_OC<T>(data);

			m_pRear->m_pChild = newNode;

			m_pRear = newNode;		
		}

		++m_nDataCount;

		return true;
	}

	//----------------------------------------------------------
	//Deque(): Delete Data Froem Queue 
	//----------------------------------------------------------
	virtual bool Deque()
	{
		if (IsEmpty())	return false;//array empty
		else
		{
			STNode_OC<T> *delNode = m_pFront;		//mark delete node

			m_pFront = m_pFront->m_pChild;		//front move to next

			delete delNode;						//Delete Node

			--m_nDataCount;

			return true;
		}
	}

	//----------------------------------------------------------
	//Deque(T data): Delete Data Froem Queue and Get The Data
	//----------------------------------------------------------
	virtual bool Deque(T &GetData)
	{
		if (IsEmpty())	return false;//array empty
		else
		{
			GetData = m_pFront->GetData();		//get value	

			STNode_OC<T> *delNode = m_pFront;		//mark delete node

			m_pFront = m_pFront->m_pChild;		//front move to next

			delete delNode;						//Delete Node

			return true;
		}
	}

	//----------------------------------------------------------
	//Peek(T data): Get The Data From Queue
	//----------------------------------------------------------
	virtual bool Peek(T &GetData) 
	{
		if (IsEmpty())	return false;			//array empty
		else
		{
			GetData = m_pFront->GetData();		//get value		
			return true;
		}
	}

private:
	STNode_OC<T> *m_pFront;
	STNode_OC<T> *m_pRear;
};



//-------------------------------------------------
//Deque : Double-ended Queue
//Base  : Doublely linked List
//-------------------------------------------------
template<typename T>
class CDeque
{
public:
	CDeque()
		: m_pHead(NULL), m_pTail(NULL) {}

	virtual~CDeque()
	{
		while (RemoveFirst());
	}
	//----------------------------------------------------------
	//IsEmpty(): Check If Queue Is Empty
	//----------------------------------------------------------
	bool IsEmpty()
	{
		if (m_pHead == NULL)return TRUE;
		else return FALSE;
	}

	//----------------------------------------------------------
	// AddFirst(T data):Add Data At Front
	//----------------------------------------------------------
	bool AddFirst(T data)
	{
		STNode_OPOC<T> *newNode = new STNode_OPOC<T>(NULL, m_pHead, data);
		if (IsEmpty())
		{
			m_pHead = newNode;
			m_pTail = newNode;
		}
		else
		{
			m_pHead->m_pParent = newNode;
			m_pHead = newNode;
		}


		return TRUE;
	}

	//----------------------------------------------------------
	//AddLast(T data):Add Data At Last
	//----------------------------------------------------------
	bool AddLast(T data)
	{
		STNode_OPOC<T> *newNode = new STNode_OPOC<T>(NULL, NULL, data);

		if (IsEmpty())
		{
			m_pHead = newNode;
			m_pTail = newNode;
		}
		else
		{
			newNode->m_pParent = m_pTail;

			m_pTail->m_pChild = newNode;

			m_pTail = newNode;
		}

		return TRUE;



	}

	//----------------------------------------------------------
	//RemoveFirst():Remove First Data
	//----------------------------------------------------------
	bool RemoveFirst()
	{
		if (IsEmpty())return FALSE;		//deque empty!!

		STNode_OPOC<T>* delNode = m_pHead;	//mark delete node	

		m_pHead = m_pHead->m_pChild;		//move head

		delete delNode;					//delete node

		if (m_pHead == NULL)			//container empty 
		{
			m_pTail = NULL;
		}
		else
		{
			m_pHead->m_pParent = NULL;	//next node's front node set to null
		}
	}
	//----------------------------------------------------------
	//RemoveFirst(T &data):Remove First Data and get the Data
	//----------------------------------------------------------
	bool RemoveFirst(T &data)
	{
		if (IsEmpty())return FALSE;		//deque empty!!

		data = m_pHead.GetData();		//get Data

		STNode_OPOC<T>* delNode = m_pHead;	//mark delete node	

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

	//----------------------------------------------------------
	//RemoveLast():Remove Last Data
	//----------------------------------------------------------
	bool RemoveLast()
	{
		if (IsEmpty())return FALSE;		//deque empty!!

		STNode_OPOC<T>* delNode = m_pTail;	//mark delete node	

		m_pTail = m_pTail->m_pParent;	//move tail

		delete delNode;					//delete node

		if (m_pTail == NULL)			//container empty 
		{
			m_pHead = NULL;
		}
		else
		{
			m_pTail->m_pChild = NULL;	//next node's front node set to null
		}

	}
	//----------------------------------------------------------
	//RemoveLast(T &data):Remove Last Data and get the Data
	//----------------------------------------------------------
	bool RemoveLast(T &data)
	{
		if (IsEmpty())return FALSE;		//deque empty!!

		data = m_pTail->GetData();		//get Data

		STNode_OPOC<T>* delNode = m_pTail;	//mark delete node	

		m_pTail = m_pTail->m_pParent;	//move tail

		delete delNode;					//delete node

		if (m_pTail == NULL)			//container empty 
		{
			m_pHead = NULL;
		}
		else
		{
			m_pTail->m_pChild = NULL;	//next node's front node set to null
		}

	}

	//----------------------------------------------------------
	//GetFirst(T &data):Get First Data
	//----------------------------------------------------------
	bool GetFirst(T &data)
	{
		if (IsEmpty())return FALSE;

		data = m_pHead->GetData();		//get Data

		return TRUE;
	}
	//----------------------------------------------------------
	//GetLast(T &data):Get Last Data
	//----------------------------------------------------------
	bool GetLast(T &data)
	{
		if (IsEmpty())return FALSE;

		data = m_pTail->GetData();		//get Data

		return TRUE;

	}

private:
	STNode_OPOC<T> *m_pHead;
	STNode_OPOC<T> *m_pTail;
};

