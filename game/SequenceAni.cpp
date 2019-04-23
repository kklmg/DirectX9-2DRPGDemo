#include"SequenceAni.h"
#include"ddutil.h"
#include"Global_data.h"
#include"RectMath.h"

CAni_Sequence::CAni_Sequence() : m_pDDSAnimation(NULL)
{
	m_dwCurFrame = 0;

	m_dwHeight = 0;

	m_dwWidth = 0;

	m_dwCellWidth = 0;

	m_dwCellHeight = 0;

	m_pRect = NULL;

	m_bCopyCreate = FALSE;
}

CAni_Sequence::~CAni_Sequence()
{
	if (m_pRect != NULL)
	{
		delete[] m_pRect;
		m_pRect = NULL;
	}


	if (m_bCopyCreate) 
	{
		if (m_pDDSAnimation != NULL)
		{
			m_pDDSAnimation = NULL;
		}
	}
	else 
	{
		if (m_pDDSAnimation != NULL)
		{
			m_pDDSAnimation->Release();
			m_pDDSAnimation = NULL;
		}
	}
}

CAni_Sequence::CAni_Sequence(STAnimationDesc desc) : m_bCopyCreate(TRUE), m_pDDSAnimation(NULL)
{
	CAnimation::SetMaxFrame(desc.MaxFrame);

	CAnimation::SetScale(desc.Hscale, desc.Vscale);

	//Load Sprite to Surface
	m_pDDSAnimation = desc.lpDDS;

	//Declare dds desc
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));

	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;

	//Get Surface Size
	m_pDDSAnimation->GetSurfaceDesc(&ddsd);

	m_dwWidth = ddsd.dwWidth;

	m_dwHeight = ddsd.dwHeight;

	m_dwCellWidth = ddsd.dwWidth / desc.MaxFrame;

	m_dwCellHeight = ddsd.dwHeight;

	//
	InitSourceRect();

	for (int i = 0; i <desc.MaxFrame; i++)
	{
		SetSourceRect(i, GetSourceRect(i));
	}
}

CAni_Sequence::CAni_Sequence(LPCSTR BitmapFile, int maxFrame, float Vscale, float Hscale, COLORREF key):m_bCopyCreate(FALSE), m_pDDSAnimation(NULL)
{
	LoadBitmapFile(BitmapFile, maxFrame, Vscale, Hscale, key);
}

void CAni_Sequence::Init(LPDIRECTDRAWSURFACE7 lpdds, int maxFrame, float Vscale, float Hscale,
	DWORD Width, DWORD Height, DWORD CellWidth, DWORD CellHeight)
{
	m_pDDSAnimation = lpdds;
	m_bCopyCreate = TRUE;

	CAnimation::SetMaxFrame(maxFrame);

	CAnimation::SetScale(Hscale, Vscale);

	m_dwWidth = Width;

	m_dwHeight = Height;

	m_dwCellWidth = CellWidth;

	m_dwCellHeight = CellHeight;

	InitSourceRect();

}

bool CAni_Sequence::LoadBitmapFile(LPCSTR FileName, int maxFrame, float Vscale, float Hscale, COLORREF key) 
{
	CAnimation::SetMaxFrame(maxFrame);

	CAnimation::SetScale(Hscale,Vscale);

	//Load Sprite to Surface
	m_pDDSAnimation = DDLoadBitmap(g_pDD, FileName, 0, 0);

	//Set Color Key
	DDSetColorKey(m_pDDSAnimation, key);

	//Declare dds desc
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));

	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;

	//Get Surface Size
	m_pDDSAnimation->GetSurfaceDesc(&ddsd);

	m_dwWidth = ddsd.dwWidth;

	m_dwHeight = ddsd.dwHeight;

	m_dwCellWidth = ddsd.dwWidth / maxFrame;

	m_dwCellHeight = ddsd.dwHeight;

	//
	InitSourceRect();

	for (int i = 0; i < maxFrame; i++) 
	{
		SetSourceRect(i, GetSourceRect(i));	
	}

	m_bCopyCreate = FALSE;

	return TRUE;
}

bool CAni_Sequence::DisplayAnimation(RECT DestRect, LPDIRECTDRAWSURFACE7 DDSDest,double Rate, bool repeat, bool ActiveScale)
{
	if (!m_pDDSAnimation) return FALSE;

	//Init Timer And Animation State
	CAnimation::InitAnimation();


	if (ActiveScale)
	{
		//Get Scaled Rect
		RECT ScaledRect = CAnimation::GetScaleRect(DestRect);

		//blt
		DDSDest->Blt(&ScaledRect, m_pDDSAnimation, &m_pRect[m_dwCurFrame], DDBLT_KEYSRC | DDBLT_WAIT, NULL);
	}
	else 	
	{
		//blt
		DDSDest->Blt(&DestRect, m_pDDSAnimation, &m_pRect[m_dwCurFrame], DDBLT_KEYSRC | DDBLT_WAIT, NULL);
	}


	//Frame Update
	CAnimation::UpdateFrame(Rate, repeat);

	return TRUE;
}

