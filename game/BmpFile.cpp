#include"Global_data.h"
#include"BmpFile.h"
#include"ddutil.h"

LPDIRECTDRAWSURFACE7 DDLoadBitmapFast(IDirectDraw7 * pdd, LPCSTR szBitmap, COLORREF key)
{
	LPDIRECTDRAWSURFACE7 temp = NULL;
	temp = DDLoadBitmap(pdd, szBitmap, 0, 0);
	DDSetColorKey(temp, key);
	return temp;
}

STDDSData* DDLoadBitmapFastEX(IDirectDraw7 * pdd, LPCSTR szBitmap, bool setkey, COLORREF key)
{
	HBITMAP                 hbm;
	BITMAP                  bm;
	DDSURFACEDESC2          ddsd;
	IDirectDrawSurface7    *pdds;

	STDDSData			   *pDDSData = NULL;

	//
	//  Try to load the bitmap as a resource, if that fails, try it as a file
	//
	hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, 0,
		0, LR_CREATEDIBSECTION);
	if (hbm == NULL)
		hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hbm == NULL)
		return NULL;
	//
	// Get size of the bitmap
	//
	GetObject(hbm, sizeof(bm), &bm);
	//
	// Create a DirectDrawSurface for this bitmap
	//
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	ddsd.dwWidth = bm.bmWidth;
	ddsd.dwHeight = bm.bmHeight;
	if (pdd->CreateSurface(&ddsd, &pdds, NULL) != DD_OK)
		return NULL;
	DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);
	DeleteObject(hbm);

	//set color key
	if (setkey)DDSetColorKey(pdds, key);

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = (long)ddsd.dwWidth;
	rect.bottom = (long)ddsd.dwHeight;

	pDDSData = new STDDSData(pdds, rect, setkey);


	return pDDSData;
}

CSheetFile::CSheetFile()
	:m_bUseKey(FALSE), m_pDDSData(NULL) {}

CSheetFile::~CSheetFile() 
{
	std::map<string, CRECTNode*>::iterator iter = m_mapRect.begin();

	while (iter != m_mapRect.end()) 
	{
		if ((*iter).second != NULL)
		{
			delete (*iter).second;
			(*iter).second = NULL;
		}
		m_mapRect.erase(iter++);
	}

	SAFE_RELEASE(m_pDDSData);
}

bool CSheetFile::LoadSheetFile(LPCSTR FileAddress, LPCSTR SheetName, bool SetKey, COLORREF colorkey)
{
	m_pDDSData = DDLoadBitmapFastEX(g_pDD, FileAddress, SetKey, colorkey);

	m_bUseKey = SetKey;

	if (m_pDDSData)return TRUE;
	else return FALSE;
}

void CSheetFile::AddSubRect(CRECTNode* rectnode, LPCSTR str)
{
	m_mapRect.insert(std::pair<string, CRECTNode*>(str, rectnode));
}


bool CSheetFile::CheckIfExists(LPCSTR str)
{
	std::map<string, CRECTNode*>::iterator iter = m_mapRect.find(str);
	if (iter == m_mapRect.end())return FALSE;
	else return TRUE;
}

CRECTNode* CSheetFile::GetRectNode(LPCSTR str)
{
	std::map<string, CRECTNode*>::iterator iter = m_mapRect.find(str);

	if (iter == m_mapRect.end())
		return NULL;
	else
	return (*iter).second;
}

HRESULT CSheetFile::Draw(LPDIRECTDRAWSURFACE7 DestSuf, RECT DestRect, RECT SrcRect, DWORD DrawSetting)
{
	HRESULT hr;
	if(m_bUseKey) hr=DestSuf->Blt(&DestRect, m_pDDSData->lpDDS, &SrcRect, DDBLT_KEYSRC | DDBLT_WAIT, NULL);

	else hr = DestSuf->Blt(&DestRect, m_pDDSData->lpDDS, &SrcRect, DDBLT_WAIT, NULL);

	return hr;
}

CSheetFile_E::CSheetFile_E()
:m_pDDSData(NULL) {}
CSheetFile_E::~CSheetFile_E() 
{
	SAFE_RELEASE(m_pDDSData);
}

bool CSheetFile_E::LoadSheetFile(LPCSTR FileAddress, LPCSTR SheetName, DWORD row, DWORD column, bool SetKey, COLORREF colorkey)
{
	m_pDDSData = DDLoadBitmapFastEX(g_pDD, FileAddress, SetKey, colorkey);

	m_wRow = row;

	m_wColumn = column;
	m_wCellWidth = m_pDDSData->RectEx.width / column;
	m_wCellHeight = m_pDDSData->RectEx.Height / row;
	m_wTileAmount = row * column;

	if (m_pDDSData)return TRUE;
	else return FALSE;
}

RECT CSheetFile_E::GetRect(WORD Row, WORD Column) 
{
	return __GetCoord({ Row,Column }, m_wCellWidth, m_wCellHeight);
}

DWORD CSheetFile_E::GetSheetCount() 
{
	return m_wTileAmount;
}
