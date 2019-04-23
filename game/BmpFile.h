#pragma once
#include<string>
#include"RectMath.h"	
#include<map>



struct STDDSData
{
	STDDSData(LPDIRECTDRAWSURFACE7 lpDDsuf, RECT rect,bool colorkey=FALSE)
		:lpDDS(lpDDsuf), RectEx(rect), UseCKey(colorkey){}

	~STDDSData()
	{
		if (lpDDS != NULL) { lpDDS->Release(), lpDDS = NULL; }
	};

	HRESULT DrawNormal(LPRECT DestRect,LPDIRECTDRAWSURFACE7 DestSuf) 
	{
		if(UseCKey)
		return DestSuf->Blt(DestRect,lpDDS, NULL, DDBLT_WAIT|DDBLT_KEYSRC, NULL);
		else
		return DestSuf->Blt(DestRect, lpDDS, NULL, DDBLT_WAIT, NULL);

		
	}

	LPDIRECTDRAWSURFACE7 lpDDS = NULL;
	STRectEx RectEx;
	bool UseCKey;
};

LPDIRECTDRAWSURFACE7 DDLoadBitmapFast(IDirectDraw7 * pdd, LPCSTR szBitmap, COLORREF key);

STDDSData* DDLoadBitmapFastEX(IDirectDraw7 * pdd, LPCSTR szBitmap, bool setkey, COLORREF key);



class CSheetFile
{
public:
	CSheetFile();
	~CSheetFile();

	bool LoadSheetFile(LPCSTR Address, LPCSTR SheetName, bool SetKey, COLORREF colorkey);

	void AddSubRect(CRECTNode* rectnode, LPCSTR str);

	CRECTNode* GetRectNode(LPCSTR str);

	bool CheckIfExists(LPCSTR str);

	HRESULT Draw(LPDIRECTDRAWSURFACE7 DestSuf, RECT DestRect, RECT SrcRect, DWORD DrawSetting);

private:
	bool m_bUseKey;

	STDDSData *m_pDDSData;

	std::map<string, CRECTNode*>m_mapRect;
};

class CSheetFile_E
{
public:
	CSheetFile_E();
	~CSheetFile_E();

	bool LoadSheetFile(LPCSTR Address, LPCSTR SheetName,DWORD row,DWORD column ,bool SetKey, COLORREF colorkey);

	RECT GetRect(WORD Row, WORD Column);

	DWORD GetSheetCount();
private:

	bool m_bUseKey;

	STDDSData *m_pDDSData;

	WORD m_wTileAmount;

	WORD m_wColumn;
	WORD m_wRow;

	WORD m_wCellWidth;
	WORD m_wCellHeight;
};