bool CAni_Sequence::RepeatAnimation(RECT desRect, LPDIRECTDRAWSURFACE7 DDSDest, double Rate, DWORD repeatcount, bool ActiveScale) 
{
	if (!m_pDDSAnimation) return FALSE;

	//Init Timer And Animation State
	CAnimation::InitAnimation();


	if (ActiveScale)
	{
		//Get Scaled Rect
		RECT ScaledRect = CAnimation::GetScaleRect(desRect);

		//blt
		DDSDest->Blt(&ScaledRect, m_pDDSAnimation, &m_pRect[m_dwCurFrame], DDBLT_KEYSRC | DDBLT_WAIT, NULL);
	}
	else
	{
		//blt
		DDSDest->Blt(&desRect, m_pDDSAnimation, &m_pRect[m_dwCurFrame], DDBLT_KEYSRC | DDBLT_WAIT, NULL);
	}


	//Frame Update
	CAnimation::UpdateFrame(Rate, repeatcount);

	return TRUE;

}

bool CAni_Sequence::DisplayAnimation(POINT point, LPDIRECTDRAWSURFACE7 DDSDest, double Rate, bool repeat, bool ActiveScale)
{
	if (!m_pDDSAnimation) return FALSE;

	//Init Timer And Animation State
	CAnimation::InitAnimation();

	//figure out destinate rect
	DWORD width = m_pRect[m_dwCurFrame].right - m_pRect[m_dwCurFrame].left;
	DWORD height = m_pRect[m_dwCurFrame].bottom - m_pRect[m_dwCurFrame].top;

	RECT destRect;
	if(ActiveScale) destRect = CreateRect(point, width*m_fHScale, height*m_fVScale);
	else  destRect = CreateRect(point, width, height);

	//blt
	DDSDest->Blt(&destRect, m_pDDSAnimation, &m_pRect[m_dwCurFrame], DDBLT_KEYSRC | DDBLT_WAIT, NULL);

	//Frame Update
	CAnimation::UpdateFrame(Rate, repeat);

	return TRUE;
}

bool CAni_Sequence::DisplayOneFrame(POINT point, LPDIRECTDRAWSURFACE7 DDSDest,DWORD showFrame, bool ActiveScale)
{
	if (!m_pDDSAnimation) return FALSE;

	//figure out destinate rect
	DWORD width = m_pRect[showFrame].right - m_pRect[showFrame].left;
	DWORD height = m_pRect[showFrame].bottom - m_pRect[showFrame].top;

	RECT destRect;
	if (ActiveScale) destRect = CreateRect(point, width*m_fHScale, height*m_fVScale);
	else  destRect = CreateRect(point, width, height);

	//blit______________________________________________________________
	DDSDest->Blt(&destRect, m_pDDSAnimation, &m_pRect[showFrame], DDBLT_KEYSRC | DDBLT_WAIT, NULL);

	return TRUE;
}

bool CAni_Sequence::DisplayOneFrame(RECT desRect, LPDIRECTDRAWSURFACE7 DDSDest,DWORD showFrame, bool ActiveScale)
{
	if (!m_pDDSAnimation) return FALSE;

	if (ActiveScale)
	{
		//Get Scaled Rect
		RECT ScaledRect = CAnimation::GetScaleRect(desRect);

		//blt
		DDSDest->Blt(&ScaledRect, m_pDDSAnimation, &m_pRect[showFrame], DDBLT_KEYSRC | DDBLT_WAIT, NULL);
	}
	else
	{
		//blt
		DDSDest->Blt(&desRect, m_pDDSAnimation, &m_pRect[showFrame], DDBLT_KEYSRC | DDBLT_WAIT, NULL);
	}

	return TRUE;
}

const LPDIRECTDRAWSURFACE7 CAni_Sequence::GetSurface()
{
	return m_pDDSAnimation;
}

void CAni_Sequence::ClearSurfaceAdress() 
{
	if(m_pDDSAnimation!=NULL)
	m_pDDSAnimation = NULL;
}

bool CAni_Sequence::InitSourceRect()
{
	if (m_dwMaxFrame == 0)
		return FALSE;

	m_pRect = new RECT[m_dwMaxFrame];

	return TRUE;
}

RECT CAni_Sequence::GetSourceRect(DWORD frame) 
{
	RECT rect;

	rect.left = m_dwCellWidth * frame;
	rect.right = m_dwCellWidth * (frame + 1);
	rect.top = 0 ;
	rect.bottom = m_dwCellHeight;

	return rect;
}

bool CAni_Sequence::SetSourceRect(DWORD index, RECT rect)
{
	if (m_pRect == NULL)return FALSE;
	if (index > m_dwMaxFrame)return FALSE;

	m_pRect[index] = rect;

	return TRUE;

}

