#include"TileSheet.h"
#include"Global_data.h"
#include"ddutil.h"
#include"RectMath.h"

//Tile Sheet format
//-----------------------------------------------
CTileSheet::CTileSheet()
	:m_wWidth(0), m_wHeight(0), m_wTileAmount(0),
m_wColumn(0), m_wRow(0), m_wCellWidth(0), m_wCellHeight(0), m_bHasInited(FALSE)
{
};

CTileSheet::~CTileSheet() 
{
}

RECT CTileSheet::GetSourceRect(WORD Row, WORD Column)
{
	if (Row > m_wRow || Column > m_wColumn) 
	return __GetCoord({ 0,0 }, m_wCellWidth, m_wCellHeight);
	
	else	
	return __GetCoord({ Row,Column }, m_wCellWidth, m_wCellHeight);
}
DWORD CTileSheet::GetTileCount() const
{
	return m_wTileAmount;
}

void CTileSheet::SetName(LPCSTR Name) 
{
	m_TileSheetName = Name;
}
std::string  CTileSheet::GetName() const
{
	return m_TileSheetName;
}
bool CTileSheet::HasAnimation()const
{
	return m_bIsMultiSheet;
}


//Tile Sheet normal
//-----------------------------------------------
CTileSheet_Single::CTileSheet_Single() :m_pTileSuf(NULL)
{
	m_bIsMultiSheet = FALSE;
}

CTileSheet_Single::~CTileSheet_Single() 
{
	if (m_pTileSuf != NULL)
	{
		m_pTileSuf->Release();
		m_pTileSuf = NULL;
	}
}

bool CTileSheet_Single::LoadBitMapFile(LPCSTR szBitmap, WORD Row, WORD Column) 
{
	DDSURFACEDESC2 ddsd;
	m_pTileSuf = DDLoadBitmap(g_pDD, szBitmap, 0, 0);//get bitmap file
	ZeroMemory(&ddsd, sizeof(ddsd));
	//DDSetColorKey(m_pTileSuf, key);//set color key
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;

	m_pTileSuf->GetSurfaceDesc(&ddsd);

	CTileSheet::m_wHeight = ddsd.dwHeight;
	CTileSheet::m_wWidth = ddsd.dwWidth;
	CTileSheet::m_wRow = Row;

	CTileSheet::m_wColumn = Column;
	CTileSheet::m_wCellWidth = m_wWidth / Column;
	CTileSheet::m_wCellHeight = m_wHeight / Row;
	CTileSheet::m_wTileAmount = Row*Column;

	CTileSheet::m_bHasInited = TRUE;
	if(m_pTileSuf)return TRUE;
	else return FALSE;
}

DWORD CTileSheet_Single::GetSheetCount()
{
	if(m_pTileSuf)
		return  1;
		else return 0;
}


LPDIRECTDRAWSURFACE7 CTileSheet_Single::GetTileSurface(DWORD index)
{
	return m_pTileSuf;	
}

HRESULT CTileSheet_Single::Draw(LPDIRECTDRAWSURFACE7 DestlpDDS, RECT& DestRect, WORD row, WORD column, DWORD DrawSetting)
{
	RECT SrcRect = GetSourceRect(row, column);
	return DestlpDDS->Blt(&DestRect, m_pTileSuf, &SrcRect, DDBLT_WAIT, NULL);
}

//Tile Sheet ani
//-----------------------------------------------
CTileSheet_Multi::CTileSheet_Multi()
{
	m_bIsMultiSheet = TRUE;
}

CTileSheet_Multi::~CTileSheet_Multi()
{
	for (int i = 0; i < m_vector_lpDDSTS.size(); ++i) 
	{
		if (m_vector_lpDDSTS[i] != NULL)
		{
			m_vector_lpDDSTS[i]->Release();
			m_vector_lpDDSTS[i] = NULL;
		}
	}
	m_vector_lpDDSTS.clear();
}

bool CTileSheet_Multi::LoadBitMapFile(LPCSTR szBitmap, WORD Row, WORD Column)
{
	DDSURFACEDESC2 ddsd;

	LPDIRECTDRAWSURFACE7 lpDDS= DDLoadBitmap(g_pDD, szBitmap, 0, 0);//get bitmap file

	m_vector_lpDDSTS.push_back(lpDDS);

	if (!m_bHasInited)
	{
		ZeroMemory(&ddsd, sizeof(ddsd));
		//DDSetColorKey(m_pTileSuf, key);//set color key
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;

		lpDDS->GetSurfaceDesc(&ddsd);

		CTileSheet::m_wHeight = ddsd.dwHeight;
		CTileSheet::m_wWidth = ddsd.dwWidth;
		CTileSheet::m_wRow = Row;

		CTileSheet::m_wColumn = Column;
		CTileSheet::m_wCellWidth = m_wWidth / Column;
		CTileSheet::m_wCellHeight = m_wHeight / Row;
		CTileSheet::m_wTileAmount = Row*Column;

		CTileSheet::m_bHasInited = TRUE;
	}
	return TRUE;
}

DWORD CTileSheet_Multi::GetSheetCount() 
{
	return  m_vector_lpDDSTS.size();
}

LPDIRECTDRAWSURFACE7 CTileSheet_Multi::GetTileSurface(DWORD index)
{
	if (index >= m_vector_lpDDSTS.size())return NULL;
	return m_vector_lpDDSTS[index];
}

HRESULT CTileSheet_Multi::Draw(LPDIRECTDRAWSURFACE7 DestlpDDS, RECT& DestRect, WORD row, WORD column, DWORD drawID)
{
	RECT SrcRect = GetSourceRect(row, column);

	if (drawID > m_vector_lpDDSTS.size())return FALSE;
	return DestlpDDS->Blt(&DestRect, m_vector_lpDDSTS[drawID], &SrcRect, DDBLT_WAIT, NULL);
}


