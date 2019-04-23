#pragma once
#include<list>
#include<stack>
#include<queue>

//-----------------------------------------------------------
//Include My Head File
//-----------------------------------------------------------
#include"collide.h"
#include"blit.h"
#include"damage.h"
#include"RectMath.h"
#include"ObjectMng.h"
#include"Text.h"
#include"QuadTree.h"

using namespace Collide;


////////////
//QuadNode//
////////////

//---------------------------------------------------------------
//Class QuadNode
//----------------------------------------------------------------
CQuadNode::CQuadNode()
	:m_pPreNode(NULL)
{
	m_pNode[0] = NULL;
	m_pNode[1] = NULL;
	m_pNode[2] = NULL;
	m_pNode[3] = NULL;
}

CQuadNode::CQuadNode(RECT bound)
	:m_pPreNode(NULL), m_rtBound(bound)
{
	m_pNode[0] = NULL;
	m_pNode[1] = NULL;
	m_pNode[2] = NULL;
	m_pNode[3] = NULL;
}

CQuadNode::~CQuadNode()
{
	ClearChildren();
}


//---------------------------------------------------------------
//QuadNode Public Main function
//----------------------------------------------------------------
WORD CQuadNode::GetDepth()
{
	CQuadNode* node = this;
	WORD depth = 0;
	
	while (node->m_pPreNode != NULL) 
	{
		++depth;
		node = node->m_pPreNode;	
	}
	return depth;
}

void CQuadNode::AssignVector(std::list<CObject*>&collideobj, std::list<CLivingObj*>&attackobj, std::list<CSceneLink*>&link)
{
	m_List_pCollideObj.assign(collideobj.begin(), collideobj.end());

	m_List_pAttackableObj.assign(attackobj.begin(), attackobj.end());

	m_List_pLink.assign(link.begin(), link.end());
}

bool CQuadNode::CheckRefresh()
{
	if (m_pPreNode)
	{
		//check obj
		std::list<CObject*>::iterator iter_obj;
		for (iter_obj = m_List_pCollideObj.begin(); iter_obj != m_List_pCollideObj.end(); iter_obj++)
		{
			//Check if obj still in pre rect
			if (!IsObjectCollideAreaInvolvedInRect((*iter_obj), m_rtBound)) return TRUE;
		}
		
		//check life obj be attack area
		std::list<CLivingObj*>::iterator iter_livobj;
		for (iter_livobj = m_List_pAttackableObj.begin(); iter_livobj != m_List_pAttackableObj.end(); iter_livobj++)
		{
			//Check if life obj atk bound still in pre rect
			if (!IsObjectBeAttackAreaInvolvedInRect((*iter_livobj), m_rtBound))return TRUE;
		}
	}

	if (m_pNode[RT])
	{
		if (m_pNode[RT]->CheckRefresh())return TRUE;
	}
	if (m_pNode[LT])
	{
		if (m_pNode[LT]->CheckRefresh())return TRUE;
	}
	if (m_pNode[LB])
	{
		if (m_pNode[LB]->CheckRefresh())return TRUE;
	}
	if (m_pNode[RB])
	{
		if (m_pNode[RB]->CheckRefresh())return TRUE;
	}
	return FALSE;
}


RECT CQuadNode::GerBound()const
{
	return m_rtBound;
}

bool CQuadNode::GetSpiltedRect(std::vector<RECT> &GetVector, CQuadNode*node)
{
	if (!node)return false;
	
	GetVector.resize(4);

	RECT bound = node->GerBound();

	//calculate each rect size
	int subWidth = (bound.right - bound.left) >> 1;
	int subHeight = (bound.bottom - bound.top) >> 1;

	//calculate spilted rect size
	RECT rectRT =
	{
		bound.left + subWidth,
		bound.top,
		bound.right,
		bound.bottom - subHeight
	};
	RECT rectLT =
	{
		bound.left,
		bound.top,
		bound.right - subWidth,
		bound.bottom - subHeight
	};
	RECT rectLB =
	{	bound.left,
		bound.top + subHeight,
		bound.right - subWidth,
		bound.bottom
	};
	RECT rectRB =
	{	bound.left + subWidth,
		bound.top + subHeight,
		bound.right,
		bound.bottom
	};


	GetVector[LT] = rectLT;
	GetVector[RT] = rectRT;
	GetVector[LB] = rectLB;
	GetVector[RB] = rectRB;
}


