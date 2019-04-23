#pragma once
#include<ddraw.h>
#include<vector>

enum enPixelFormat
{
	eRGB565,
	eRGB555,
	eRGB888,
	eBGR888,
	eRGBOther
};

struct STColor16 
{
	WORD R,G,B;
};

struct STColor32
{
	BYTE A, R, G, B;
};

BOOL _ChangeDisplayMode(DWORD dwWidth, DWORD dwHeight, WORD wBPP);

//check rgbformat
enPixelFormat __CheckRGBBit(LPDIRECTDRAWSURFACE7 lpDDS);

//lock surface
BOOL __Lock(LPDIRECTDRAWSURFACE7 pDDSDest);
BOOL __Lock(LPDIRECTDRAWSURFACE7 pDDSDest, DDSURFACEDESC2 &desc);

BOOL __Unlock(LPDIRECTDRAWSURFACE7 pDDSDest);

//////////////
//put pixel//
/////////////
void __PutPixel_8(int x, int y, BYTE bColor);
void __PutPixel_8(int x, int y, DDSURFACEDESC2 &desc, BYTE bColor);

void __PutPixel_16(int x, int y, WORD wColor);
void __PutPixel_16(int x, int y, DDSURFACEDESC2 &desc, WORD bColor);

void __PutPixel_32(int x, int y, DWORD dwColor);
void __PutPixel_32(int x, int y, DDSURFACEDESC2 &desc, DWORD bColor);


//////////////
//get pixel//
/////////////
BYTE __GetPixel_8(int x, int y);
BYTE __GetPixel_8(int x, int y, DDSURFACEDESC2 &desc);

WORD __GetPixel_16(int x, int y);
WORD __GetPixel_16(int x, int y, DDSURFACEDESC2 &desc);

DWORD __GetPixel_32(int x, int y);
DWORD __GetPixel_32(int x, int y, DDSURFACEDESC2 &desc);

//////////////////////
//change 24bit color//
//////////////////////

WORD __GetPixelRGB_16(int R, int G, int B, enPixelFormat pixelformat);

//----------------------------------------
//Decompose color
//----------------------------------------
void DeComposeColor_RGB565(WORD &color16, STColor16 &GetColor);
void DeComposeColor_RGB555(WORD &color16, STColor16 &GetColor);

void DeComposeColor_RGB888(DWORD &color32, STColor32 &GetColor);
void DeComposeColor_BGR888(DWORD &color32, STColor32 &GetColor);

//----------------------------------------
//compose clor
//----------------------------------------
void ComposeColor_RGB565(STColor16 &color16, WORD &GetColor);
void ComposeColor_RGB555(STColor16 &color16, WORD &GetColor);

void ComposeColor_RGB888(STColor32 &color32, DWORD &GetColor);
void ComposeColor_BGR888(STColor32 &color32, DWORD &GetColor);



/////////////
//Draw Line//
/////////////
bool __DrawVLineEX(int x, int y1, int y2, int R, int G, int B, LPDIRECTDRAWSURFACE7 pDDSDest);

LPDIRECTDRAWSURFACE7 __GrayScale(LPDIRECTDRAWSURFACE7 pDDSDest);

LPDIRECTDRAWSURFACE7 __Colorize(LPDIRECTDRAWSURFACE7 pDDSDest, int dR, int dG, int dB);

std::vector<LPDIRECTDRAWSURFACE7>* __CreateColorizeSurface(LPDIRECTDRAWSURFACE7 pDDSSrc);

LPDIRECTDRAWSURFACE7 __GetAlphaBlend(LPDIRECTDRAWSURFACE7 pDDSSrc_front, LPDIRECTDRAWSURFACE7 pDDSSrc_back, RECT Destrect, float alpha);

bool __AlphaBlend(LPDIRECTDRAWSURFACE7 pDDSSrc_front, LPDIRECTDRAWSURFACE7 pDDSSrc_back, RECT Destrect, float alpha, LPDIRECTDRAWSURFACE7 *GetSuf);



//bool __GrayScale(LPDIRECTDRAWSURFACE7 pDDSDest);
