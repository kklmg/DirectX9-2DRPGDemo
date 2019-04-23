#pragma once
#include<ddraw.h>
#include<Windows.h>
#include<vector>
#include"TimerClass.h"

class CTileSheet
{
public:
	CTileSheet();
	virtual~CTileSheet();

	void SetName(LPCSTR Name);
	std::string GetName()const;

	RECT GetSourceRect(WORD Row, WORD Column);
	
	DWORD GetTileCount() const;

	virtual DWORD GetSheetCount() = 0;

	bool HasAnimation()const;

	virtual bool LoadBitMapFile(LPCSTR szBitmap, WORD Row, WORD Column) = 0;

	virtual HRESULT Draw(LPDIRECTDRAWSURFACE7 DestlpDDS, RECT& DestRect, WORD row, WORD column,DWORD DrawSetting) = 0;

	virtual LPDIRECTDRAWSURFACE7 GetTileSurface(DWORD index) = 0;
protected:

	std::string m_TileSheetName;

	bool m_bHasInited;
	bool m_bIsMultiSheet;

	WORD m_wWidth;
	WORD m_wHeight;

	WORD m_wTileAmount;

	WORD m_wColumn;
	WORD m_wRow;

	WORD m_wCellWidth;
	WORD m_wCellHeight;
};

class CTileSheet_Single :public CTileSheet
{
public:
	CTileSheet_Single();
	virtual~CTileSheet_Single();

	virtual bool LoadBitMapFile(LPCSTR szBitmap, WORD Row, WORD Column);

	virtual DWORD GetSheetCount();

	virtual LPDIRECTDRAWSURFACE7 GetTileSurface(DWORD index);

	virtual HRESULT Draw(LPDIRECTDRAWSURFACE7 DestlpDDS, RECT& DestRect, WORD row, WORD column, DWORD DrawSetting);
private:
	LPDIRECTDRAWSURFACE7 m_pTileSuf;
};


class CTileSheet_Multi :public CTileSheet 
{
public:
	CTileSheet_Multi();
	virtual~CTileSheet_Multi();

	virtual bool LoadBitMapFile(LPCSTR szBitmap, WORD Row, WORD Column);

	virtual DWORD GetSheetCount();

	virtual LPDIRECTDRAWSURFACE7 GetTileSurface(DWORD index);

	virtual HRESULT Draw(LPDIRECTDRAWSURFACE7 DestlpDDS, RECT& DestRect, WORD row, WORD column, DWORD DrawID);
private:
	std::vector<LPDIRECTDRAWSURFACE7>m_vector_lpDDSTS;
};