//Spilt self
void CQuadNode::Spilt(const DWORD MaxObj, const WORD MaxDEpth)
{
	queue<CQuadNode*>l_queue;
	CQuadNode* CurNode = nullptr;
	l_queue.push(this);

	//BFS
	while (!l_queue.empty())
	{
		CurNode = l_queue.front();
		l_queue.pop();

		if (!CurNode)continue;

		if (CurNode->m_pPreNode) 
		{
			RECT Bound = CurNode->GerBound();

			std::list<CObject*>::iterator iter_obj;
			std::list<CObject*>::iterator iter_obj_temp;		
			//----------------------------------------------------------
			//Distribute collideable Object
			//----------------------------------------------------------			
			for (iter_obj = CurNode->m_pPreNode->m_List_pCollideObj.begin(); iter_obj != CurNode->m_pPreNode->m_List_pCollideObj.end(); )
			{
				//Check object involved in RECT
				if (IsObjectCollideAreaInvolvedInRect((*iter_obj), Bound))
				{
					iter_obj_temp = iter_obj++;
					//splice object 
					CurNode->m_List_pCollideObj.splice(CurNode->m_List_pCollideObj.begin(), CurNode->m_pPreNode->m_List_pCollideObj, iter_obj_temp);
				}
				else iter_obj++;
			}


			std::list<CLivingObj*>::iterator iter_livobj;
			std::list<CLivingObj*>::iterator iter_livobj_temp;
			//----------------------------------------------------------
			//Distribute Attackable Object
			//----------------------------------------------------------
			for (iter_livobj = CurNode->m_pPreNode->m_List_pAttackableObj.begin(); iter_livobj != CurNode->m_pPreNode->m_List_pAttackableObj.end(); )
			{
				//check involved in rect
				if (IsObjectBeAttackAreaInvolvedInRect((*iter_livobj), Bound))
				{
					iter_livobj_temp = iter_livobj++;
					//splice object to rect LT list
					CurNode->m_List_pAttackableObj.splice(CurNode->m_List_pAttackableObj.begin(), CurNode->m_pPreNode->m_List_pAttackableObj, iter_livobj_temp);
				}
				else iter_livobj++;
			}


			std::list<CSceneLink*>::iterator iter_Link;
			std::list<CSceneLink*>::iterator iter_Link_temp;
			//----------------------------------------------------------
			//Distribute Link 
			//----------------------------------------------------------
			for (iter_Link = CurNode->m_pPreNode->m_List_pLink.begin(); iter_Link != CurNode->m_pPreNode->m_List_pLink.end();)
			{
				//check rect involved
				if ((*iter_Link)->IsInvolvedInrect(Bound))
				{
					iter_Link_temp = iter_Link++;
					//spliceLink to rect LT list
					CurNode->m_List_pLink.splice(CurNode->m_List_pLink.begin(), CurNode->m_pPreNode->m_List_pLink, iter_Link_temp);
				}
				else iter_Link++;
			}
		}
		//-------------------------------------------------------
		//Spilt	
		//-------------------------------------------------------
		if (CurNode->m_List_pCollideObj.size() < MaxObj)continue;
		if (CurNode->GetDepth() > MaxDEpth)continue;

		std::vector<RECT>SpiltedRect;
		GetSpiltedRect(SpiltedRect, CurNode);

		//create child rect
		
		CurNode->m_pNode[RT] = new CQuadNode(SpiltedRect[RT]); CurNode->m_pNode[RT]->m_pPreNode = CurNode;
		CurNode->m_pNode[LT] = new CQuadNode(SpiltedRect[LT]); CurNode->m_pNode[LT]->m_pPreNode = CurNode;
		CurNode->m_pNode[LB] = new CQuadNode(SpiltedRect[LB]); CurNode->m_pNode[LB]->m_pPreNode = CurNode;
		CurNode->m_pNode[RB] = new CQuadNode(SpiltedRect[RB]); CurNode->m_pNode[RB]->m_pPreNode = CurNode;

		l_queue.push(CurNode->m_pNode[RT]);
		l_queue.push(CurNode->m_pNode[LT]);
		l_queue.push(CurNode->m_pNode[LB]);
		l_queue.push(CurNode->m_pNode[RB]);
	}
}

