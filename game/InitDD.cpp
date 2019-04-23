#include"initDD.h"
#include<time.h>
#include"Global_data.h"
#include"effect.h"

BOOL DDInit() 
{

	HRESULT hRet;
	DDSURFACEDESC2              ddsd;		// 钎搁俊 措茄 加己 汲沥
	DDSCAPS2                    ddscaps;	// 钎搁 瓷仿 汲沥
	
	
	hRet = DirectDrawCreateEx(NULL, (VOID**)&g_pDD, IID_IDirectDraw7, NULL);
	if (hRet != DD_OK) return DDERRCHK(hRet);

	hRet = g_pDD->SetCooperativeLevel(g_hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
	if (hRet != DD_OK) return DDERRCHK(hRet);

//	g_pDD->EnumDisplayModes()


	hRet = g_pDD->SetDisplayMode(HRES, VRES, BPP,
		0,
		0);

	if (hRet != DD_OK) return DDERRCHK(hRet);

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP |
		DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount = nBackbuffcount;
	hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, NULL);
	if (hRet != DD_OK) return DDERRCHK(hRet);


	ZeroMemory(&ddscaps, sizeof(ddscaps));
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	hRet = g_pDDSPrimary->GetAttachedSurface(&ddscaps, &g_pDDSBack);
	if (hRet != DD_OK) return DDERRCHK(hRet);

	return TRUE;
}

BOOL DDInitWin() {

	DDSURFACEDESC2 ddsd;
	HRESULT        ddrval;

	HDC hdc = GetDC(g_hWnd);
	int nBitPixel = GetDeviceCaps(hdc, BITSPIXEL);
	ReleaseDC(g_hWnd,hdc);

	if (nBitPixel != nBitPixel)return FALSE;
	LPDIRECTDRAW lpDD;
	ddrval = DirectDrawCreate(NULL, &lpDD, NULL);
	if (ddrval != DD_OK) return DDERRCHK(ddrval);
	ddrval = lpDD->QueryInterface(IID_IDirectDraw7, (LPVOID*)&g_pDD);
	if (ddrval != DD_OK) return DDERRCHK(ddrval);
	lpDD->Release();

	///// 
	ddrval = g_pDD->SetCooperativeLevel(g_hWnd, DDSCL_NORMAL);
	if (ddrval != DD_OK) return DDERRCHK(ddrval);

	///// 
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	ddrval = g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, NULL);
	if (ddrval != DD_OK) return DDERRCHK(ddrval);

	/////// 
	bool bIsVram = TRUE;			 //video memory or system memory
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	if (bIsVram)
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	else
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

		ddsd.dwHeight = VRES;
		ddsd.dwWidth =  HRES;
		
		ddrval = g_pDD->CreateSurface(&ddsd, &g_pDDSBack, NULL);
		if (ddrval == DDERR_OUTOFVIDEOMEMORY)
		{
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
			ddrval = g_pDD->CreateSurface(&ddsd, &g_pDDSBack, NULL);
		}
		if (ddrval != DD_OK) { DDERRCHK(ddrval); return NULL; }

		return TRUE;
}

BOOL DXW_Init()
{
	///// DX 按眉 积己 棺 檬扁拳
	BOOL bRval;

	///// 
#ifdef _DEBUG
	bRval = DDInitWin();
	if (!bRval)
	{
		bRval = DDInit();
		if (!bRval) return FALSE;
	}
#else
	bRval = DDInit();
	if (!bRval) return FALSE;
#endif
	
	__DDCSetWindowClipper(g_pDDSPrimary);
	__DDCSetClip(g_pDDSBack, 0, 0, HRES, VRES);
	
	g_enPixelFormat = __CheckRGBBit(g_pDDSPrimary);
	
	return TRUE;
}

BOOL DDSWBlt()
{
	RECT  rcDest;
	POINT pt = { 0, 0 };

	/////	
	::GetClientRect(g_hWnd, &rcDest);
	::ClientToScreen(g_hWnd, &pt);
	::OffsetRect(&rcDest, pt.x, pt.y);

	return g_pDDSPrimary->Blt(&rcDest, g_pDDSBack, NULL, DDBLT_WAIT, NULL);
}

struct __clipper__ ;
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
BOOL __DDCSetClip(LPDIRECTDRAWSURFACE7 lpDDS, int x1, int y1, int x2, int y2)
{
	LPRGNDATA pRd;
	HRESULT ddrval;

	if (!g_pDDClipper) {

		ddrval = DirectDrawCreateClipper(0, &g_pDDClipper, NULL);
		if (ddrval != DD_OK) return DDERRCHK(ddrval);
	}

	///// RGNDATA크기+ 버퍼크기 만큼 할당
	pRd = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER) + sizeof(RECT));

	/////
	pRd->rdh.dwSize= sizeof(pRd->rdh);
	pRd->rdh.iType = RDH_RECTANGLES;							    // 사각영역임을 의미
	pRd->rdh.nCount = 1;										    // 1개의 영역데이터
	pRd->rdh.nRgnSize = sizeof(RECT);								// 버퍼데이터의 크기
	SetRect(&(pRd->rdh.rcBound), -8000, -8000, 8000, 8000);		    // 바운더리 영역
	SetRect((RECT*)(pRd->Buffer), x1, y1, x2, y2);					// 원하는 클립영역	

	///// 클립리스트 설정
	ddrval = g_pDDClipper->SetClipList(pRd, 0);
	free(pRd);
	if (ddrval != DD_OK) { return DDERRCHK(ddrval); }

	ddrval = lpDDS->SetClipper(g_pDDClipper);
	if (ddrval != DD_OK) { return DDERRCHK(ddrval); }

	g_pDDClipper->Release();
	g_pDDClipper = NULL;

	return TRUE;


	}
