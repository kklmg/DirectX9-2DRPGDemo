#pragma once
#include<ddraw.h>
#include"DxMng.h"


//mirror blit
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void _PutSpriteHFlip(LPDIRECTDRAWSURFACE7 pDDS, int x, int y);

void _PutSpriteVFlip(LPDIRECTDRAWSURFACE7 pDDS, int x, int y);

void _PutSpriteHVFlip(LPDIRECTDRAWSURFACE7 pDDS, int x, int y);
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



//create surface
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

LPDIRECTDRAWSURFACE7 CreateSurfaceEX(DWORD dwWidth, DWORD dwHeight, BOOL bIsVram);
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



//palette
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
BOOL _DPLoad(LPCSTR name);

BOOL _DPSave(LPCSTR name);

void _PaletteRotation(int nBegin, int nEnd, DWORD dwDelay);
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//transfer bitmapfile to binfile
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void _Bmp2Bin(LPCSTR strBitmap, LPCSTR strBinfile);

void _GetBitmapInfo(LPCSTR szBitmap, LPBITMAPINFO lpBmpInfo);

BOOL _CopyDDS2MEM(LPDIRECTDRAWSURFACE7 lpSurface, LPBYTE lpMem, int nWidth, int nHeight);
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



//load bin file
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
LPDIRECTDRAWSURFACE7 _DDLoadBin(LPCSTR strFile);

BOOL _MEM2DDS(LPDIRECTDRAWSURFACE7 lpSurface, LPBYTE lpMem, int nWidth, int nHeight);
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



//normal draw 
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void _FillSurface(LPDIRECTDRAWSURFACE7 lpDDSDest, DWORD dwColor);

void _FillRect(LPDIRECTDRAWSURFACE7 lpDDSDest, int x1, int y1, int x2, int y2, DWORD dwColor);

void _FillRectEx(LPDIRECTDRAWSURFACE7 lpDDSDest, LPRECT lpRect, DWORD dwColor);

void _PutSprite(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y);
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

BOOL __CreateFastVBuffer(DWORD dwWidth, DWORD dwHeight, COLORREF rgb);

BOOL __CreateFastSBuffer(DWORD dwWidth, DWORD dwHeight, COLORREF rgb);

BOOL __CreateFastSBuffer2(DWORD dwWidth, DWORD dwHeight, COLORREF rgb);

void __LockMemcpy(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc);

void __BltCpy(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc);

bool CreateAndCopySurface(LPDIRECTDRAWSURFACE7 lpDDSDest, LPDIRECTDRAWSURFACE7 *lpDDSSrc, bool inVideoMemory, bool LockCopy);
