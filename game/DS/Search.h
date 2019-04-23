#pragma once
#include"MyNode.h"
//using namespace std;

template<typename T>
class CBSTree
{
public:
	CBSTree() :m_pRoot(NULL) {}
	~CBSTree() 
	{
		if (m_pRoot != NULL)
		{
			m_pRoot->DestroyChildren();
			delete m_pRoot;
			m_pRoot = NULL;
		}
	}

	bool InsertData(T InsData) 
	{
		//cout << "aaaa" << endl;

		STBTreeNode<T> *ParPos = nullptr;
		
		STBTreeNode<T> *CurPos = m_pRoot;

		//Find insert Position
		while (CurPos !=nullptr)
		{
			//Get Cur Data
			T CurData = CurPos->GetData();

			//Insert the Same key is forbiddened
			if (CurData == InsData)	return false;

			ParPos = CurPos;

			if (CurData < InsData) 
			{
				CurPos = CurPos->GetRightSubTree();
			}
			else
			{	
				CurPos = CurPos->GetLeftSubTree();				
			}
		}

		STBTreeNode<T> *NewNode = new STBTreeNode<T>(InsData);

		//if it's root node
		if (ParPos == nullptr) 
		{
			m_pRoot = NewNode;
		}
		else
		{
			if (ParPos->GetData() < InsData)
			{
				ParPos->SetRight(NewNode);
			}
			else
			{
				ParPos->SetLeft(NewNode);
			}
		}
	}

	STBTreeNode<T> * SearchNode(T InsData)
	{
		STBTreeNode<T> *CurPos = m_pRoot;

		while (CurPos != nullptr)
		{
			//Get Cur Data
			T CurData = CurPos->GetData();

			//found it
			if (CurData == InsData)	return CurPos;


			if (CurData < InsData)
			{
				CurPos = CurPos->GetLeftSubTree();
			}
			else
			{
				CurPos = CurPos->GetRightSubTree();
			}
		}
	
		return nullptr;
	}

	STBTreeNode<T> * Remove(T DeleteData) 
	{
		STBTreeNode<T> *VNode = new STBTreeNode<T>;	//Virtual Node

		VNode->SetRight(m_pRoot);					//virtula Node is root node's Parent

		STBTreeNode<T> *ParPos = VNode;				//parent Pos Init

		STBTreeNode<T> *CurPos = m_pRoot;			//Current Pos Init

		STBTreeNode<T> *DelNode = m_pRoot;			//The node will be removed
	
	
		//------------------------------------------------------
		//Case 1: Find the node will be removed
		//------------------------------------------------------
		while (CurPos!=nullptr && CurPos->GetData()!= DeleteData)
		{
			ParPos = CurPos;

			//Get Cur Data
			T CurData = CurPos->GetData();

			if (CurData > DeleteData)
			{
				CurPos = CurPos->GetLeftSubTree();
			}
			else
			{
				CurPos = CurPos->GetRightSubTree();
			}
		}

		//-------------------------------------------------------------------------------
		//Case 1: can't find the node will be removed
		//-------------------------------------------------------------------------------
		if (CurPos == nullptr) return nullptr;


		//-----------------------------------------
		//Mark the Node will be deleted
		DelNode = CurPos;


		//-------------------------------------------------------------------------------
		//Case 2: There are no neither left child nor right child in the node will be removed
		//--------------------------------------------------------------------------------
		if (CurPos->GetLeftSubTree()==nullptr && CurPos->GetRightSubTree()==nullptr) 
		{
			//judege which one is the Node will be removed
			if (ParPos->GetLeftSubTree() == DelNode)
			{	
				ParPos->RemoveLeftSubTree();	//remove left tree
			}
			else
			{
				ParPos->RemoveRightSubTree();	//remove right tree
			}
		}

		//--------------------------------------------------------------
		//Case 3: There is only one child in the node will be removed
		//---------------------------------------------------------------
		else if (CurPos->GetLeftSubTree() == nullptr || CurPos->GetRightSubTree() == nullptr) 
		{
			STBTreeNode<T> *DelNode_Child;	//deleted node's child

			//mark deleted node's child
			if (CurPos->GetLeftSubTree() != nullptr) 
			{	
				DelNode_Child = CurPos->GetLeftSubTree();
			}
			else
			{
				DelNode_Child = CurPos->GetRightSubTree();
			}

			//set link between par Node and  deleted node's child
			if (ParPos->GetLeftSubTree() == CurPos) 
			{
				ParPos->ChangeLeftSubTree(DelNode_Child);
			}
			else
			{
				ParPos->ChangeRightSubTree(DelNode_Child);
			}
		}

		//--------------------------------------------------------------
		//Case 4: the node will be removed has Two Children
		//---------------------------------------------------------------
		else if (CurPos->GetLeftSubTree() == nullptr || CurPos->GetRightSubTree() == nullptr)
		{
			STBTreeNode<T> *rpNode = CurPos->GetRightSubTree();		//the node will replace deleted node
			STBTreeNode<T> *rpNodeParent = CurPos;					//the replace node's parent
	
			//find the minimum data in right tree
			while (rpNode!=nullptr)
			{
				rpNodeParent = rpNode;

				rpNode = rpNode->GetLeftSubTree();
			}

			T DeleteData_backup = CurPos->GetData();// back up original data

			CurPos->SetData(rpNode->GetData());	//replace data


			if (rpNodeParent->GetLeftSubTree() == rpNode)
			{
				rpNodeParent->ChangeLeftSubTree(CurPos->GetRightSubTree());
			}
			else
			{
				rpNodeParent->ChangeRightSubTree(CurPos->GetLeftSubTree());
			}

			DelNode = rpNode;

			DelNode->SetData(DeleteData);
		}

		//---------------------------------------------------------------End


		//--------------------------------------------------------------
		//Case if The Root Node has been removed
		//---------------------------------------------------------------		
		if (VNode->GetRightSubTree() != m_pRoot) 
		{
			m_pRoot = VNode->GetRightSubTree();	
		}
			
		delete VNode;

		return DelNode;
	}