BOOL __DDCSetWindowClipper(LPDIRECTDRAWSURFACE7 lpDDS)
{
	HRESULT ddrval;

	if (!g_pDDClipper)
	{
		ddrval = DirectDrawCreateClipper(0, &g_pDDClipper, NULL);
		if (ddrval != DD_OK) return DDERRCHK(ddrval);
	}

	//// 해당 윈도우에 대해서 크기 변화, 이동시 화면에 보이는 윈도우 클라이언트 영역에 대해서
	//// 클리핑 리스트를 자동으로 갱신하여 적용한다
	ddrval = g_pDDClipper->SetHWnd(0, g_hWnd);
	if (ddrval != DD_OK) { return DDERRCHK(ddrval); }

	ddrval = lpDDS->SetClipper(g_pDDClipper);
	if (ddrval != DD_OK) { return DDERRCHK(ddrval); }

	g_pDDClipper->Release();
	g_pDDClipper = NULL;

	return TRUE;
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

LPDIRECTDRAWSURFACE7 __CreateSurface(DWORD dwWidth, DWORD dwHeight, BOOL bIsVram)
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
LPDIRECTDRAWSURFACE7 CreateSurface(BOOL bIsVram) {

	DDSURFACEDESC2              ddsd;
	DDSCAPS2                    ddscaps;

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	g_pDDSBack->GetSurfaceDesc(&ddsd);
	return __CreateSurface(ddsd.dwWidth, ddsd.dwHeight, bIsVram);

}

void __DDGetAvailableVidMem(LPDWORD lpdwTotal, LPDWORD lpdwFree)
{
	DDSCAPS2 ddscaps;

	::memset(&ddscaps, 0, sizeof(ddscaps));
	ddscaps.dwCaps = DDSCAPS_VIDEOMEMORY;
	g_pDD->GetAvailableVidMem(&ddscaps, lpdwTotal, lpdwFree);
}

void DXW_DisplayInfo()
{
	///// 프레임수 측정 및 기타 정보
	STAPPSetting appstate = g_pGameSystem->getAppState();
	if (appstate.bIsDisplayInfo)
	{
		
		DWORD dwFps = g_pGameSystem->GetFPS();

		//// 
		DWORD dwTotalVRAM, dwUsedVRAM, dwFreeVRAM;
		::__DDGetAvailableVidMem(&dwTotalVRAM, &dwFreeVRAM);
		dwUsedVRAM = dwTotalVRAM - dwFreeVRAM;

		////
		DDCAPS HAL;
		memset(&HAL, 0, sizeof(HAL));
		HAL.dwSize = sizeof(HAL);
		g_pDD->GetCaps(&HAL, NULL);
		/////
		g_pDDSBack->GetDC(&gHdc);
		
		//Display fps
		__PutTextf(10, 10, RED, "FPS %d", dwFps);

		//Display Memory
		__PutTextf(10, 30, RED, "TotalVram %dkb UsedVram %dkb FreeVram %dkb",
			dwTotalVRAM / 1024, dwUsedVRAM / 1024, dwFreeVRAM / 1024);

		//Display capacity
		if (HAL.dwCaps & DDCAPS_BLT) __PutText(10, 50, RED, "HAL - DDCAPS_BLT ");
		if (HAL.dwCaps & DDCAPS_BLTCOLORFILL)
			__PutText(10, 70, RED, "HAL - DDCAPS_BLTCOLORFILL");
		if (HAL.dwFXCaps & DDFXCAPS_BLTROTATION)
			__PutText(10, 70, RED, "HAL - DDFXCAPS_BLTROTATION");

		
		//Display PixelFormat
		switch (g_enPixelFormat)
		{
		case eRGB565: __PutText(10, 90, RED, "PixelFormat - RGB565");
			break;
		case eRGB555: __PutText(10, 90, RED, "PixelFormat - RGB555");
			break;
		case eRGB888: __PutText(10, 90, RED, "PixelFormat - RGB888");
			break;
		case eBGR888: __PutText(10, 90, RED, "PixelFormat - BGR888");
			break;
		case eRGBOther: __PutText(10, 90, RED, "PixelFormat - ???");
			break;
		default:
			break;
		}


		//g_pCurMap->DisplayMapData();


		g_pDDSBack->ReleaseDC(gHdc);
		__PutText(50, 50, BLUE, "bound");
	}
}

