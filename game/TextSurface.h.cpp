#include"blit.h"
#include"ddutil.h"
#include"Global_data.h"
#include"TextSurface.h"

CTextSuf::CTextSuf() :m_lpsTextSuf(NULL)
{
}

CTextSuf::CTextSuf(LPCSTR str, int maxCharInRow, int RowInterval, CTextSheet *textSheet, enColor eColor)
{
	CreateTextSuf(str, maxCharInRow, RowInterval, textSheet, eColor);
}

CTextSuf::~CTextSuf() 
{
	if (m_lpsTextSuf != NULL) 
	{
		m_lpsTextSuf->Release();
		m_lpsTextSuf = NULL;
	}
}

bool CTextSuf::UpdateString(LPCSTR str)
{
	if (m_str != str) 
	{
		m_str.replace(0,string::npos, str);	
		return TRUE;
	}
	return FALSE;
}

void CTextSuf::CreateTextSuf(LPCSTR str, int maxCharInRow, int RowInterval, CTextSheet *textSheet,enColor eColor)
{
	//assign string
	if (UpdateString(str))
	{
		m_lpsTextSuf = textSheet->CreateTextSuf(m_str.data(), maxCharInRow, RowInterval, eColor);

		//Get Surface width & height__________________
		DDSURFACEDESC2 ddsd;
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;

		m_lpsTextSuf->GetSurfaceDesc(&ddsd);

		//save width  and width
		m_dwHeight = ddsd.dwHeight;
		m_dwWidth = ddsd.dwWidth;
	}
}


void CTextSuf::CreateTextSuf_Single(char cr, CTextSheet *textSheet, enColor eColor)
{
	m_lpsTextSuf = textSheet->CreateTextSuf_Single(cr,eColor);
}

void CTextSuf::DrawTextSuf(RECT desrect, LPDIRECTDRAWSURFACE7 dessuf)
{
	if (m_lpsTextSuf == NULL)return;
	dessuf->Blt(&desrect, m_lpsTextSuf, NULL, DDBLT_KEYSRC | DDBLT_WAIT, NULL);
}

void CTextSuf::DrawTextSuf_Dynamic(LPCSTR str, int maxCharInRow, int RowInterval, 
	CTextSheet *textSheet, RECT desrect, LPDIRECTDRAWSURFACE7 dessuf,enColor color)
{
	SAFE_RELEASE_DDSURFACE(m_lpsTextSuf);

	CreateTextSuf(str, maxCharInRow, RowInterval, textSheet,color);

	dessuf->Blt(&desrect, m_lpsTextSuf, NULL, DDBLT_KEYSRC | DDBLT_WAIT, NULL);

}

void CTextSuf::DrawTextSuf(float x, float y, LPDIRECTDRAWSURFACE7 dessuf) 
{

	RECT DesRect= CreateRect({(long)x,(long)y}, m_dwWidth, m_dwHeight);

	dessuf->Blt(&DesRect, m_lpsTextSuf, NULL, DDBLT_KEYSRC | DDBLT_WAIT, NULL);
}


void CTextSuf::DrawTextSuf_Dynamic(LPCSTR str, float x, float y, int maxCharInRow, int RowInterval,
	CTextSheet *textSheet,LPDIRECTDRAWSURFACE7 dessuf, enColor color)
{
	RECT DesRect = CreateRect({ (long)x,(long)y }, m_dwWidth, m_dwHeight);


	SAFE_RELEASE_DDSURFACE(m_lpsTextSuf);

	CreateTextSuf(str, maxCharInRow, RowInterval, textSheet,color);

	dessuf->Blt(&DesRect, m_lpsTextSuf, NULL, DDBLT_KEYSRC | DDBLT_WAIT, NULL);

}

///////////////
//Text Sheet//
//////////////

CTextSheet::CTextSheet(LPCSTR FileName, DWORD row, DWORD column, COLORREF key)
{
	Init(FileName, row, column, key);
}

CTextSheet::~CTextSheet()
{
	if (m_prectFrame!=NULL)
	{	
		delete[]m_prectFrame;
		m_prectFrame = NULL;
	}

	if (m_lpsSuf != NULL) 
	{
	
		m_lpsSuf->Release();
		m_lpsSuf = NULL;
	}
	if (m_pVector != NULL)
	{
		vector<LPDIRECTDRAWSURFACE7>::iterator iter;

		for (iter = (*m_pVector).begin(); iter != (*m_pVector).end(); iter++)
		{
			if ((*iter) != NULL) 
			{
				(*iter)->Release();
				(*iter) = NULL;
			}	
		}
		(*m_pVector).clear();

		delete m_pVector;
		m_pVector = NULL;
	}

}

bool CTextSheet::Init(LPCSTR FileName, DWORD row, DWORD column, COLORREF key)
{
	LoadBitmapFile(FileName, row, column, key);

	CreateAllFrameRect();

	ColorizeSurface();

	return TRUE;
}

