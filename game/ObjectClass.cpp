//------------------------------------------------------
//Include My Head File
//------------------------------------------------------
#include"ObjectClass.h"
#include"RectMath.h"
#include"blit.h"
#include"Global_data.h"


//------------------------------------------------------
//Object State Member
//------------------------------------------------------
DWORD CObject::s_dwObjCount=0;

DWORD CObject::GetObjectCount() 
{
	return s_dwObjCount;
}


//------------------------------------------------------
//Object Creator
//------------------------------------------------------
CObject::CObject()
	: 
	m_strName("UnKnown"),										//Name
	m_enObjType(enObjectType::eOTUnKnown),						//Type
	m_dwWidth(0), m_dwHeight(0), m_rectDraw({ 0,0,0,0 }),		//Draw Rect
	m_bIsAlive(TRUE),											//Alive
	m_enDir(enDirection::eUp),									//Direction
	m_enForce(enObjectForce::eForFNeutral),						//Force
	m_pointfPatrol({0,0}),										//Patrol Position
	m_pCollideArea(NULL),										//Collision
	m_bControlAble(FALSE),										//ControlAble
	m_bDrawFirst(FALSE),										//Always Draw First
	m_bMoveAble(TRUE),											//Object Moveable
	m_bLockDir(FALSE)											//Lock Direction
{	
	//obj count++
	++s_dwObjCount;

	//save in list
	g_list_pObj.push_back(this);
}

//------------------------------------------------------
//Object Destroyer
//------------------------------------------------------
CObject::~CObject()
{
	DestroyCollideArea();

	s_dwObjCount--;
}

void CObject::Init(int x, int y, DWORD Width, DWORD Height) 
{
	SetPosition(x, y);

	m_dwWidth = Width;
	m_dwHeight = Height;

	UpdateObject();

	m_pointfPatrol = CPosition::GetPosition();
}


//------------------------------------------------------
//Object Name
//------------------------------------------------------

//------------------------------------------------------
//Object Direction
//------------------------------------------------------
enDirection CObject::GetCurDir() const 
{
	return m_enDir;
}

bool CObject::SetObjectDir(enDirection dir) 
{
	if (m_bLockDir)return FALSE;
	else
	{
		m_enDir = dir;
		return TRUE;
	}
}

void CObject::LockDir() 
{
	m_bLockDir = TRUE;
}
void CObject::UnLockDir() 
{
	m_bLockDir = FALSE;
}

//------------------------------------------------------
//object Attribute
//------------------------------------------------------
enObjectType CObject::GetObjType() const
{
	return m_enObjType;
}

enObjectForce CObject::GetObjForce() const
{
	return m_enForce;
}

RECT CObject::GetDrawRect()const
{
	return m_rectDraw;
}

POINTFLOAT CObject::GetPatrolPoint()const
{
	return m_pointfPatrol;
}

bool CObject::IsMoveAble()const
{
	return m_bMoveAble;
}


//------------------------------------------------------
//object State
//------------------------------------------------------
bool CObject::IsAlive() const
{
	return m_bIsAlive;
}

bool CObject::IsObjNearBy(CObject*obj)const
{
	if (obj == NULL)return FALSE;
	RECT NearRect = GetScaledRect(m_rectDraw, 1.2, 1.2);

	return Collide::IsObjectCollideAreaCollideWithRect(obj, NearRect);
}

void CObject::MarkHasBeenCollide()
{
	m_stObjState.bHasBeenCollide = TRUE;
}

void CObject::MarkHasBeenAttack() 
{
	m_stObjState.bHasBeenAttack = TRUE;
}

STObjState CObject::GetObjState() const
{
	return m_stObjState;

}

//------------------------------------------------------
//object Name
//------------------------------------------------------
LPCSTR CObject::GetName() const
{
	return m_strName.data();
}

void CObject::SetName(string Name) 
{
	m_strName.clear();
	m_strName = Name;
}


//------------------------------------------------------
//Object Collision
//------------------------------------------------------
CCollideArea* CObject::GetCollideArea()const
{
	return m_pCollideArea;
}

void CObject::CreateCollideArea(bool Dynamic)
{
	m_pCollideArea = new CCollideArea(CPosition::GetVectorRef());
	m_pCollideArea->SetAreaDynamic(Dynamic);
	g_List_CollideObj.push_back(this);
}

void CObject::DestroyCollideArea()
{
	if (m_pCollideArea != NULL)
	{
		list<CObject*>::iterator iter;
		for (iter = g_List_CollideObj.begin(); iter != g_List_CollideObj.end(); iter++)
		{
			if ((*iter) == this)
			{
				(*iter) = NULL;
				g_List_CollideObj.erase(iter);
				break;
			}
		}
		SAFE_RELEASE(m_pCollideArea);
	}
}



//------------------------------------------------------
//Object Update
//------------------------------------------------------
void CObject::UpdateObject()
{
	//Update draw rect
	m_rectDraw = CreateRect({ (long)m_V2Pos.x,(long)m_V2Pos.y }, m_dwWidth, m_dwHeight);

	//Update Collide Area
	if(m_pCollideArea !=NULL)
		m_pCollideArea->UpdateArea();
}

void CObject::UpdatePatrolPoint() 
{
	m_pointfPatrol = CPosition::GetPosition();
}

void CObject::ResetState()
{
	m_stObjState.ResetState();
}







