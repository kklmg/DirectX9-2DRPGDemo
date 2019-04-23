#pragma once
//--------------------------------------
//Struct Node Base
//--------------------------------------
template<typename T>
struct STNodeBasic 
{
	STNodeBasic() {}

	STNodeBasic(T Data)
	{
		SetData(Data);
	}

	void SetData(T Data)
	{
		m_Data = Data;
	}

	T GetData()const
	{
		return m_Data;
	}
private:
	T m_Data;
};



//--------------------------------------
//Node: Common Node
//LInk:	One Child
//--------------------------------------
template<typename T>
struct STNode_OC :public STNodeBasic<T>
{
	STNode_OC()
		: m_pChild(NULL) {}

	STNode_OC(T Data)
		: STNodeBasic<T>(Data), m_pChild(NULL) {}

	STNode_OC(STNode_OC*child, T Data)
		: STNodeBasic<T>(Data),m_pChild(child) {}

	//child Node
	STNode_OC* m_pChild;
};

//--------------------------------------
//Node: Common Node
//LInk: One Parent,One Child
//--------------------------------------
template<typename T>
struct STNode_OPOC :public STNodeBasic<T>
{
	STNode_OPOC() :m_pParent(NULL) {}

	STNode_OPOC(T Data) :STNodeBasic<T>(Data), m_pParent(NULL) {}

	STNode_OPOC(STNode_OPOC*parent, STNode_OPOC*Child, T Data)
		:STNodeBasic<T>(Data),m_pParent(parent), m_pChild(Child){}

	//Parent Node
	STNode_OPOC* m_pParent;
	//child Node
	STNode_OPOC* m_pChild;
};


//--------------------------------------
//Node: Common Node
//LInk:	MultiChildren
//--------------------------------------
template<typename T>
struct STNode_MC :public STNodeBasic<T>
{
	STNode_MC() :m_ppChildren(NULL),m_nNumOfChildren(0) {}

	STNode_MC(int NumChildren):
	{
		InitChildren()
	}

	~STNode_MC() 
	{
		delete[] m_ppChildren;
	}


	void InitChildren(unsigned int NumChildren) 
	{
		if (NumChildren == 0)return;

		m_nNumOfChildren = NumChildren;

		m_ppChildren = new STNode_MC*[NumChildren];

		for (int i = 0; i < NumChildren; ++i)
		{
			m_ppChildren[i] = NULL;
		}
	
	}

	//Num Of Children
	unsigned int m_nNumOfChildren;
	//Children Node
	STNode_MC** m_ppChildren;
};

//--------------------------------------
//Node: Common Node
//LInk:	One Parent MultiChildren
//--------------------------------------
template<typename T>
struct STNode_OPMC :public STNodeBasic<T>
{
	STNode_OPMC() :m_pParent(NULL), m_ppChildren(NULL), m_nNumOfChildren(0) {}
	STNode_OPMC(int NumChildren, STNode_OPMC *parent)
		:m_pParent(parent)
	{	
		InitChildren(NumChildren);
	}

	~STNode_OPMC() { delete[] m_ppChildren; }


	//Set Children count
	void InitChildren(unsigned int NumChildren)
	{
		if (NumChildren == 0)return;

		m_nNumOfChildren = NumChildren;

		m_ppChildren = new STNode_MC*[NumChildren];

		for (int i = 0; i < NumChildren; ++i)
		{
			m_ppChildren[i] = NULL;
		}

	}
	//Num Of Children
	unsigned int m_nNumOfChildren;
	//Parent Node
	STNode_OPMC* m_pParent;
	//Children Node
	STNode_MC** m_ppChildren;
};


//--------------------------------------
//Node: Binary Tree Node
//LInk: Two Children
//--------------------------------------
template<typename T>
struct STBTreeNode : public STNodeBasic<T>
{
	STBTreeNode() :m_pLeft(NULL), m_pRight(NULL) {}
	STBTreeNode(T Data) :STNodeBasic<T>(Data),m_pLeft(NULL), m_pRight(NULL) {}

	STBTreeNode* m_pLeft;
	STBTreeNode* m_pRight;

	void SetLeft(STBTreeNode* LeftNode) 
	{
		if (m_pLeft != NULL) delete m_pLeft;

		m_pLeft = LeftNode;
	}
	STBTreeNode *GetLeftSubTree()
	{
		return m_pLeft;
	}
	void ChangeLeftSubTree(STBTreeNode* LeftNode)
	{
		m_pLeft = LeftNode;
	}
	STBTreeNode *RemoveLeftSubTree()
	{
		if (this != NULL)
		{
			STBTreeNode * DelNode = m_pLeft;
			m_pLeft = NULL;

			return DelNode;
		}
		else return NULL;
	}

	void SetRight(STBTreeNode* RightNode)
	{
		if (m_pRight != NULL) delete m_pRight;

		m_pRight = RightNode;
	}
	STBTreeNode *GetRightSubTree()
	{
		return m_pRight;
	}
	void ChangeRightSubTree(STBTreeNode* RightNode)
	{
		m_pRight = RightNode;
	}
	STBTreeNode *RemoveRightSubTree() 
	{
		if (this != NULL)
		{
			STBTreeNode * DelNode = m_pRight;
			m_pRight = NULL;

			return DelNode;
		}
		else return NULL;
	
	
	}


	void DestroyChildren() 
	{
		if (m_pLeft != NULL) m_pLeft->DestroyChildren();
		if (m_pRight != NULL) m_pRight->DestroyChildren();
	
		if (m_pLeft != NULL) delete m_pLeft;
		if (m_pRight != NULL) delete m_pRight;
	}

	//Traverse
	void PreOrderTraverse(void(*DoSomeThing)(T data))
	{
		if (this == NULL)return;

		DoSomeThing(GetData());
		m_pLeft->PreOrderTraverse(DoSomeThing);
		m_pRight->PreOrderTraverse(DoSomeThing);
	}
	void InOrderTraverse(void(*DoSomeThing)(T Data))
	{
		if (this == NULL)return;
	
		m_pLeft->InOrderTraverse(DoSomeThing);
		DoSomeThing(GetData());
		m_pRight->InOrderTraverse(DoSomeThing);
	}
	void PostOrderTraverse(void(*DoSomeThing)(T Data))
	{
		if (this == NULL)return;

		m_pLeft->PostOrderTraverse(DoSomeThing);
		m_pRight->PostOrderTraverse(DoSomeThing);
		DoSomeThing(GetData());
	}
};