bool CTextSheet::ColorizeSurface()
{
	LPDIRECTDRAWSURFACE7 tempGray= __GrayScale(m_lpsSuf);

	m_pVector = __CreateColorizeSurface(m_lpsSuf);

	return TRUE;

}

bool CTextSheet::LoadBitmapFile(LPCSTR FileName, DWORD row, DWORD column, COLORREF key)
{
	//set surface
	m_lpsSuf = DDLoadBitmap(g_pDD, FileName, 0, 0);
	DDSetColorKey(m_lpsSuf, key);

	//Get Surface width & height__________________
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;

	m_lpsSuf->GetSurfaceDesc(&ddsd);

	//save width  and width
	m_dwHeight = ddsd.dwHeight;
	m_dwWidth = ddsd.dwWidth;

	m_dwFrameHeight = ddsd.dwHeight / row;
	m_dwFrameWidth = ddsd.dwWidth / column;

	m_dwColumn = column;
	m_dwRow = row;

	m_dwFrameCount = row*column;

	//save color key
	m_colorKey = key;


	if(m_lpsSuf) return TRUE;
	else return FALSE;
	
}

void CTextSheet::CreateAllFrameRect()
{
	
	m_prectFrame = new RECT[m_dwFrameCount];

	int Frame = 0;

	for (int row = 0; row < m_dwRow; row++)
	{
		for (int column = 0; column < m_dwColumn; column++)
		{
			m_prectFrame[Frame].left = column*m_dwFrameWidth;
			m_prectFrame[Frame].top = row*m_dwFrameHeight;
			m_prectFrame[Frame].right = (column + 1)*m_dwFrameWidth;
			m_prectFrame[Frame].bottom = (row + 1)*m_dwFrameHeight;

			Frame++;
		}
	}
}

RECT CTextSheet::GetTextFrame(char ch)
{
	int frame = int(ch) - 32;

	if (frame<0 || frame>m_dwFrameCount) 
	{
		return m_prectFrame[0];
	}

	return m_prectFrame[frame];
}

LPDIRECTDRAWSURFACE7 CTextSheet::CreateTextSuf(LPCSTR str, int maxCharInRow, int RowInterval,enColor eColor)
{
	int strLength = strlen(str);
	int maxRow = 1;
	int result_Column = 0;

	//figure out row and column count
	if (strLength <= maxCharInRow) 
	{
		result_Column = strLength;
	}
	else
	{
		result_Column = maxCharInRow;

		while (strLength > maxCharInRow)
		{
			maxRow++;
			strLength -= maxCharInRow;
		}
	}
	
	//create surface
	LPDIRECTDRAWSURFACE7 suf= CreateSurfaceEX(result_Column *m_dwFrameWidth, (m_dwFrameHeight + RowInterval)*maxRow, TRUE);
	
	_FillSurface(suf, m_colorKey);
	DDSetColorKey(suf, m_colorKey);

	int x = 0;
	//draw surface

	for (int row = 0; row < maxRow; row++)
	{
		for (int column = 0; column < maxCharInRow; column++)
		{
			if (x > strlen(str))break;

			//get source rect
			RECT sourceRect = GetTextFrame(str[x]);

			//figure out destination rect
			RECT desRect;

			desRect.left = column*m_dwFrameWidth;
			desRect.right = (column + 1)*m_dwFrameWidth;

			if (row == 0)
			{
				desRect.top = row*m_dwFrameHeight;
				desRect.bottom = (row + 1)*m_dwFrameHeight;
			}
			else
			{
				desRect.top = row*(m_dwFrameHeight + RowInterval);
				desRect.bottom = (row + 1)*(m_dwFrameHeight + RowInterval);;
			}

			//blit
			suf->Blt(&desRect,(*m_pVector)[eColor], &sourceRect, DDBLT_KEYSRC | DDBLT_WAIT, NULL);

			x++;
		}
	}
	return suf;

}

LPDIRECTDRAWSURFACE7 CTextSheet::CreateTextSuf_Single(char cr, enColor color)
{
	LPDIRECTDRAWSURFACE7 suf = CreateSurfaceEX(m_dwFrameWidth, m_dwFrameHeight, TRUE);

	_FillSurface(suf, m_colorKey);
	DDSetColorKey(suf, m_colorKey);

	RECT sourceRect = GetTextFrame(cr);

	suf->Blt(NULL, (*m_pVector)[color], &sourceRect, DDBLT_KEYSRC | DDBLT_WAIT, NULL);

	return suf;
}

bool CTextSheet::DrawTextSheet(LPDIRECTDRAWSURFACE7 dessuf, RECT desRect) 
{
	dessuf->Blt(&desRect, m_lpsSuf, NULL, DDBLT_KEYSRC | DDBLT_WAIT, NULL);
	return TRUE;
}