void CQuadNode::ClearSelf() 
{
	ClearChildren();

	m_List_pCollideObj.clear();

	m_List_pAttackableObj.clear();

	m_List_pLink.clear();
}

void CQuadNode::ClearChildren()
{
	SAFE_RELEASE(m_pNode[RT]);
	SAFE_RELEASE(m_pNode[LT]);
	SAFE_RELEASE(m_pNode[LB]);
	SAFE_RELEASE(m_pNode[RB]);
}

//traversal by recursion
void CQuadNode::CheckCollision()
{
	CheckCollision(this);
}

//traversal by stack
void CQuadNode::CheckCollisionEX() 
{
	stack<CQuadNode*> stack_o;
	stack<CQuadNode*> stack_i;

	CQuadNode*CurNode;
	CQuadNode*CheckNode;

	stack_o.push(this);

	//Traversal Tree out
	//---------------------------
	while (!stack_o.empty())
	{
		CurNode = stack_o.top();
		stack_o.pop();

		CurNode->CheckCollideWithSupTree();
		//Traversal Tree In
		//-----------------------------------------------
		stack_i.push(CurNode);
		while (!stack_i.empty())
		{
			CheckNode = stack_i.top();
			stack_i.pop();

			//Check Cllision
			CurNode->CheckCollideWithNode(CheckNode);

			if (CheckNode->m_pNode[0])stack_i.push(CheckNode->m_pNode[0]);
			if (CheckNode->m_pNode[1])stack_i.push(CheckNode->m_pNode[1]);
			if (CheckNode->m_pNode[2])stack_i.push(CheckNode->m_pNode[2]);
			if (CheckNode->m_pNode[3])stack_i.push(CheckNode->m_pNode[3]);
		}
		//----------------------------------------------Travesal End
		if (CurNode->m_pNode[0])stack_o.push(CurNode->m_pNode[0]);
		if (CurNode->m_pNode[1])stack_o.push(CurNode->m_pNode[1]);
		if (CurNode->m_pNode[2])stack_o.push(CurNode->m_pNode[2]);
		if (CurNode->m_pNode[3])stack_o.push(CurNode->m_pNode[3]);
	}
}

bool CQuadNode::GetObjectInRect(RECT &rect,std::list<CLivingObj*>&ObjList)
{
	CQuadNode** tempNode = new CQuadNode*();
	*tempNode = this;

	GetRectNode(rect, tempNode);


	//check collide
	(*tempNode)->GetRectCollideWithNode(rect,ObjList);
	(*tempNode)->GetRectCollideWithPreNode(rect, ObjList);

	delete tempNode;

	return TRUE;
}

bool CQuadNode::GetObjectInCircle(_CIRCLE &circle, std::list<CLivingObj*>&ObjList)
{
	CQuadNode** tempNode = new CQuadNode*();
	*tempNode = this;

	GetCircleNode(circle, tempNode);

	//check collide
	(*tempNode)->GetCircleCollideWithNode(circle, ObjList);
	(*tempNode)->GetCircleCollideWithPreNode(circle, ObjList);

	SAFE_RELEASE(tempNode);

	return TRUE;
}

void CQuadNode::DisplayTreeBound(LPDIRECTDRAWSURFACE7 suf)
{
	if (this == NULL)return;

	suf->Blt(&m_rtBound, g_pDDSRect, NULL, DDBLT_KEYSRC | DDBLT_WAIT, NULL);

	m_pNode[0]->DisplayTreeBound(suf);
	m_pNode[1]->DisplayTreeBound(suf);
	m_pNode[2]->DisplayTreeBound(suf);
	m_pNode[3]->DisplayTreeBound(suf);
}

