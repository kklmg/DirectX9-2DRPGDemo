#include"ddutil.h"
#include"Global_data.h"
#include"BuildingClass.h"
#include"RectMath.h"
#include"collide.h"
#include"blit.h"

//Class Door
CDoor::CDoor(RECT entry) :
	m_rtDoor(entry), m_rtLink(entry), m_rtDetect(entry), m_plink(NULL)
{
	WORD height = GetRectHeight(m_rtDoor);

	m_rtLink.bottom -= height*0.5;
	m_rtDetect.top = m_rtDoor.top;
	m_rtDetect.bottom += height*0.3;
}

bool CDoor::DrawLink(LPDIRECTDRAWSURFACE7 Dest)
{
	if (m_plink == NULL)return FALSE;
	else
	{
		_FillRectEx(Dest, &m_rtDoor, RGB(255, 255, 255));	
		return TRUE;
	}
}

void CDoor::CreateLink(enScene DestSce, RECT DestRect)
{
	m_plink = new CRectSLink(m_rtLink, DestSce, DestRect);
}
bool CDoor::IsObjNearby(CObject* obj) 
{
	return Collide::IsObjectCollideAreaCollideWithRect(obj, m_rtDetect);
}


//Class Building
CBuilding::CBuilding()
{
	m_enObjType = enObjectType::eOTBuilding;

	m_pCollideArea->SetCollideStrength(enPhysicalStrength::epyFixed);

	g_list_pBuilding.push_back(this);
}

CBuilding::~CBuilding()
{
}

CBuilding::CBuilding(int x, int y, DWORD width, DWORD height, CBuildingImg* Img)
{
	m_enObjType = enObjectType::eOTBuilding;

	m_pCollideArea->SetCollideStrength(enPhysicalStrength::epyFixed);

	InitBuilding(x, y, width, height);
	
	m_pImage = Img;
	
	SetDoor();

	UpdateCollideRect();

	g_list_pBuilding.push_back(this);
}

CBuilding::CBuilding(RECT pos, CBuildingImg* Img)
{
	m_enObjType = enObjectType::eOTBuilding;

	m_pCollideArea->SetCollideStrength(enPhysicalStrength::epyFixed);

	InitBuilding(pos);

	m_pImage = Img;

	SetDoor();

	UpdateCollideRect();

	g_list_pBuilding.push_back(this);
}

bool CBuilding::InitBuilding(int x, int y, DWORD Width, DWORD Height)
{
	CPosition::SetPosition(x, y);

	CObject::m_dwWidth = Width;
	CObject::m_dwHeight = Height;

	CObject::UpdateObject();

	return TRUE;
}

bool CBuilding::InitBuilding(RECT rect)
{
	CObject::m_rectDraw = rect;

	CObject::m_dwWidth = rect.right - rect.left;
	CObject::m_dwHeight = rect.bottom-rect.top;

	POINT pos= GetRectCenter(rect);	
	CPosition::SetPosition(pos.x, pos.y);

	return TRUE;
}

bool CBuilding::SetDoorLink(STBDLinkSetting LinkSetting)
{
	if (LinkSetting.index > m_vector_Door.size())return FALSE;

	m_vector_Door[LinkSetting.index].CreateLink(LinkSetting.enDesScene, LinkSetting.DestRect);

	return TRUE;
}

bool CBuilding::UpdateCollideRect()
{
	STRectData *rectData= m_pImage->GetTransformedColRect(m_rectDraw);

	if (rectData != NULL)
	{
		for (int i = 0; i < rectData->nRectCount; i++)
		{
			m_pCollideArea->AddCollideRect(rectData->pRect[i]);
		}
	}

	SAFE_RELEASE(rectData);

	return TRUE;
}

HRESULT CBuilding::DrawObject(LPDIRECTDRAWSURFACE7 suf, DWORD DrawSetting)
{
	
	suf->Blt(&(CObject::m_rectDraw), m_pImage->m_pSuf, NULL, DDBLT_KEYSRC | DDBLT_WAIT, NULL);


	for (int i = 0; i < 1; i++)
	{
		if (m_vector_Door[i].IsObjNearby(g_pControledObj))
		{		
			m_vector_Door[i].DrawLink(suf);
		}
	}	
	

	if (DrawSetting == 0)
	{
		for (int i = 0; i < 1; i++)
		{
			m_vector_Door[i].DrawLink(suf);
		}
	}

	return TRUE;
}

void CBuilding::UpdateAll()
{
	return;
}

bool CBuilding::SetDoor()
{
	STRectData *rectData = m_pImage->GetTransformedSceRect(m_rectDraw);

	if (rectData != NULL)
	{
		for (int i = 0; i < rectData->nRectCount; i++)
		{
			m_vector_Door.push_back(CDoor(rectData->pRect[i]));
		}
	}

	SAFE_RELEASE(rectData);

	return TRUE;
}
