#include"SheetAni.h"
#include"Global_data.h"
#include"RectMath.h"
#include"ddutil.h"

CAni_Sheet::CAni_Sheet():m_pDDS_AniSheet(NULL), m_pAni(NULL), m_dwDrawRow(0)
{
}

CAni_Sheet::CAni_Sheet(LPCSTR BitmapFile, int row,int column, float Vscale, float Hscale, COLORREF key)
{
	LoadAniSheet(BitmapFile, row, column, Vscale, Hscale, key);
}

CAni_Sheet::~CAni_Sheet()
{
	ClearALLSurface();

	if (m_pAni != NULL) 
	{
		delete[]m_pAni;
		m_pAni = NULL;	
	}

	if (m_pDDS_AniSheet != NULL)
	{
		m_pDDS_AniSheet->Release();
		m_pDDS_AniSheet = NULL;
	}
}

bool CAni_Sheet::LoadAniSheet(LPCSTR FileName, DWORD row, DWORD column, float Vscale, float Hscale, COLORREF key)
{
	//create surface from bitmap file
	m_pDDS_AniSheet = DDLoadBitmap(g_pDD, FileName, 0, 0);

	//set color key
	DDSetColorKey(m_pDDS_AniSheet, key);

	//surface desc
	DDSURFACEDESC2 ddsd;

	//get surface size 
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	m_pDDS_AniSheet->GetSurfaceDesc(&ddsd);

	//save surface size
	m_dwSheetWidth = ddsd.dwWidth;
	m_dwSheetHeight = ddsd.dwHeight;


	//figure out per frame size
	m_dwCellHeight = ddsd.dwHeight / row;
	m_dwCellWidth = ddsd.dwWidth / column;

	//save row column
	m_dwRow = row;
	m_dwColumn = column;
	
	//Init Animation


	m_pAni = new CAni_Sequence[row];

	for (int i = 0; i < row; i++) 
	{
		m_pAni[i].Init(m_pDDS_AniSheet, column, Vscale, Hscale,
			m_dwSheetWidth, m_dwCellHeight, m_dwCellWidth, m_dwCellHeight);

		for (int j=0; j < column; j++)
		{
			m_pAni[i].SetSourceRect(j, GetSourceRect(i, j));
		}
	}

	return TRUE;
}

RECT CAni_Sheet::GetSourceRect(DWORD row,DWORD column)
{
	RECT rect;

	rect.left = m_dwCellWidth * column;
	rect.right = m_dwCellWidth * (column + 1);
	rect.top = m_dwCellHeight * row;
	rect.bottom = m_dwCellHeight * (row + 1);
	
	return rect;
}

bool CAni_Sheet::DisplayAnimation(RECT DestRect, LPDIRECTDRAWSURFACE7 DestSuf, DWORD DrawRow, double Rate, bool repeat, bool ActiveScale)
{
	if (DrawRow > m_dwRow) return FALSE;

	m_pAni[DrawRow].DisplayAnimation(DestRect, DestSuf, Rate, repeat, ActiveScale);

	return TRUE;
}

bool CAni_Sheet::DisplayAnimation(POINT DestPoint, LPDIRECTDRAWSURFACE7 DestSuf, DWORD DrawRow, double Rate, bool repeat, bool ActiveScale)
{
	if (DrawRow > m_dwRow) return FALSE;

	m_pAni[DrawRow].DisplayAnimation(DestPoint, DestSuf, Rate, repeat, ActiveScale);

	return TRUE;
}

bool CAni_Sheet::DisplayOneFrame(RECT desRect,LPDIRECTDRAWSURFACE7 DestSuf ,DWORD DrawRow, DWORD DrawFrame, bool ActiveScale)
{
	if (DrawRow > m_dwRow) return FALSE;

	m_pAni[DrawRow].DisplayOneFrame(desRect, DestSuf, DrawFrame, ActiveScale);

	return TRUE;

}

bool CAni_Sheet::DisplayOneFrame(POINT DestPoint, LPDIRECTDRAWSURFACE7 DestSuf, DWORD DrawRow, DWORD DrawFrame, bool ActiveScale)
{
	if (DrawRow > m_dwRow) return FALSE;

	m_pAni[DrawRow].DisplayOneFrame(DestPoint, DestSuf, DrawFrame, ActiveScale);

	return TRUE;

}

void CAni_Sheet::ResetALLAnimation() 
{
	for (int i = 0; i < m_dwRow; i++) 
	{
		m_pAni[i].ResetAnimation();
	}
}

bool CAni_Sheet::IsAnimaitionEnd(DWORD row)
{
	return	m_pAni[row].IsAnimationEnd();
}

DWORD CAni_Sheet::GetCurFrame(DWORD row) 
{
	return	m_pAni[row].GetCurFrame();
}

void CAni_Sheet::ClearALLSurface() 
{
	//for (int i = 0; i < m_dwColumn; i++)
	//	{
	//	m_pAni[i].ClearSurfaceAdress();
	//}
}
