#pragma once
#include"StillObject.h"
#include"SceneLink.h"
#include"BuildingImage.h"
#include<vector>

#define Building_MyHome g_pDataBase->GetBuildingImg(enBuilding::eMyhome)

struct STBDLinkSetting
{
	STBDLinkSetting(DWORD id, enScene desSce, RECT Dest) 
	{
		index = id;
		enDesScene = desSce;
		DestRect = Dest;	
	}

	DWORD index;
	enScene enDesScene;
	RECT DestRect;
};

class CDoor
{
public:
	CDoor(RECT entry);

	bool DrawLink(LPDIRECTDRAWSURFACE7 Dest);

	void CreateLink(enScene DestSce, RECT DestRect);

	bool IsObjNearby(CObject* obj);
private:
	RECT m_rtDoor;
	RECT m_rtLink;
	RECT m_rtDetect;
	CSceneLink* m_plink;
};


class CBuilding : public CStillObject
{
public:
	CBuilding();
	CBuilding(int x, int y, DWORD width, DWORD height, CBuildingImg* Img);
	CBuilding(RECT pos, CBuildingImg* Img);

	virtual ~CBuilding();

	CBuildingImg* m_pImage;

	CSceneLink *m_plink;
	
	std::vector<CDoor>m_vector_Door;

	bool SetDoor();

	bool UpdateCollideRect();	
	virtual HRESULT CObject::DrawObject(LPDIRECTDRAWSURFACE7 suf, DWORD DrawSetting);
	virtual void UpdateAll();

	
	bool InitBuilding(int x,int y, DWORD Width, DWORD Height);
	bool InitBuilding(RECT rect);

	bool SetDoorLink(STBDLinkSetting LinkSetting);
};

