//---------------------------------------------
//my Head File
//---------------------------------------------
#include"BasicBevNode.h"
#include"DxMng.h"


//---------------------------------------------
//Class Basic Behavior Node
//---------------------------------------------
CBevNode::CBevNode() 
{
	m_bHasInited = FALSE;
	m_bHasRun = FALSE;
}

CBevNode::~CBevNode() 
{
}

bool CBevNode::HasRunAlready() 
{
	return m_bHasRun;
}

bool CBevNode::HasInitedAlready() 
{
	return m_bHasInited;
}

//--------------------------------------------------
//Class Composite Node
//--------------------------------------------------
std::vector<CBevNode*> CCompositeNode::GetChildren() const
{
	return ChildrenVector;
}

void CCompositeNode::AddChild(CBevNode*child)
{
	ChildrenVector.emplace_back(child);
}

bool CCompositeNode::AreAllChildrenEnded()
{
	std::vector<CBevNode*>::iterator it;
	for (it = ChildrenVector.begin(); it != ChildrenVector.end(); it++)
	{	
		if (!(*it)->HasRunAlready()) return false;
	}	
	return true;
}

void CCompositeNode::ResetNodeState() 
{
	std::vector<CBevNode*>::iterator it;
	for (it = ChildrenVector.begin(); it != ChildrenVector.end(); it++)
	{		
		(*it)->ResetNodeState();
	}

	m_bHasInited = FALSE;
	m_bHasRun = FALSE;
}

void CCompositeNode::ClearInited() 
{
	std::vector<CBevNode*>::iterator it;
	for (it = ChildrenVector.begin(); it != ChildrenVector.end(); it++)
	{
		(*it)->ClearInited();
	}

	m_bHasInited = FALSE;
}

void CCompositeNode::Destroy() 
{
	std::vector<CBevNode*>::iterator it;
	for (it = ChildrenVector.begin(); it != ChildrenVector.end(); it++) 
	{
		(*it)->Destroy();	
	}
	delete this;

}




//--------------------------------------------------
//Composite Node::Squence Node
//--------------------------------------------------
CSequenceNode::CSequenceNode()
{
}

CSequenceNode::~CSequenceNode() 
{
}

enNodeState  CSequenceNode::run()
{
	std::vector<CBevNode*>::iterator it;
	for (it = ChildrenVector.begin(); it != ChildrenVector.end(); it++)
	{
		//skip completed node
		if ((*it)->HasRunAlready()) continue;

		//run and get node state
		enNodeState runResult = (*it)->run();

		// If one child fails, then enter operation run() fails.  Success only results if all children succeed.
		if (runResult == enNodeState::eRunnuing
			|| runResult == enNodeState::eNodeFalse) return runResult;


		//if (!result) return false; 

		//if ((*it)->GetNodeState() == enNodeState::eRunnuing)break;

	}
	// All children suceeded, so the entire run() operation succeeds.
	m_bHasRun = TRUE;

	ClearInited();

	return enNodeState::eNodeTrue;
		
}

//--------------------------------------------------
//Composite Node::Selector Node
//--------------------------------------------------
CSelectorNode::CSelectorNode() 
{
}

CSelectorNode::~CSelectorNode() 
{
}

enNodeState  CSelectorNode::run()
{
	std::vector<CBevNode*>::iterator it;
	for (it = ChildrenVector.begin(); it != ChildrenVector.end();it++)
	{
		//skip completed node
		if ((*it)->HasRunAlready()) continue;

		//run and get node state
		enNodeState runResult = (*it)->run();

		if (runResult == enNodeState::eRunnuing)
		{
			return runResult;
		}

		// If one child succeeds, the entire operation run() succeeds.  Failure only results if all children fail.
		if (runResult == enNodeState::eNodeTrue)
		{
			m_bHasRun = TRUE;
			ClearInited();
			return runResult;
		}

		//// If one child succeeds, the entire operation run() succeeds.  Failure only results if all children fail.
		//bool result = (*it)->run();

		//if ((*it)->GetNodeState() == enNodeState::eRunnuing) break;

		//if (result)
		//{
		//	SetNodeState(enNodeState::eEnd);
		//	return true;
		//}
		////if ((*it)->GetNodeState() == enNodeState::eRunnuing)break;


	}
	// All children failed so the entire run() operation fails.


	return enNodeState::eNodeFalse;

	
}


//--------------------------------------------------
//Composite Node::RandomSelector Node
//--------------------------------------------------
CRandomSelector::CRandomSelector()
{
	m_nRandFigure = -1;
}

CRandomSelector::~CRandomSelector()
{
}

enNodeState CRandomSelector::run()
{
	//Rand pick up
	if (m_bHasInited == FALSE)
	{
		m_nRandFigure = rand() % ChildrenVector.size();

		m_bHasInited = TRUE;
	}

	enNodeState runResult = ChildrenVector[m_nRandFigure]->run();

	m_bHasRun = ChildrenVector[m_nRandFigure]->HasRunAlready();

	return runResult;
}

//--------------------------------------------------
//Composite Node::Parallel Node
//--------------------------------------------------Uncomplete!!!!!
CParallelNode::CParallelNode() 
{
}

CParallelNode::~CParallelNode() 
{
}

enNodeState CParallelNode::run()
{
	return enNodeState::eNodeFalse;
}


//--------------------------------------------------
//Class Decorate Node
//--------------------------------------------------
CDecoratorNode::CDecoratorNode() 
{
	m_pChild = NULL;
}
CDecoratorNode::~CDecoratorNode()
{
	m_pChild = NULL;
}

void CDecoratorNode::setChild(CBevNode* newChild)
{
	m_pChild = newChild;
}

void CDecoratorNode::ResetNodeState() 
{
	m_pChild->ResetNodeState();

	m_bHasInited = FALSE;
	m_bHasRun = FALSE;
}

void CDecoratorNode::ClearInited()
{
	m_pChild->ClearInited();

	m_bHasInited = FALSE;
}

void CDecoratorNode::Destroy() 
{
	m_pChild->Destroy();

	delete this;
}


//--------------------------------------------------
//Decorate Node::Repeat Node
//--------------------------------------------------
CRepeatNode::CRepeatNode() 
{
}

CRepeatNode::~CRepeatNode() 
{
}

enNodeState CRepeatNode::run()
{
	if (m_pChild->HasRunAlready()) 
	{
		ResetNodeState();
	}
	//return child's result
	return(m_pChild->run());	
}


//--------------------------------------------------
//Decorate Node::Opposite Node
//--------------------------------------------------
COppositeNode::COppositeNode()
{
}

COppositeNode::~COppositeNode()
{

}

enNodeState COppositeNode::run()
{
	//return child's oposite result
	enNodeState result = m_pChild->run();

	if(result==enNodeState::eNodeTrue)
	{
		return enNodeState::eNodeFalse;
	}
	if(result == enNodeState::eNodeFalse)
	{
		m_bHasRun = TRUE;
		return enNodeState::eNodeTrue;
	}
}

