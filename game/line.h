#pragma once
#include <windows.h>
#include <D2D1.h>
#include <ddraw.h>

void DrawLine(LPDIRECTDRAWSURFACE7 suf,int x1,int y1,int x2,int y2) 
{

	DDSURFACEDESC2 desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.dwSize = sizeof(desc);

	if (suf->Lock(0, &desc, DDLOCK_WAIT, 0) == DD_OK) 
	{
		abs(x2-x1);
	}
}