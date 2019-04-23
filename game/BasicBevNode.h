#pragma once
//---------------------------------------------
//Head File
//---------------------------------------------
#include <iostream>
#include<vector>

//---------------------------------------------
//my Head File
//---------------------------------------------


//---------------------------------------------
//Behavior Node Related enum 
//---------------------------------------------
//enum node state
enum enNodeState
{
	eNodeFalse,
	eNodeTrue,
	eRunnuing,
};

//---------------------------------------------
//Class Basic Behavior Node
//---------------------------------------------
class CBevNode
{
public:
	CBevNode();
	~CBevNode();

	virtual void ResetNodeState() = 0;	//reset Node State

	virtual void ClearInited() = 0;	//reInit signal

	virtual void Destroy() = 0;	//Destroy Self

	bool HasRunAlready();

	bool HasInitedAlready();

	virtual enNodeState run() = 0;//perform Behavior

protected:
	bool m_bHasRun;		//Mark this Node has runned already

	bool m_bHasInited;	//Mark this Node has Inited already
};

//--------------------------------------------------
//Class Composite Node
//--------------------------------------------------
class CCompositeNode : public CBevNode
{
public:
	std::vector<CBevNode*> GetChildren() const;//Get children Vector

	void AddChild(CBevNode*child);	//Add Node

	virtual void Destroy();

protected:
	bool AreAllChildrenEnded();

	virtual void ResetNodeState();

	virtual void ClearInited();

	std::vector<CBevNode*> ChildrenVector;
};

//--------------------------------------------------
//Composite Node::Squence Node
//--------------------------------------------------
class CSequenceNode : public CCompositeNode
{
public:
	CSequenceNode();
	~CSequenceNode();

	virtual enNodeState run();
};

//--------------------------------------------------
//Composite Node::Selector Node
//--------------------------------------------------
class CSelectorNode : public CCompositeNode
{
public:
	CSelectorNode();
	~CSelectorNode();

	virtual enNodeState run();
};


//--------------------------------------------------
//Composite Node::RandomSelector Node
//--------------------------------------------------
class CRandomSelector : public CCompositeNode
{  
public:
	CRandomSelector();
	~CRandomSelector();

	virtual enNodeState run();	
	
	int m_nRandFigure;
};


//--------------------------------------------------
//Composite Node::Parallel Node
//--------------------------------------------------Uncomplete!!!!!
class CParallelNode : public CCompositeNode
{
public:
	CParallelNode();
	~CParallelNode();

	bool *pbIsSucceedARR;

	virtual enNodeState run();
};

//--------------------------------------------------
//Class Decorate Node
//--------------------------------------------------
class CDecoratorNode : public CBevNode
{  
public:
	CDecoratorNode();
	~CDecoratorNode();

	void setChild(CBevNode* newChild);

	virtual void ResetNodeState();

	virtual void ClearInited();

	virtual void Destroy();

protected:
	CBevNode* m_pChild;  // Only one child allowed
};

//--------------------------------------------------
//Decorate Node::Repeat Node
//--------------------------------------------------
class CRepeatNode : public CDecoratorNode
{
public:
	CRepeatNode();
	~CRepeatNode();


	virtual enNodeState run();
};

//--------------------------------------------------
//Decorate Node::Opposite Node
//--------------------------------------------------
class COppositeNode : public CDecoratorNode
{
public:
	COppositeNode();
	~COppositeNode();


	virtual enNodeState run();
};