void CQuadNode::Traversal(void(*DoSomeThing)(CQuadNode*Node))
{
	stack<CQuadNode*>l_Stack;
	CQuadNode* CurNode = nullptr;

	l_Stack.push(this);
	while (!l_Stack.empty())
	{
		CurNode = l_Stack.top();
		DoSomeThing(CurNode);

		if (CurNode->m_pNode[0]) 
		{
			l_Stack.push(CurNode->m_pNode[0]);
			continue;		
		}
		if (CurNode->m_pNode[1])
		{
			l_Stack.push(CurNode->m_pNode[1]);
			continue;
		}
		if (CurNode->m_pNode[0])
		{
			l_Stack.push(CurNode->m_pNode[0]);
			continue;
		}
		if (CurNode->m_pNode[0])
		{
			l_Stack.push(CurNode->m_pNode[0]);
			continue;
		}
		else
		{
			l_Stack.pop();
		}
	}
}
//----------------------------------------------------------------
//QuadNode Private Collision function
//----------------------------------------------------------------
void CQuadNode::CheckCollideWithNode(CQuadNode* node) 
{
	if (node == NULL)return;

	//check collide
	std::list<CObject*>::iterator iter_Obj_A, iter_Obj_B;
	for (iter_Obj_A = m_List_pCollideObj.begin(); iter_Obj_A != m_List_pCollideObj.end(); iter_Obj_A++)
	{
		//if (!(*iter_Obj_A)->IsMoveAble())continue;

		//check collide with object
		for (iter_Obj_B = node->m_List_pCollideObj.begin(); iter_Obj_B != node->m_List_pCollideObj.end(); iter_Obj_B++)//object list
		{
			//skip check static object
			if ((*iter_Obj_A)->IsMoveAble()&& (*iter_Obj_A)->IsMoveAble())


			//skip check collide with myself
			if (&(*(*iter_Obj_A)) == &(*(*iter_Obj_B)))continue;

			//check and perform collision
			PerformTwoObjectsCollision((*iter_Obj_A), (*iter_Obj_B));
		}

		//if the object is User then check collide with link
		if (&(*(*iter_Obj_A)) == g_pControledObj)
		{
			std::list<CSceneLink*>::iterator iter_Link;
			for (iter_Link = node->m_List_pLink.begin(); iter_Link != node->m_List_pLink.end(); iter_Link++)
			{
				bool IsCollide = (*iter_Link)->IsObjTouchLink((*iter_Obj_A));

				if (IsCollide)
				{
					g_pTouchLink = (*iter_Link);
					break;
				}
			}
		}
	}
}

void CQuadNode::CheckCollideWithTree(CQuadNode* node) 
{
	if (!node)return;

	CheckCollideWithNode(node);

	CheckCollideWithTree(node->m_pNode[LT]);
	CheckCollideWithTree(node->m_pNode[RT]);
	CheckCollideWithTree(node->m_pNode[LB]);
	CheckCollideWithTree(node->m_pNode[RB]);
}

void CQuadNode::CheckCollideWithSupTree() 
{
	CQuadNode* p = m_pPreNode;
	while (p)
	{
		CheckCollideWithNode(p);
		p = p->m_pPreNode;
	}
}

//recursive helper
void CQuadNode::CheckCollision(CQuadNode*node)
{
	if (!node)return;

	node->CheckCollideAll();

	CheckCollision(node->m_pNode[0]);
	CheckCollision(node->m_pNode[1]);
	CheckCollision(node->m_pNode[2]);
	CheckCollision(node->m_pNode[3]);

	
}
//recursive helper
void CQuadNode::CheckCollideAll()
{
	CheckCollideWithTree(this);
	CheckCollideWithSupTree();
}


//---------------------------------------------------------------
//Figure out which object collide with rect
//---------------------------------------------------------------
void CQuadNode::GetRectNode(RECT &rect, CQuadNode **out)
{
	if (this == NULL)
		return;

	if (IsRectInvolvedinRect(m_rtBound, rect))
	{
		*out = this;

		m_pNode[RT]->GetRectNode(rect, out);
		m_pNode[LT]->GetRectNode(rect, out);
		m_pNode[LB]->GetRectNode(rect, out);
		m_pNode[RB]->GetRectNode(rect, out);
	}
	else return;
}

