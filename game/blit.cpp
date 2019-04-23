#pragma once
#include"blit.h"
#include"Global_data.h"
#include"ddutil.h"

///////////////
//mirror blit//
///////////////

void _PutSpriteHFlip(LPDIRECTDRAWSURFACE7 pDDS, int x, int y)
{
	ASSERT_VALID(pDDS != NULL);

	DDSURFACEDESC2 ddsd;
	DDBLTFX ddBltFx;

	ddBltFx.dwSize = sizeof(DDBLTFX);
	ddBltFx.dwDDFX = DDBLTFX_MIRRORLEFTRIGHT;

	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	pDDS->GetSurfaceDesc(&ddsd);

	RECT rcDest = { x, y, x + ddsd.dwWidth, y + ddsd.dwHeight };

	HRESULT ddrval = g_pDDSBack->Blt(&rcDest, pDDS, NULL, DDBLT_KEYSRC | DDBLT_DDFX | DDBLT_WAIT, &ddBltFx);
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}

void _PutSpriteVFlip(LPDIRECTDRAWSURFACE7 pDDS, int x, int y)
{
	ASSERT_VALID(pDDS != NULL);

	DDSURFACEDESC2 ddsd;
	DDBLTFX ddBltFx;

	ddBltFx.dwSize = sizeof(DDBLTFX);
	ddBltFx.dwDDFX = DDBLTFX_MIRRORUPDOWN;

	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	pDDS->GetSurfaceDesc(&ddsd);

	RECT rcDest = { x, y, x + ddsd.dwWidth, y + ddsd.dwHeight };

	HRESULT ddrval = g_pDDSBack->Blt(&rcDest, pDDS, NULL, DDBLT_KEYSRC | DDBLT_DDFX | DDBLT_WAIT, &ddBltFx);
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}

