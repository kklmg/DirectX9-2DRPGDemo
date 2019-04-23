#include<ddraw.h>
#include<Windows.h>
#include"DxMng.h"
#include"Text.h"
#pragma once


BOOL DDInit();
BOOL DDInitWin();
BOOL DDSWBlt();

BOOL DXW_Init();

BOOL __DDCSetClip(LPDIRECTDRAWSURFACE7 lpDDS, int x1, int y1, int x2, int y2);
BOOL __DDCSetWindowClipper(LPDIRECTDRAWSURFACE7 lpDDS);

LPDIRECTDRAWSURFACE7 __CreateSurface(DWORD dwWidth, DWORD dwHeight, BOOL bIsVram);
LPDIRECTDRAWSURFACE7 CreateSurface(BOOL bIsVram);

void __DDGetAvailableVidMem(LPDWORD lpdwTotal, LPDWORD lpdwFree);

void DXW_DisplayInfo();