#pragma once
//--------------------------------------
//My Head File
//--------------------------------------
#include"MyNode.h"

template<typename T>
class CBTree
{
public:
	CBTree() {}
	~CBTree() {}

	T GetData(STBTreeNode<T>* Node)
	{
		return Node->GetData();
	}

	void SetData(STBTreeNode<T>* Node, T Data) 
	{
		Node->SetData(Data);	
	}

	STBTreeNode<T> *GetLeftSubTree(STBTreeNode<T>* Node) 
	{
		return Node->m_pLeft();
	}

	STBTreeNode<T> *GetRightSubTree(STBTreeNode<T>* Node) 
	{
		return Node->m_pRight();
	}

	void MakeLeftSubTree(STBTreeNode<T>* MainNode, STBTreeNode<T>* SubNode) 
	{
		if (MainNode->m_pLeft != nullptr)
		{
			delete MainNode->m_pLeft;
		}
		MainNode->m_pLeft = SubNode;
	}

	void MakeRightSubTree(STBTreeNode<T>* MainNode, STBTreeNode<T>* SubNode)
	{
		if (MainNode->m_pRight != nullptr)
		{
			delete MainNode->m_pRight;
		}
		MainNode->m_pRight = SubNode;
	}

	void PreOrderTraverse(STBTreeNode<T>* Node, void(*DoSomeThing)(T data))
	{
		if (Node == NULL)return;

		DoSomeThing(Data);
		PreOrderTraverse(Node->m_pLeft);
		PreOrderTraverse(Node->m_pRight);	
	}

	void InOrderTraverse(STBTreeNode<T>* Node, void(*DoSomeThing)(T Data))
	{
		if (Node == NULL)return;

		InOrderTraverse(Node->m_pLeft);
		DoSomeThing(Data);
		InOrderTraverse(Node->m_pRight);
	}

	void PostOrderTraverse(STBTreeNode<T>* Node, void(*DoSomeThing)(T Data)) 
	{
		if (Node == NULL)return;

		InOrderTraverse(Node->m_pLeft);	
		InOrderTraverse(Node->m_pRight);
		DoSomeThing(Data);
	}

	STBTreeNode<T> GetRoot() 
	{
		return m_pRoot;
	}


	STBTreeNode<T> *m_pRoot;

};