void _PutSpriteHVFlip(LPDIRECTDRAWSURFACE7 pDDS, int x, int y)
{
	ASSERT_VALID(pDDS != NULL);

	DDSURFACEDESC2 ddsd;
	DDBLTFX ddBltFx;

	ddBltFx.dwSize = sizeof(DDBLTFX);
	ddBltFx.dwDDFX = DDBLTFX_MIRRORLEFTRIGHT | DDBLTFX_MIRRORUPDOWN;

	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	pDDS->GetSurfaceDesc(&ddsd);

	RECT rcDest = { x, y, x + ddsd.dwWidth, y + ddsd.dwHeight };

	HRESULT ddrval = g_pDDSBack->Blt(&rcDest, pDDS, NULL, DDBLT_KEYSRC | DDBLT_DDFX | DDBLT_WAIT, &ddBltFx);
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

///////////////////
//create surface//
//////////////////

LPDIRECTDRAWSURFACE7 CreateSurfaceEX(DWORD dwWidth, DWORD dwHeight, BOOL bIsVram)
{
	HRESULT ddrval;
	DDSURFACEDESC2 ddsd;

	///// 오프스크린을 생성
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	if (bIsVram)
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	else
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

	ddsd.dwHeight = dwHeight;
	ddsd.dwWidth = dwWidth;

	/////
	LPDIRECTDRAWSURFACE7 lpDDS = NULL;
	ddrval = g_pDD->CreateSurface(&ddsd, &lpDDS, NULL);
	if (ddrval == DDERR_OUTOFVIDEOMEMORY)
	{
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		ddrval = g_pDD->CreateSurface(&ddsd, &lpDDS, NULL);
	}
	if (ddrval != DD_OK) { DDERRCHK(ddrval); return NULL; }

	return lpDDS;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

///////////
//palette//
///////////

BOOL _DPLoad(LPCSTR name)
{
	//// 파일포맷은 MsPalette, PaintShop에서 생성	
	HRESULT			hRet;

	FILE* file = fopen(name, "rb");
	if (!file) return ___Error("팔레트파일이 없습니다", __FILE__, __LINE__);

	//// 23바이트의 헤더 Skip
	fseek(file, 24, 0);

	//// 팔레트 본체 Read
	fread(g_Pal, sizeof(g_Pal), 1, file);

	////
	fclose(file);

	hRet = g_pDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, g_Pal, &g_pDDPal, NULL);
	if (hRet != DD_OK) return DDERRCHK(hRet);

	hRet = g_pDDSPrimary->SetPalette(g_pDDPal);
	if (hRet != DD_OK) return DDERRCHK(hRet);

	return TRUE;
}

BOOL _DPSave(LPCSTR name)
{
	//// 
	FILE* file = fopen(name, "wb");
	if (!file)
	{
		return ___Error("팔레트파일생성오류", __FILE__, __LINE__);
	}

	//// 팔레트 본체 Read
	fwrite(g_Pal, sizeof(g_Pal), 1, file);

	////
	fclose(file);

	return TRUE;
}

void _PaletteRotation(int nBegin, int nEnd, DWORD dwDelay)
{

	static DWORD dwOldTime = 0;
	static DWORD dwCurTime = GetTickCount();
	int i;
	PALETTEENTRY PalTemp;


	if (dwDelay + dwCurTime <= GetTickCount()) {
		dwCurTime = GetTickCount();
		PalTemp = g_Pal[nBegin];
		for (i = nBegin; i < nEnd; i++) {

			g_Pal[i] = g_Pal[i + 1];
		}
		g_Pal[nEnd - 1] = PalTemp;

	}

	
	g_pDDPal->SetEntries(0, 1, 7, g_Pal);
	
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//////////////////////////////////
//transfer bitmapfile to binfile//
//////////////////////////////////

void _Bmp2Bin(LPCSTR strBitmap, LPCSTR strBinfile)
{
	///// 비트맵 정보를 읽어온다
	BITMAPINFO       bmpinfo;

	_GetBitmapInfo(strBitmap, &bmpinfo);

	/////
	HBITMAP             hbm;
	BITMAP              bm;

	hbm = (HBITMAP)LoadImage(NULL, strBitmap, IMAGE_BITMAP,
		bmpinfo.bmiHeader.biWidth, bmpinfo.bmiHeader.biHeight,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hbm == NULL) return;

	GetObject(hbm, sizeof(bm), &bm);      // get size of bitmap

	LPDIRECTDRAWSURFACE7 lpDDS = CreateSurfaceEX(bm.bmWidth, bm.bmHeight, FALSE);

	DDCopyBitmap(lpDDS, hbm, 0, 0, 0, 0);

	DeleteObject(hbm);

	/////
	int nWidth = bm.bmWidth;
	switch (BPP)
	{
	case  8: nWidth = nWidth;      break; // 
	case 16: nWidth = nWidth << 1; break; // nWidth * 2
	case 24: nWidth = nWidth * 3; break; // nWidth * 3
	case 32: nWidth = nWidth << 2; break; // nWidth * 4
	}

	LPBYTE lpMA = (LPBYTE)malloc(nWidth * bm.bmHeight);
	BOOL bRval = _CopyDDS2MEM(lpDDS, lpMA, bm.bmWidth, bm.bmHeight);
	if (!bRval)
	{
		___Error("_CopyDDS2MEM", __FILE__, __LINE__);
		::DestroyWindow(g_hWnd);
		return;
	}

	lpDDS->Release();

	/////
	FILE* file = fopen(strBinfile, "wb");
	if (!file)
	{
		___Error("화일을 읽을수 없음", __FILE__, __LINE__);
		::DestroyWindow(g_hWnd);
		return;
	}
	SIZE szBIN;
	szBIN.cx = bm.bmWidth;
	szBIN.cy = bm.bmHeight;
	int nSize = nWidth * bm.bmHeight;
	::fwrite(&szBIN, sizeof(SIZE), 1, file);
	::fwrite(&nSize, sizeof(int), 1, file);
	::fwrite(lpMA, nWidth*bm.bmHeight, 1, file);
	fclose(file);
}

void _GetBitmapInfo(LPCSTR szBitmap, LPBITMAPINFO lpBmpInfo)
{
	FILE* file = fopen(szBitmap, "rb");
	if (!file)
	{
		___Error("GetBitmapInfo() - 화일을 읽을수 없음", __FILE__, __LINE__);
		::DestroyWindow(g_hWnd);
		return;
	}

	fseek(file, sizeof(BITMAPFILEHEADER), SEEK_SET);
	fread(lpBmpInfo, sizeof(BITMAPINFO), 1, file);

	fclose(file);
}

BOOL _CopyDDS2MEM(LPDIRECTDRAWSURFACE7 lpSurface, LPBYTE lpMem, int nWidth, int nHeight)
{

	int ix, iy;
	HRESULT hRet;
	DDSURFACEDESC2 ddsd;

	int nWidthStep;
	switch (BPP)
	{
	case  8: nWidthStep = nWidth;      break; // 
	case 16: nWidthStep = nWidth << 1; break; // dwWidth * 2
	case 24: nWidthStep = nWidth * 3; break; // dwWidth * 3
	case 32: nWidthStep = nWidth << 2; break; // dwWidth * 4
	}

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	hRet = lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);
	if (hRet != DD_OK) { DDERRCHK(hRet); return FALSE; }

	LPBYTE lpDDS = (LPBYTE)ddsd.lpSurface;

	for (iy = ix = 0; iy < nHeight; iy++)
	{
		memcpy(lpMem + ix, lpDDS, nWidthStep);
		lpDDS += ddsd.lPitch;
		ix += nWidthStep;
	}

	hRet = lpSurface->Unlock(NULL);
	if (hRet != DD_OK) { DDERRCHK(hRet); return FALSE; }

	return TRUE;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

////////////////
//load binfile//
////////////////

LPDIRECTDRAWSURFACE7 _DDLoadBin(LPCSTR strFile)
{
	SIZE szBin;
	int nSize;

	FILE* file = fopen(strFile, "rb");
	fread(&szBin, sizeof(SIZE), 1, file);
	fread(&nSize, sizeof(int), 1, file);
	LPBYTE lpImg = (LPBYTE)malloc(nSize);
	fread(lpImg, nSize, 1, file);
	fclose(file);

	/////
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	lpSurface = CreateSurfaceEX(szBin.cx, szBin.cy, FALSE);
	if (!lpSurface) { ___Error("_DDLoadBin", __FILE__, __LINE__); return NULL; }

	/////
	BOOL bRval = _MEM2DDS(lpSurface, lpImg, szBin.cx, szBin.cy);
	if (!bRval) { ___Error("DDLoadBin", __FILE__, __LINE__); return NULL; }

	free(lpImg);

	return lpSurface;
}

BOOL _MEM2DDS(LPDIRECTDRAWSURFACE7 lpSurface, LPBYTE lpMem, int nWidth, int nHeight)
{
	int ix, iy;
	DDSURFACEDESC2 ddsd;
	HRESULT hRet;

	int nWidthStep;
	switch (BPP)
	{
	case  8: nWidthStep = nWidth;      break; // 
	case 16: nWidthStep = nWidth << 1; break; // nWidth * 2
	case 24: nWidthStep = nWidth * 3; break; // nWidth * 3
	case 32: nWidthStep = nWidth << 2; break; // nWidth * 4
	}

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	while ((hRet = lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL)) == DDERR_WASSTILLDRAWING);
	if (hRet == DD_OK)
	{
		LPBYTE lpDDS = (LPBYTE)ddsd.lpSurface;

		for (iy = ix = 0; iy < nHeight; iy++)
		{
			memcpy(lpDDS, lpMem + ix, nWidthStep);
			lpDDS += ddsd.lPitch;
			ix += nWidthStep;
		}

		hRet = lpSurface->Unlock(NULL);
		if (hRet != DD_OK) return DDERRCHK(hRet);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

///////////////////////////////
//Fill surface with the color//
///////////////////////////////

void _FillSurface(LPDIRECTDRAWSURFACE7 lpDDSDest, DWORD dwColor)
{
	DDBLTFX ddbltfx;

	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(DDBLTFX);
	ddbltfx.dwFillColor = dwColor;

	HRESULT ddrval = lpDDSDest->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbltfx);
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}

void _FillRect(LPDIRECTDRAWSURFACE7 lpDDSDest, int x1, int y1, int x2, int y2, DWORD dwColor)
{
	DDBLTFX ddbltfx;
	RECT rcDest = { x1, y1, x2, y2 };

	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = dwColor;

	HRESULT ddrval = lpDDSDest->Blt(&rcDest, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbltfx);
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}

void _FillRectEx(LPDIRECTDRAWSURFACE7 lpDDSDest, LPRECT lpRect, DWORD dwColor)
{
	DDBLTFX ddbltfx;

	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = dwColor;

	/*HRESULT ddrval = */lpDDSDest->Blt(lpRect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbltfx);
	//if (ddrval != DD_OK) DDERRCHK(ddrval);
}

//////////////
//put sprite//
//////////////

void _PutSprite(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{
	///// 임시코드
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	lpDDS->GetSurfaceDesc(&ddsd);

	/////
	RECT rcDest = { x, y, x + ddsd.dwWidth, y + ddsd.dwHeight };
	HRESULT ddrval = g_pDDSBack->Blt(&rcDest, lpDDS, NULL, DDBLT_KEYSRC | DDBLT_WAIT, NULL);
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}


BOOL _PutImageScale(LPDIRECTDRAWSURFACE7 lpSurface, int x, int y, float fSx, float fSy)
{
	int ix, iy;
	DDSURFACEDESC2 ddsdDst, ddsdSrc;
	HRESULT hRet;

	if (BPP != 8) return FALSE;

	memset(&ddsdDst, 0, sizeof(ddsdDst));
	ddsdDst.dwSize = sizeof(ddsdDst);
	memset(&ddsdSrc, 0, sizeof(ddsdSrc));
	ddsdSrc.dwSize = sizeof(ddsdSrc);

	hRet = g_pDDSBack->Lock(NULL, &ddsdDst, DDLOCK_WAIT, NULL);
	if (hRet == DD_OK)
	{
		hRet = lpSurface->Lock(NULL, &ddsdSrc, DDLOCK_WAIT, NULL);
		if (hRet == DD_OK)
		{
			LPBYTE lpDDSBack = (LPBYTE)ddsdDst.lpSurface + ddsdDst.lPitch * y + x;
			LPBYTE lpDDS = (LPBYTE)ddsdSrc.lpSurface;

			//// 크기 조정
			int dx = ddsdSrc.dwWidth * fSx;
			int dy = ddsdSrc.dwHeight * fSy;

			int srcX, srcY;

			for (iy = 0; iy < dy; iy++)
			{
				for (ix = 0; ix < dx; ix++)
				{
					//// 소스 표면의 크기 만큼
					srcX = (int)(ix / fSx);
					srcY = (int)(iy / fSy);

					*(lpDDSBack + ix) = *(lpDDS + srcY * ddsdSrc.lPitch + srcX);
				}
				lpDDSBack += ddsdDst.lPitch;
			}

			hRet = lpSurface->Unlock(NULL);
			if (hRet != DD_OK) return DDERRCHK(hRet);
		}
		hRet = g_pDDSBack->Unlock(NULL);
		if (hRet != DD_OK) return DDERRCHK(hRet);
	}

	return TRUE;
}

BOOL __CreateFastVBuffer(DWORD dwWidth, DWORD dwHeight, COLORREF rgb)
{
	HRESULT ddrval;

	if (g_pDDSVBuffer)
	{
		ddrval = g_pDDSVBuffer->Release();
		if (ddrval != DD_OK) { DDERRCHK(ddrval); return FALSE; }
	}

	g_pDDSVBuffer = CreateSurfaceEX(dwWidth, dwHeight, TRUE);
	if (!g_pDDSVBuffer) return FALSE;

	if (BPP >= 16 && rgb != NULL)
	{
		ddrval = DDSetColorKey(g_pDDSVBuffer, rgb);
		if (ddrval != DD_OK) DDERRCHK(ddrval);
	}

	return TRUE;
}

BOOL __CreateFastSBuffer(DWORD dwWidth, DWORD dwHeight, COLORREF rgb)
{
	HRESULT ddrval;

	if (g_pDDSSBuffer)
	{
		ddrval = g_pDDSSBuffer->Release();
		if (ddrval != DD_OK) { DDERRCHK(ddrval); return FALSE; }
	}

	g_pDDSSBuffer = CreateSurfaceEX(dwWidth, dwHeight, FALSE);
	if (!g_pDDSSBuffer) return FALSE;

	if (BPP >= 16)
	{
		ddrval = DDSetColorKey(g_pDDSSBuffer, rgb);
		if (ddrval != DD_OK) DDERRCHK(ddrval);
	}

	return TRUE;
}

BOOL __CreateFastSBuffer2(DWORD dwWidth, DWORD dwHeight, COLORREF rgb)
{
	HRESULT ddrval;

	if (g_pDDSSBuffer2)
	{
		ddrval = g_pDDSSBuffer2->Release();
		if (ddrval != DD_OK) { DDERRCHK(ddrval); return FALSE; }
	}

	g_pDDSSBuffer2 = CreateSurfaceEX(dwWidth, dwHeight, FALSE);
	if (!g_pDDSSBuffer2) return FALSE;

	if (BPP >= 16)
	{
		ddrval = DDSetColorKey(g_pDDSSBuffer2, rgb);
		if (ddrval != DD_OK) DDERRCHK(ddrval);
	}

	return TRUE;
}

void __LockMemcpy(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc)
{
	DDSURFACEDESC2 ddsdSrc, ddsdDst;

	//// 소스표면의 RECT
	ddsdSrc.dwSize = sizeof(ddsdSrc);
	lpDDSSrc->GetSurfaceDesc(&ddsdSrc);
	RECT rcSrc = { 0, 0, ddsdSrc.dwWidth, 1 };
	DWORD dwWidth = ddsdSrc.dwWidth;

	//// 소스표면 Lock
	ZeroMemory(&ddsdSrc, sizeof(ddsdSrc));
	ddsdSrc.dwSize = sizeof(ddsdSrc);
	HRESULT ddrval = lpDDSSrc->Lock(&rcSrc, &ddsdSrc, DDLOCK_READONLY | DDLOCK_WAIT, NULL);
	if (ddrval != DD_OK) { DDERRCHK(ddrval); return; }

	//// 목적표면의 RECT
	ddsdDst.dwSize = sizeof(ddsdDst);
	lpDDSDst->GetSurfaceDesc(&ddsdDst);
	RECT rcDst = { 0, 0, ddsdDst.dwWidth, 1 };

	//// 목적표면 Lock
	ZeroMemory(&ddsdDst, sizeof(ddsdDst));
	ddsdDst.dwSize = sizeof(ddsdDst);
	ddrval = lpDDSDst->Lock(&rcDst, &ddsdDst, DDLOCK_WRITEONLY | DDLOCK_WAIT, NULL);
	if (ddrval != DD_OK)
	{
		//// 소스표면은 Lock 상태이므로 Unlock
		lpDDSSrc->Unlock(&rcSrc);
		DDERRCHK(ddrval);
		return;
	}

	//// 표면의 주소
	LPBYTE lpSrc = (LPBYTE)ddsdSrc.lpSurface;
	LPBYTE lpDst = (LPBYTE)ddsdDst.lpSurface;

	//TRACE2("Src Addr - %x\n Dst Addr - %x\n", lpSrc, lpDst);
	::memcpy(lpDst, lpSrc, dwWidth);



	lpDDSDst->Unlock(&rcDst);
	lpDDSSrc->Unlock(&rcSrc);

	//Sleep(1);
}

void __BltCpy(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc)
{
	/////	
	HRESULT ddrval = lpDDSDst->Blt(NULL, lpDDSSrc, NULL, DDBLT_WAIT, NULL);
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}

LPDIRECTDRAWSURFACE7 **test;

bool CreateAndCopySurface(LPDIRECTDRAWSURFACE7 lpDDSDest, LPDIRECTDRAWSURFACE7 *lpDDSSrc, bool inVideoMemory,bool LockCopy)
{
	HRESULT ddrval;
	DDSURFACEDESC2 ddsd;
	
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;

	lpDDSDest->GetSurfaceDesc(&ddsd);

	*lpDDSSrc = CreateSurfaceEX(ddsd.dwWidth, ddsd.dwHeight, inVideoMemory);


	if (!LockCopy) 
	{
		__BltCpy(*lpDDSSrc,lpDDSDest);
	}
	else
	{
		__LockMemcpy(*lpDDSSrc, lpDDSDest);
	}
	
	return TRUE;
}


