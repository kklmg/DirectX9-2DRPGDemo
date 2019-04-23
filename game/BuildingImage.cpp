#include"BuildingImage.h"
#include"ddutil.h"
#include"Global_data.h"


CBuildingImg::CBuildingImg(LPCSTR bmpfile, COLORREF key):m_pRootNode(NULL)
{
	LoadBitmapFile(bmpfile,key);
}


CBuildingImg::~CBuildingImg()
{
	if (m_pSuf != NULL)
	{
		m_pSuf->Release();
		m_pSuf = NULL;
	}
}


bool CBuildingImg::LoadBitmapFile(LPCSTR bmpfile, COLORREF key)
{
	//create surface
	m_pSuf = DDLoadBitmap(g_pDD, bmpfile, 0, 0);

	//set color key
	DDSetColorKey(m_pSuf, key);

	//get and save original surface size
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	m_pSuf->GetSurfaceDesc(&ddsd);

	m_dwBmpHeight = ddsd.dwHeight;
	m_dwBmpWidth = ddsd.dwWidth;

	m_wCmTop = m_dwBmpHeight / 3;

	m_rtImg = { 0,0,(long)ddsd.dwWidth,(long)ddsd.dwHeight};
	

	return TRUE;
}

void CBuildingImg::AddLinkRect(RECT rect)
{
	m_vector_LinkRect.push_back(rect);
}

void CBuildingImg::AddCollideRect(RECT rect)
{
	m_vector_ColRect.push_back(rect);
}

void CBuildingImg::CreateCollideRectAuto(RECT removeRect) 
{
//	WORD cmHeight= m_dwBmpHeight / 3;

//	RECT rectLeft, rectTop, rectRight;

}

WORD CBuildingImg::GetCommonTop() 
{
	return m_wCmTop;
}

STRectData* CBuildingImg::GetTransformedColRect(RECT DesRect)
{
	m_pRootNode = new CRECTNode(m_rtImg, "temp");

	STRectData *pRectData = NULL;
	vector<RECT>::iterator iter;

	//Bound collide rect
	for (iter = m_vector_ColRect.begin(); iter != m_vector_ColRect.end(); iter++)
	{
		m_pRootNode->AddRect((*iter),"temp");
	}
	//calculate size
	m_pRootNode->TransformRect(DesRect);

	//get transformed rect
	pRectData = m_pRootNode->GetChildrenRect();


	delete m_pRootNode;
	m_pRootNode = NULL;

	return pRectData;
}

STRectData* CBuildingImg::GetTransformedSceRect(RECT DesRect)
{
	m_pRootNode = new CRECTNode(m_rtImg,"temp");

	STRectData *pRectData = NULL;
	vector<RECT>::iterator iter;

	//Bound scene rect
	for (iter = m_vector_LinkRect.begin(); iter != m_vector_LinkRect.end(); iter++)
	{
		m_pRootNode->AddRect((*iter), "temp");
	}
	//calculate size
	m_pRootNode->TransformRect(DesRect);

	//get transformed rect
	pRectData = m_pRootNode->GetChildrenRect();

	delete m_pRootNode;
	m_pRootNode = NULL;

	return pRectData;
}