	void DoSomeThing(void(*DoSomeThing)(T Data))
	{	
		m_pRoot->InOrderTraverse(DoSomeThing);
	}

	STBTreeNode<T> **GetRootNodeAddress() 
	{	
		return &m_pRoot;
	}
protected:
	STBTreeNode<T> *m_pRoot;
};

template<typename T>
class CBSTree_AVL : public CBSTree<T>
{
public:
	CBSTree_AVL() {}
	virtual ~CBSTree_AVL() {}


	bool InsertData(T InsData) 
	{	
		InsertData_B(&m_pRoot, InsData);

		return true;
	}

	STBTreeNode<T> * Remove(T DeleteData) 
	{
		STBTreeNode<T> *delnode= CBSTree<T>::Remove(DeleteData);

		m_pRoot = ReBlacne(&m_pRoot);

		return delnode;
	}

	int GetTreeHeight() 
	{
		return GetHeight(m_pRoot);	
	}

private:
	void InsertData_B(STBTreeNode<T> ** pNode, T InsData)
	{
		if ((*pNode) == NULL)
		{
			*pNode = new STBTreeNode<T>(InsData);
		}
		else if (InsData < (*pNode)->GetData())
		{
			InsertData_B(&((*pNode)->m_pLeft), InsData);
			*pNode = ReBlacne(pNode);
		}
		else if (InsData >(*pNode)->GetData())
		{
			InsertData_B(&((*pNode)->m_pRight), InsData);
			*pNode = ReBlacne(pNode);
		}
		else
		{
			return ; // 키의 중복을 허용하지 않는다.
		}
	}

	int GetHeight(STBTreeNode<T> *Node)
	{
		int leftHeight;

		int rightHeight;

		if (Node == NULL)return 0;

		leftHeight = GetHeight(Node->GetLeftSubTree());
		rightHeight = GetHeight(Node->GetRightSubTree());

		if (leftHeight > rightHeight)return leftHeight + 1;
		else return rightHeight + 1;
	}

	int GetHeightDiff(STBTreeNode<T> *Node)
	{
		if (Node == NULL)return 0;

		int leftHeight;
		int rightHeight;

		leftHeight = GetHeight(Node->GetLeftSubTree());
		rightHeight = GetHeight(Node->GetRightSubTree());

		return leftHeight - rightHeight;
	}

	STBTreeNode<T> *RotateLL(STBTreeNode<T> *Node)
	{
		STBTreeNode<T> *ParNode = Node;
		STBTreeNode<T> *CurNode = Node->GetLeftSubTree();
	
		ParNode->ChangeLeftSubTree(CurNode->GetRightSubTree());
		CurNode->ChangeRightSubTree(ParNode);

		return CurNode;
	}

	STBTreeNode<T> *RotateRR(STBTreeNode<T> *Node)
	{
		STBTreeNode<T> *ParNode = Node;
		STBTreeNode<T> *CurNode = Node->GetRightSubTree();

		ParNode->ChangeRightSubTree(CurNode->GetLeftSubTree());
		CurNode->ChangeLeftSubTree(ParNode);

		return CurNode;
	}

	STBTreeNode<T> *RotateLR(STBTreeNode<T> *Node)
	{
		STBTreeNode<T> *ParNode = Node;
		STBTreeNode<T> *CurNode = Node->GetLeftSubTree();

		ParNode->ChangeLeftSubTree(RotateRR(CurNode));
		
		return RotateLL(ParNode);
	}

	STBTreeNode<T> *RotateRL(STBTreeNode<T> *Node)
	{
		STBTreeNode<T> *ParNode = Node;
		STBTreeNode<T> *CurNode = Node->GetRightSubTree();

		ParNode->ChangeRightSubTree(RotateLL(CurNode));
	
		return RotateRR(ParNode);
	}

	STBTreeNode<T> *ReBlacne(STBTreeNode<T> **Node) 
	{
		

		int HDiff = GetHeightDiff(*Node);
	
		if (HDiff >1) 
		{
			if (GetHeightDiff((*Node)->GetLeftSubTree()) > 0) 
			{	
				*Node = RotateLL(*Node);
			}
			else
			{
				*Node = RotateLR(*Node);
			}
		}
		if(HDiff<-1)
		{
			if (GetHeightDiff((*Node)->GetRightSubTree()) < 0)
			{
				*Node = RotateRR(*Node);
			}
			else
			{
				*Node = RotateRL(*Node);
			}
		}

		return *Node;
	}
};