void CQuadNode::GetRectCollideWithPreNode(RECT &rect, std::list<CLivingObj*>&ObjList)
{
	if (!m_pPreNode)return;

	m_pPreNode->GetRectCollideWithPreNode(rect, ObjList);

	//check collide
	std::list<CLivingObj*>::iterator iter;
	for (iter = m_List_pAttackableObj.begin(); iter != m_List_pAttackableObj.end(); iter++)
	{
		//collide data
		bool IsCollide = IsObjectBeAttackAreaCollideWithRect((*iter), rect);

		//save collide object to list
		if (IsCollide)
		{
			ObjList.push_back((*iter));
		}
	}
}

void CQuadNode::GetRectCollideWithNode(RECT &rect, std::list<CLivingObj*>&ObjList)
{
	if (!this)return;

	//check collide
	std::list<CLivingObj*>::iterator iter;
	for (iter = m_List_pAttackableObj.begin(); iter != m_List_pAttackableObj.end(); iter++)
	{
		//collide data
		bool IsCollide = IsObjectBeAttackAreaCollideWithRect((*iter), rect);

		//save collide object to list
		if (IsCollide)
		{
			ObjList.push_back((*iter));
		}
	}
	m_pNode[0]->GetRectCollideWithNode(rect, ObjList);
	m_pNode[1]->GetRectCollideWithNode(rect, ObjList);
	m_pNode[2]->GetRectCollideWithNode(rect, ObjList);
	m_pNode[3]->GetRectCollideWithNode(rect, ObjList);
}


//---------------------------------------------------------------
//Figure out which object collide with circle
//---------------------------------------------------------------
void CQuadNode::GetCircleNode(_CIRCLE circle, CQuadNode **out)
{
	if (!this)return;
		

	if (IsCircleInvolvedinRect(circle, m_rtBound))
	{
		*out = this;

		m_pNode[RT]->GetCircleNode(circle, out);
		m_pNode[LT]->GetCircleNode(circle, out);
		m_pNode[LB]->GetCircleNode(circle, out);
		m_pNode[RB]->GetCircleNode(circle, out);
	}
	else return;

}

void CQuadNode::GetCircleCollideWithNode(_CIRCLE circle, std::list<CLivingObj*>&ObjList)
{
	if (!this)return;

	//check collide
	std::list<CLivingObj*>::iterator iter;
	for (iter = m_List_pAttackableObj.begin(); iter != m_List_pAttackableObj.end(); iter++)
	{
		//collide data
		bool IsCollide = IsObjectBeAttackAreaCollideWithCircle((*iter), circle);

		//save collide object to list
		if (IsCollide)
		{
			ObjList.push_back((*iter));
		}
	}
	m_pNode[0]->GetCircleCollideWithNode(circle, ObjList);
	m_pNode[1]->GetCircleCollideWithNode(circle, ObjList);
	m_pNode[2]->GetCircleCollideWithNode(circle, ObjList);
	m_pNode[3]->GetCircleCollideWithNode(circle, ObjList);
}

void CQuadNode::GetCircleCollideWithPreNode(_CIRCLE circle, std::list<CLivingObj*>&ObjList)
{
	if (!m_pPreNode)return;

	m_pPreNode->GetCircleCollideWithPreNode(circle, ObjList);

	//check collide
	std::list<CLivingObj*>::iterator iter;
	for (iter = m_List_pAttackableObj.begin(); iter != m_List_pAttackableObj.end(); iter++)
	{
		//collide data
		bool IsCollide = IsObjectBeAttackAreaCollideWithCircle((*iter), circle);

		//save collide object to list
		if (IsCollide)
		{
			ObjList.push_back((*iter));
		}
	}
}


/////////////
//QuadTree//
////////////

//-------------------------------------------------------
//Class Quad Tree Init
//-------------------------------------------------------
CQuadTree::CQuadTree() 
{
}

