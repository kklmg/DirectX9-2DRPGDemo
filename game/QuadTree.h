#pragma once
//-----------------------------------------------------------
//Include Head File
//-----------------------------------------------------------
#include<list>
#include<vector>


//-----------------------------------------------------------
//Include My Head File
//-----------------------------------------------------------
#include"ObjectClass.h"
#include"LivingObjectClass.h"
#include"EventMng.h"
#include"MyStruct.h"
#include"SceneLink.h"


 //---------------------------------------------------------------
 //Class QuadNode
 //----------------------------------------------------------------
class CQuadNode
{
public:
	CQuadNode();
	CQuadNode(RECT bound);
	~CQuadNode();
	

	//---------------------------------------------------------------
	//Some Enum Declare  
	//---------------------------------------------------------------
	enum QuadrantEnum
	{
		RT = 0,
		LT = 1,
		LB = 2,
		RB = 3
	};

	enum enCollidePlace
	{
		eDrawArea,
		eCoolideArea,
		eBeAttackArea,
		eRect2
	};


	//---------------------------------------------------------------
	//QuadNode Attribute
	//---------------------------------------------------------------
private:
	//Node Bound
	RECT m_rtBound;

	//SubNode 
	CQuadNode *m_pNode[4];

	//Parent Node
	CQuadNode *m_pPreNode;

	//Object Belong to Node
	std::list<CObject*>	m_List_pCollideObj;

	std::list<CLivingObj*>m_List_pAttackableObj;

	std::list<CSceneLink*>m_List_pLink;

	
	
	//---------------------------------------------------------------
	//QuadNode Function
	//---------------------------------------------------------------
public:
	WORD GetDepth();
	//assingn Vector
	void AssignVector(std::list<CObject*>&collideobj, std::list<CLivingObj*>&attackobj, std::list<CSceneLink*>&link);

	//Collision Detect
	void CheckCollision();

	void CheckCollisionEX();

	//check if node need refresh
	bool CheckRefresh();

	//Spilt Tree
	void Spilt(const DWORD MaxObj,const WORD MaxDepth);

	RECT GerBound()const ;

	void ClearSelf();
	//Clear ChildrenNode
	void ClearChildren();
	
	//Display Bound 
	void DisplayTreeBound(LPDIRECTDRAWSURFACE7 suf);

	//get an object list which collide with rect
	bool GetObjectInRect(RECT &rect, std::list<CLivingObj*>&ObjList);

	bool GetObjectInCircle(_CIRCLE &circle, std::list<CLivingObj*>&ObjList);

	void Traversal(void(*DoSomeThing)(CQuadNode*Node));



	//---------------------------------------------------------------
	//private collision function 
	//---------------------------------------------------------------
private:	

	bool GetSpiltedRect(std::vector<RECT> &GetVector, CQuadNode*node);

	void CheckCollideWithNode(CQuadNode*);

	void CheckCollideWithTree(CQuadNode*);

	void CheckCollideWithSupTree();

	void CheckCollision(CQuadNode*node);

	void CheckCollideAll();

	

	//---------------------------------------------------------------
	//Figure out which object collide with rect
	//---------------------------------------------------------------
	void GetRectNode(RECT &rect, CQuadNode **out);

	void GetRectCollideWithNode(RECT &rect, std::list<CLivingObj*>&ObjList);

	void GetRectCollideWithPreNode(RECT &rect, std::list<CLivingObj*>&ObjList);


	//---------------------------------------------------------------
	//Figure out which object collide with Circle
	//---------------------------------------------------------------
	void GetCircleNode(_CIRCLE circle, CQuadNode **out);

	void GetCircleCollideWithNode(_CIRCLE circle,std::list<CLivingObj*>&ObjList);

	void GetCircleCollideWithPreNode(_CIRCLE circle, std::list<CLivingObj*>&ObjList);


	//---------------------------------------------------------------
	//
	//---------------------------------------------------------------UnComplete!!!!!!!
};





//-------------------------------------------------------
//Class Quad Tree Manager 
//-------------------------------------------------------
class CQuadTree 
{
public:
	CQuadTree();
	~CQuadTree();

	//-------------------------------------------------------
	//Quad Tree Basic Function
	//-------------------------------------------------------
public:
	void InitTree(RECT rect, int MaxObj, int depth);

	void CheckCollision();

	void UpdateTree();

	void UpdateAtt();

	void clear(CQuadNode* node);

	void GetObjCollideWithRect(RECT &rect, std::list<CLivingObj*>&ObjList);

	void GetObjCollideWithCircle(_CIRCLE &circle, std::list<CLivingObj*>&ObjList);

	void DisPlayTreeBound(LPDIRECTDRAWSURFACE7 suf);

	void ReBuildTree();

	//-------------------------------------------------------
	//Quad Tree Advanced Function
	//-------------------------------------------------------
	bool CheckObjInDetectRange(CLivingObj *obj, std::list<CLivingObj*>&ObjList);

	bool PerformAttack(CLivingObj* attacker);



	//-------------------------------------------------------
	//Quad Tree Attribute
	//-------------------------------------------------------
private:
	

	//Root Node
	CQuadNode *m_pRoot;	
	//Screen Rect
	RECT m_rectScreen;	
	//Max Tree depth
	int m_nDepth;
	//Max object in Node
	int m_nMaxObject;

	//
	int m_nCollideObjCount;

	int m_nAttackableObjCount;

	enScene m_enCurScene;

};