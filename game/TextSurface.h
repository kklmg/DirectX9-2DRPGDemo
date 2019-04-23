#pragma once
#include<ddraw.h>
#include"effect.h"
#include<string>
#include<vector>
#include"EnumGroup.h"

using namespace std;

class CTextSheet
{
public:

	CTextSheet(LPCSTR FileName, DWORD row, DWORD column, COLORREF key);
	~CTextSheet();

	bool Init(LPCSTR FileName, DWORD row, DWORD column, COLORREF key);

	bool ColorizeSurface();



	bool LoadBitmapFile(LPCSTR FileName, DWORD row, DWORD column, COLORREF key);

	RECT GetTextFrame(char ch);

	void CreateAllFrameRect();

	bool DrawTextSheet(LPDIRECTDRAWSURFACE7 dessuf, RECT desRect);

	bool DrawTextSuf(LPDIRECTDRAWSURFACE7 dessuf, RECT desRect, LPCSTR str);

	LPDIRECTDRAWSURFACE7 CreateTextSuf(LPCSTR str, int maxCharInRow, int RowInterval,enColor color);

	LPDIRECTDRAWSURFACE7 CreateTextSuf_Single(char cr,enColor color);

	const LPDIRECTDRAWSURFACE7 GetSurface();

private:
	LPDIRECTDRAWSURFACE7 m_lpsSuf = NULL;

	vector<LPDIRECTDRAWSURFACE7> *m_pVector = NULL;

	DWORD m_dwRow;

	DWORD m_dwColumn;

	DWORD m_dwHeight;

	DWORD m_dwWidth;

	DWORD m_dwFrameWidth;

	DWORD m_dwFrameHeight;

	DWORD m_dwFrameCount;

	RECT *m_prectFrame;

	COLORREF m_colorKey;

};





class CTextSuf
{
public:

	CTextSuf();
	CTextSuf(LPCSTR str, int maxCharInRow, int RowInterval, CTextSheet *textSheet, enColor eColor);

	~CTextSuf();

	bool UpdateString(LPCSTR str);//return TRUE if member string has changed

	void CreateTextSuf(LPCSTR str, int maxCharInRow, int RowInterval, CTextSheet *textSheet, enColor eColor);

	void CreateTextSuf_Single(char cr, CTextSheet *textSheet,enColor eColor);

	void DrawTextSuf(RECT desrect, LPDIRECTDRAWSURFACE7 dessuf);

	void DrawTextSuf_Dynamic(LPCSTR str, int maxCharInRow, int RowInterval,
		CTextSheet *textSheet, RECT desrect, LPDIRECTDRAWSURFACE7 dessuf,enColor color);

	void DrawTextSuf(float x, float y, LPDIRECTDRAWSURFACE7 dessuf);

	void DrawTextSuf_Dynamic(LPCSTR str, float x, float y, int maxCharInRow, int RowInterval,
		CTextSheet *textSheet, LPDIRECTDRAWSURFACE7 dessuf, enColor color);

private:
	LPDIRECTDRAWSURFACE7 m_lpsTextSuf;

	DWORD m_dwHeight;
	
	DWORD m_dwWidth;	

	string m_str;
};