CQuadTree::~CQuadTree() 
{
	clear(m_pRoot);
}

void CQuadTree::InitTree(RECT rect, int MaxObj, int depth)
{
	m_rectScreen = rect;

	m_pRoot = new CQuadNode(rect);

	m_nMaxObject = MaxObj;

	m_nDepth = depth;

	ReBuildTree();
}

void CQuadTree::ReBuildTree() 
{
	//Attribute Update
	UpdateAtt();

	//clear pre root
	m_pRoot->ClearSelf();

	//Assign Vector
	m_pRoot->AssignVector(g_List_CollideObj, g_List_AttackableObj, g_list_maplink);

	//spilt Tree
	m_pRoot->Spilt(m_nMaxObject, m_nDepth);
}

//-------------------------------------------------------
//Quad Tree Basic Function
//-------------------------------------------------------
void CQuadTree::CheckCollision()
{
	m_pRoot->CheckCollisionEX();
}

void CQuadTree::UpdateTree()
{
	if (m_enCurScene != g_enCurScene) 
	{
		ReBuildTree();
		return;
	} 
	else if (m_nAttackableObjCount != g_List_AttackableObj.size()) 
	{
		ReBuildTree();
		return;
	}

	else if (m_nCollideObjCount != g_List_CollideObj.size()) 
	{
		ReBuildTree();
		return;
	}

	else if (m_pRoot->CheckRefresh()) 
	{
		ReBuildTree();
		return;
	}
	else return;
}

void CQuadTree::UpdateAtt() 
{
	m_enCurScene = g_enCurScene; 

	m_nAttackableObjCount = g_List_AttackableObj.size();

	m_nCollideObjCount = g_List_CollideObj.size();
}

void CQuadTree::clear(CQuadNode* node)
{
	SAFE_RELEASE(m_pRoot);
}

void CQuadTree::GetObjCollideWithRect(RECT &rect, std::list<CLivingObj*>&ObjList)
{
	m_pRoot->GetObjectInRect(rect, ObjList);
}

void CQuadTree::GetObjCollideWithCircle(_CIRCLE &circle, std::list<CLivingObj*>&ObjList)
{
	m_pRoot->GetObjectInCircle(circle, ObjList);
}

void CQuadTree::DisPlayTreeBound(LPDIRECTDRAWSURFACE7 suf)
{
	m_pRoot->DisplayTreeBound(suf);
}


//-------------------------------------------------------
//Quad Tree Advanced Function
//-------------------------------------------------------
bool CQuadTree::CheckObjInDetectRange(CLivingObj *obj, std::list<CLivingObj*>&ObjList)
{
	CQuadNode *node = NULL;

	std::list<CLivingObj*>::iterator iter;
	
	_CIRCLE cir(obj->GetPosition(), (float)obj->GetDetectDistance());

	GetObjCollideWithCircle(cir, ObjList);

	if (ObjList.size()>0) 
	{
		return TRUE;
	}
	else return FALSE;
}

bool CQuadTree::PerformAttack(CLivingObj* attacker)
{
	CQuadNode *node=NULL;

	std::list<CLivingObj*>ObjList;
	std::list<CLivingObj*>::iterator iter;

	
	GetObjCollideWithRect(attacker->GetAttackRect(),ObjList);

	for (iter = ObjList.begin(); iter != ObjList.end(); iter++) 
	{
		if (attacker == (*iter))continue;

		enObjectForce Force_Attacker = attacker->GetObjForce();
		enObjectForce Force_Target = (*iter)->GetObjForce();
	
		if (attacker->GetObjForce() == enObjectForce::eForFriend) 
		{
			if (Force_Target == enObjectForce::eForFriend)continue;
			if (Force_Target == enObjectForce::eForFNeutral)continue;
		}

		if (Force_Attacker == enObjectForce::eForHostile)
		{
			if (Force_Target == enObjectForce::eForHostile)continue;
		}

		CDamageData *damage=__CalDamage(attacker, (*iter));
		(*iter)->GetDamage(damage->m_nDamage);
	}
	return TRUE;
}