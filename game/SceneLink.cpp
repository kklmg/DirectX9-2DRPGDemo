#include"SceneLink.h"
#include"RectMath.h"
#include"blit.h"
#include"SceneMng.h"
#include"collide.h"
#include"Global_data.h"


//Class Scene Link Base
//const/destr
CSceneLink::CSceneLink(enShape LinkType, enScene Cur, enScene Dest,enDirection EntDir)
	:m_enDestScene(Dest),m_enCurScene(Cur),m_enEntDir(EntDir)
	, m_bHasOppLink(TRUE), m_bIsActive(TRUE),m_enLinkType(LinkType)
{
	g_list_maplink.push_back(this);
}

CSceneLink::~CSceneLink()
{
	//m_pObj = NULL;
}

//Scene Setting
void CSceneLink::SetCurScene(enScene eSce)
{
	m_enCurScene = eSce;
}
void CSceneLink::SetDestScene(enScene scene)
{
	m_enDestScene = scene;
}
enScene CSceneLink::GetCurScene() const
{
	return m_enCurScene;
}
enScene CSceneLink::GetDestScene() const
{
	return m_enDestScene;
}
//Active Link
void CSceneLink::ActiveLink()
{
	m_bIsActive = TRUE;
}
void CSceneLink::DisActiveLink()
{
	m_bIsActive = FALSE;
}
bool CSceneLink::IsActive() const
{
	return m_bIsActive;
}

enShape CSceneLink::GetLinkType()const
{
	return m_enLinkType;
}













CRectSLink::CRectSLink(RECT Entrance, enScene DestScene, RECT Dest, enDirection EntDir)
	: CSceneLink(eRect,g_enCurScene, DestScene,EntDir)
{
	InitLink(Entrance, Dest);
}
//CRectSLink::CRectSLink(STSceneDesc desc)
//	: CSceneLink(g_enCurScene, desc.Scene)
//{
//	InitLink(desc.Entrance, desc.Dest);
//}
//CRectSLink::CRectSLink(const CRectSLink & c)
//	: CSceneLink(c.m_enCurScene, c.m_enDestScene)
//{
//	InitLink(c.m_rtEntrance, c.m_rtDest);
//}
CRectSLink::~CRectSLink() 
{}

void CRectSLink::operator=(const CRectSLink & c)
{
	m_enCurScene = c.m_enCurScene;
	m_enDestScene = c.m_enDestScene;
	InitLink(c.m_rtEntrance,c.m_rtDest);
}

void CRectSLink::InitLink(RECT Entrance, RECT DestRect)
{
	m_rtEntrance = Entrance;
	m_rtDest = DestRect;
}

void CRectSLink::SetTriggerRect(RECT rect)
{
	m_rtEntrance = rect;
}
void CRectSLink::SetDestRect(RECT Dset)
{
	m_rtDest = Dset;
}

bool CRectSLink::IsObjTouchLink(CObject *obj)
{
	if (obj == NULL)return FALSE;

	return Collide::IsObjectCollideAreaCollideWithRect(obj,m_rtEntrance);
}



CSceneLink* CRectSLink::CreateOPPLinkAuto(bool Active)
{
	if (m_bHasOppLink)
	{
		CRectSLink  *newLink = new CRectSLink(m_rtDest, m_enCurScene,m_rtEntrance,__GetReverseDir(m_enEntDir));
		if(Active)	newLink->ActiveLink();
		else newLink->DisActiveLink();
		newLink->m_bHasOppLink = FALSE;

		newLink->SetCurScene(m_enDestScene);
		return newLink;
	}
	else return NULL;
	
}

POINT CRectSLink::GetDestPos(CObject *obj)
{
	RECT objpos=obj->GetDrawRect();

	RECT offset = m_rtDest;
	WORD width = GetRectWidth(m_rtDest)+50;
	WORD Height = GetRectHeight(m_rtDest)+50;

	switch (m_enEntDir)
	{
		case eDown: 
		{
			offset.top += Height;
			offset.bottom += Height;	
		}break;

		case eLeft:
		{
			offset.left -= width;
			offset.right -= width;
		}break;

		case eRight:
		{
			offset.left += width;
			offset.right += width;
		}break;

		case eUp:
		default: 
		{
			offset.top -= Height;
			offset.bottom -= Height;
	
		}break;
	}


	return GetRectCenter(offset);
}

bool CRectSLink::DisplayLinkInfo(LPDIRECTDRAWSURFACE7 suf)
{
	_FillRectEx(suf, &m_rtEntrance, RGB(255, 255, 255));
	return TRUE;
}

bool CRectSLink::IsInvolvedInrect(RECT &rect)
{
	return Collide::IsRectInvolvedinRect(rect, m_rtEntrance);
}

UNLinkShape CRectSLink::GetDest()
{
	return UNLinkShape(m_rtDest);
}

bool CRectSLink::IsTheSameAsLink(CSceneLink* Link) 
{
	if (m_enLinkType == Link->GetLinkType())
	{
		if (m_enDestScene == Link->GetDestScene())
		{
			if (m_enCurScene == Link->GetCurScene())
			{
				UNLinkShape entr = Link->GetDest();

				if (AreTheSameRect(m_rtDest,entr.rect))return TRUE;		
			}
		}
	}
	return FALSE;
}



CLineSLink::CLineSLink()
{
}

CLineSLink::~CLineSLink()
{
}
