#include"effect.h"
#include"DxMng.h"
#include"Global_data.h"
#include"blit.h"
#include"ddutil.h"
#include<vector>

BOOL _ChangeDisplayMode(DWORD dwWidth, DWORD dwHeight, WORD wBPP)
{
	DEVMODE dm;
	LONG    rval;

	dm.dmPelsWidth = dwWidth;
	dm.dmPelsHeight = dwHeight;
	dm.dmBitsPerPel = wBPP;
	dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

	rval = ChangeDisplaySettings(&dm, 0);
	if (rval != DISP_CHANGE_SUCCESSFUL) return FALSE;

	return TRUE;
}

//check rgbbitcount
enPixelFormat __CheckRGBBit(LPDIRECTDRAWSURFACE7 lpDDS)
{
	DDSURFACEDESC2 ddsd;
	enPixelFormat ePixelFormat = eRGBOther;

	::ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_PIXELFORMAT;


	HRESULT hRet = lpDDS->GetSurfaceDesc(&ddsd);
	if (hRet != DD_OK) { DDERRCHK(hRet); return ePixelFormat; }

	g_nRGBBitCount = ddsd.ddpfPixelFormat.dwRGBBitCount;
	switch (ddsd.ddpfPixelFormat.dwRBitMask)
	{
	case 0x0000F800: ePixelFormat = enPixelFormat::eRGB565; break;    // 11111000 00000000
	case 0x00007C00: ePixelFormat = enPixelFormat::eRGB555; break;    // 01111100 00000000	
	case 0x00FF0000: ePixelFormat = enPixelFormat::eRGB888; break;    // 11111111 00000000 00000000
	case 0x000000FF: ePixelFormat = enPixelFormat::eBGR888; break;    // 00000000 00000000 11111111
	}

	return ePixelFormat;
}

//lock surface
BOOL __Lock(LPDIRECTDRAWSURFACE7 pDDSDest)
{
	::ZeroMemory(&g_ddsd, sizeof(DDSURFACEDESC2));
	g_ddsd.dwSize = sizeof(DDSURFACEDESC2);
	HRESULT hRet = pDDSDest->Lock(NULL, &g_ddsd, DDLOCK_WAIT, NULL);
	if (hRet != DD_OK) { return DDERRCHK(hRet); }

	return TRUE;
}
BOOL __Lock(LPDIRECTDRAWSURFACE7 pDDSDest, DDSURFACEDESC2 &desc)
{
	::ZeroMemory(&desc, sizeof(DDSURFACEDESC2));
	desc.dwSize = sizeof(DDSURFACEDESC2);
	HRESULT hRet = pDDSDest->Lock(NULL, &desc, DDLOCK_WAIT, NULL);
	if (hRet != DD_OK) { return DDERRCHK(hRet); }

	return TRUE;
}

DDSURFACEDESC2 __LockEx(LPDIRECTDRAWSURFACE7 pDDSDest)
{
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	::ZeroMemory(&g_ddsd, sizeof(DDSURFACEDESC2));

	HRESULT hRet = pDDSDest->Lock(NULL, &g_ddsd, DDLOCK_WAIT, NULL);
	//if (hRet != DD_OK) { return ddsd; }

	return ddsd;
}

BOOL __Unlock(LPDIRECTDRAWSURFACE7 pDDSDest)
{
	HRESULT hRet = pDDSDest->Unlock(NULL);
	if (hRet != DD_OK) { return DDERRCHK(hRet); }

	return TRUE;
}

//////////////
//put pixel//
/////////////
void __PutPixel_8(int x, int y, BYTE bColor)
{
	LPBYTE lpDDS = (LPBYTE)g_ddsd.lpSurface;
	lpDDS[y * g_ddsd.lPitch + x] = bColor;
}
void __PutPixel_8(int x, int y, DDSURFACEDESC2 &desc, BYTE bColor)
{
	LPBYTE lpDDS = (LPBYTE)desc.lpSurface;
	lpDDS[y * desc.lPitch + x] = bColor;
}

void __PutPixel_16(int x, int y, WORD wColor)
{
	LPBYTE lpDDS = (LPBYTE)g_ddsd.lpSurface;
	WORD* lpDst = (WORD*)(lpDDS + y*g_ddsd.lPitch + (x * 2));

	*lpDst = wColor;
}
void __PutPixel_16(int x, int y, DDSURFACEDESC2 &desc, WORD wColor)
{
	LPBYTE lpDDS = (LPBYTE)desc.lpSurface;
	WORD* lpDst = (WORD*)(lpDDS + y*desc.lPitch + (x * 2));

	*lpDst = wColor;
}

void __PutPixel_32(int x, int y, DWORD dwColor)
{
	LPBYTE lpDDS = (LPBYTE)g_ddsd.lpSurface;
	DWORD* lpDst = (DWORD*)(lpDDS + y*g_ddsd.lPitch + (x * 4));

	*lpDst = dwColor;
}
void __PutPixel_32(int x, int y, DDSURFACEDESC2 &desc, DWORD dwColor)
{
	LPBYTE lpDDS = (LPBYTE)desc.lpSurface;
	DWORD* lpDst = (DWORD*)(lpDDS + y*desc.lPitch + (x * 4));

	*lpDst = dwColor;
}


//////////////
//get pixel//
/////////////
BYTE __GetPixel_8(int x, int y)
{
	LPBYTE lpDDS = (LPBYTE)g_ddsd.lpSurface;
	BYTE* pPixel = (BYTE*)(lpDDS + y*g_ddsd.lPitch + x);

	return *pPixel;
}
BYTE __GetPixel_8(int x, int y, DDSURFACEDESC2 &desc)
{
	LPBYTE lpDDS = (LPBYTE)desc.lpSurface;
	BYTE* pPixel = (BYTE*)(lpDDS + y*desc.lPitch + x);

	return *pPixel;
}

WORD __GetPixel_16(int x, int y)
{
	LPBYTE lpDDS = (LPBYTE)g_ddsd.lpSurface;
	WORD* pPixel = (WORD*)(lpDDS + y*g_ddsd.lPitch + (x * 2));

	return *pPixel;
}
WORD __GetPixel_16(int x, int y, DDSURFACEDESC2 &desc)
{
	LPBYTE lpDDS = (LPBYTE)desc.lpSurface;
	WORD* pPixel = (WORD*)(lpDDS + y*desc.lPitch + (x * 2));

	return *pPixel;
}

DWORD __GetPixel_32(int x, int y)
{
	LPBYTE lpDDS = (LPBYTE)g_ddsd.lpSurface;
	DWORD* pPixel = (DWORD*)(lpDDS + y*g_ddsd.lPitch + (x * 4));

	return *pPixel;
}
DWORD __GetPixel_32(int x, int y, DDSURFACEDESC2 &desc)
{
	LPBYTE lpDDS = (LPBYTE)desc.lpSurface;
	DWORD* pPixel = (DWORD*)(lpDDS + y*desc.lPitch + (x * 4));

	return *pPixel;
}

///////////////////////////////
//change rgb24 color to rgb16//
///////////////////////////////
WORD __GetPixelRGB_16(int R, int G, int B)
{
	WORD wPixel = 0;

	if (g_enPixelFormat == eRGB565)
	{
		wPixel = (R << 11) | (G << 5) | B;   // 0000 0000 0001 1111		                                 
		return wPixel;					 // 0000 0111 1110 0000   5
	}                                    // 1111 1000 0000 0000  11
	else if (g_enPixelFormat == eRGB555)
	{
		wPixel = (R << 10) | (G << 5) | B;
		return wPixel;
	}
}
WORD __GetPixelRGB_16(DWORD color32)
{
	WORD wPixel = 0;

	if (g_enPixelFormat == eRGB565)
	{
		WORD R16 = (color32 >> 16 >> 3) & 31;
		WORD G16 = (color32 >> 8 >> 2) & 63;
		WORD B16 = (color32 >> 3) & 31;

		wPixel = (R16 << 11) | (G16 << 5) | B16;   // 0000 0000 0001 1111		                                 
		return wPixel;					 // 0000 0111 1110 0000   5
	}                                    // 1111 1000 0000 0000  11
	else if (g_enPixelFormat == eRGB555)
	{
		WORD R16 = (color32 >> 16 >> 3) & 31;
		WORD G16 = (color32 >> 8 >> 3) & 31;
		WORD B16 = (color32 >> 3) & 31;


		wPixel = (R16 << 10) | (G16 << 5) | B16;
		return wPixel;
	}
}

//----------------------------------------
//Decompose color
//----------------------------------------
void DeComposeColor_RGB565(WORD &color16,STColor16 &GetColor) 
{
	GetColor.R = ((color16 >> 11) & 31);
	GetColor.G = ((color16 >> 5) & 63);
	GetColor.B = (color16 & 31);
}
void DeComposeColor_RGB555(WORD &color16, STColor16 &GetColor)
{
	GetColor.R = ((color16 >> 10) & 31);
	GetColor.G = ((color16 >> 5) & 31);
	GetColor.B = (color16 & 31);
}

void DeComposeColor_RGB888(DWORD &color32,STColor32 &GetColor)
{
	GetColor.A = color32 >> 24;
	GetColor.R = color32 >> 16;
	GetColor.G = color32 >> 8;
	GetColor.B = color32;
}
void DeComposeColor_BGR888(DWORD &color32, STColor32 &GetColor)
{
	GetColor.A = color32 >> 24;
	GetColor.B = color32 >> 16;
	GetColor.G = color32 >> 8;
	GetColor.R = color32;
}

//----------------------------------------
//compose clor
//----------------------------------------
void ComposeColor_RGB565(STColor16 &color16, WORD &GetColor)
{
	GetColor= (color16.R << 11) | (color16.G << 6) | color16.B;
}
void ComposeColor_RGB555(STColor16 &color16, WORD &GetColor)
{
	GetColor = (color16.R << 10) | (color16.G << 5) | color16.B;
}

void ComposeColor_RGB888(STColor32 &color32, DWORD &GetColor)
{
	GetColor = ((color32.A << 24) | (color32.R << 16) | (color32.G << 8) | color32.B);
}
void ComposeColor_BGR888(STColor32 &color32, DWORD &GetColor)
{
	GetColor = ((color32.A << 24) | (color32.B << 16) | (color32.G << 8) | color32.R);
}

//Change Color
void ChangeColor(STColor16 &in, STColor16 &out,void(*fptrChange)(STColor16&, STColor16 &))
{
	fptrChange(in, out);
}

void ChangeColor(STColor32 &in, STColor32 &out, void(*fptrChange)(STColor32&, STColor32 &))
{
	fptrChange(in, out);
}


LPDIRECTDRAWSURFACE7 __GrayScale(LPDIRECTDRAWSURFACE7 pDDSDest)
{
	LPDIRECTDRAWSURFACE7 tempDDS = NULL;

	DDSURFACEDESC2 ddsd;

	//Get Surface width & height__________________	
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;

	pDDSDest->GetSurfaceDesc(&ddsd);

	//create and copy
	tempDDS = CreateSurfaceEX(ddsd.dwWidth, ddsd.dwHeight, TRUE);

	//match color key
	DDCOLORKEY              ddck;
	bool bExitstCK = FALSE;

	//get source surface kolor key
	if (pDDSDest->GetColorKey(DDCKEY_SRCBLT, &ddck) == DD_OK)
	{
		bExitstCK = TRUE;
		//set color key
		tempDDS->SetColorKey(DDCKEY_SRCBLT, &ddck);
	}

	tempDDS->BltFast(0, 0, pDDSDest, NULL, NULL);
	//tempDDS->Blt(NULL, pDDSDest, NULL, DDBLT_WAIT, NULL);
	


	if (__Lock(tempDDS))
	{
		for (int x = 0; x < ddsd.dwWidth; x++) 
		{
			for (int y = 0; y < ddsd.dwHeight; y++)
			{
				switch (g_enPixelFormat)
				{
					case eRGB565:
					{
						WORD color = __GetPixel_16(x, y);

						//Skip Color Key 					
						if (bExitstCK)
						{
							if (color == ddck.dwColorSpaceHighValue
								|| color == ddck.dwColorSpaceLowValue)continue;
						}


						WORD R = ((color >> 11) & 31);
						WORD G = ((color >> 5) & 63);
						WORD B = (color & 31);


						WORD Gray = (R + G / 2 + B) / 3;

						WORD result = (Gray << 11) | ((Gray*2) << 6) | Gray;

						__PutPixel_16(x, y, result);
					}
						break;
					case eRGB555:
					{
						WORD color = __GetPixel_16(x, y);

						//Skip Color Key 					
						if (bExitstCK)
						{
							if (color == ddck.dwColorSpaceHighValue
								|| color == ddck.dwColorSpaceLowValue)continue;
						}


						WORD R = ((color >> 11) & 31);
						WORD G = ((color >> 5) & 31);
						WORD B = (color & 31);

						WORD Gray = (R + G + B) / 3;

						WORD result = (Gray << 10) | (Gray << 5) | Gray;

						__PutPixel_16(x, y, result);
					}
						break;
					case eRGB888:
					{				
						//get color
						DWORD color = __GetPixel_32(x, y);
						
						//Skip Color Key 					
						if (bExitstCK)
						{
							if (color == ddck.dwColorSpaceHighValue
								|| color == ddck.dwColorSpaceLowValue)continue;
						}

						//decompose color
						BYTE A = color >> 24;
						BYTE R = color >>16;
						BYTE G = color >> 8;
						BYTE B = color;

						//figure out color
						BYTE Gray = (R + G + B) / 3;

						//compose color
						DWORD result = ((A<<24) | (Gray << 16) | (Gray << 8) | Gray);

						//save color
						__PutPixel_32(x, y, result);
					}
						break;
					case eBGR888:
					{

						DWORD color = __GetPixel_32(x, y);

						//Skip Color Key 					
						if (bExitstCK)
						{
							if (color == ddck.dwColorSpaceHighValue
								|| color == ddck.dwColorSpaceLowValue)continue;
						}

						BYTE A = color >> 24;
						BYTE B = color >> 16;
						BYTE G = color >> 8;
						BYTE R = color;

						BYTE Gray = (R + G + B) / 3;

						DWORD result = (A << 24) | (Gray << 16) | (Gray << 8) | Gray;

						__PutPixel_32(x, y, result);

					}
						break;
					case eRGBOther:
						break;
					default:
						break;
				}


			}
		}

		__Unlock(tempDDS);

		//pDDSDest->BltFast(0, 0, surface, NULL, NULL);
		//pDDSDest->Blt(NULL, g_effect_surface, NULL, DDBLT_WAIT, NULL);
	}

	return tempDDS;
}

LPDIRECTDRAWSURFACE7 __Colorize(LPDIRECTDRAWSURFACE7 pDDSDest,int dR,int dG,int dB)
{
	LPDIRECTDRAWSURFACE7 tempDDS = NULL;

	DDSURFACEDESC2 ddsd;

	//Get Surface width & height__________________	
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	
	pDDSDest->GetSurfaceDesc(&ddsd);

	
	//create and copy
	tempDDS = CreateSurfaceEX(ddsd.dwWidth, ddsd.dwHeight, TRUE);

	//match color key
	DDCOLORKEY              ddck;
	bool bExitstCK = FALSE;

	//get source surface kolor key
	if (pDDSDest->GetColorKey(DDCKEY_SRCBLT, &ddck) == DD_OK) 
	{
		bExitstCK = TRUE;
		//set color key
		tempDDS->SetColorKey(DDCKEY_SRCBLT, &ddck);
	}

	tempDDS->BltFast(0, 0, pDDSDest, NULL, NULL);

	//tempDDS->Blt(NULL, pDDSDest, NULL, DDBLT_WAIT, NULL);


	if (__Lock(tempDDS))
	{
		for (int x = 0; x < ddsd.dwWidth; x++)
		{
			for (int y = 0; y < ddsd.dwHeight; y++)
			{
				switch (g_enPixelFormat)
				{
				case eRGB565:
				{
					WORD color = __GetPixel_16(x, y);


					//Skip Color Key 					
					if (bExitstCK) 
					{
						if (color == ddck.dwColorSpaceHighValue
							|| color == ddck.dwColorSpaceLowValue)continue;
					}
					//__GetPixelRGB_16(colorkey));


					WORD R = ((color >> 11) & 31);
					WORD G = ((color >> 5) & 63);
					WORD B = (color & 31);


					WORD dR16 = (dR >> 3) & 31;
					WORD dG16 = (dG >> 2) & 63;
					WORD dB16 = (dB >> 3) & 31;


					WORD rR = R*((float)dR16 / 31);
					WORD rG= G*((float)dG16 / 63);
					WORD rB= B*((float)dB16 / 31);
					

					WORD result = (rR << 11) | ((rG * 2) << 6) | rB;

					__PutPixel_16(x, y, result);
				}
				break;
				case eRGB555:
				{
					WORD color = __GetPixel_16(x, y);

					//Skip Color Key 					
					if (bExitstCK)
					{
						if (color == ddck.dwColorSpaceHighValue
							|| color == ddck.dwColorSpaceLowValue)continue;
					}

					WORD R = ((color >> 11) & 31);
					WORD G = ((color >> 5) & 31);
					WORD B = (color & 31);


					WORD dR16 = (dR >> 3) & 31;
					WORD dG16 = (dG >> 3) & 31;
					WORD dB16 = (dB >> 3) & 31;

					WORD rR = R*((float)dR16 / 31);
					WORD rG = G*((float)dG16 / 31);
					WORD rB = B*((float)dB16 / 31);



					WORD result = (rR << 10) | (rG << 5) | rB;

					__PutPixel_16(x, y, result);
				}
				break;
				case eRGB888:
				{
					DWORD color = __GetPixel_32(x, y);

					//Skip Color Key 					
					if (bExitstCK)
					{
						if (color == ddck.dwColorSpaceHighValue
							|| color == ddck.dwColorSpaceLowValue)continue;
					}

					BYTE A = color >> 24;
					BYTE R = color >> 16;
					BYTE G = color >> 8;
					BYTE B = color;

					
					BYTE rR = (float)(R*dR) / 255;
					BYTE rG = (float)(G*dG) / 255;
					BYTE rB = (float)(B*dB) / 255;


					DWORD result = ((A << 24) | (rR << 16) | (rG << 8) | rB);

					__PutPixel_32(x, y, result);



				}
				break;
				case eBGR888:
				{

					DWORD color = __GetPixel_32(x, y);

					//Skip Color Key 					
					if (bExitstCK)
					{
						if (color == ddck.dwColorSpaceHighValue
							|| color == ddck.dwColorSpaceLowValue)continue;
					}

					BYTE A = color >> 24;
					BYTE B = color >> 16;
					BYTE G = color >> 8;
					BYTE R = color;

					BYTE rR = (float)(R*dR) / 255;
					BYTE rG = (float)(G*dG) / 255;
					BYTE rB = (float)(B*dB) / 255;


					DWORD result = (A << 24) | (rB << 16) | (rG << 8) | rR;

					__PutPixel_32(x, y, result);

				}
				break;
				case eRGBOther:
					break;
				default:
					break;
				}


			}
		}

		__Unlock(tempDDS);

		//pDDSDest->BltFast(0, 0, surface, NULL, NULL);
		//pDDSDest->Blt(NULL, g_effect_surface, NULL, DDBLT_WAIT, NULL);
	}

	return tempDDS;
}

std::vector<LPDIRECTDRAWSURFACE7>* __CreateColorizeSurface(LPDIRECTDRAWSURFACE7 pDDSSrc)
{
	std::vector<LPDIRECTDRAWSURFACE7> *SufVector = new std::vector<LPDIRECTDRAWSURFACE7>(15);

	//get gray colorsize first!!
	(*SufVector)[enColor::eCrGray] = __GrayScale(pDDSSrc);
	
	(*SufVector)[enColor::eCrBlack]		= __Colorize((*SufVector)[enColor::eCrGray], BLACK_dcp	);
	(*SufVector)[enColor::eCrWhite]		= __Colorize((*SufVector)[enColor::eCrGray], WHITE_dcp	);
	(*SufVector)[enColor::eCrRed]	    = __Colorize((*SufVector)[enColor::eCrGray], RED_dcp	);
	(*SufVector)[enColor::eCrYellow]	= __Colorize((*SufVector)[enColor::eCrGray], YELLOW_dcp);
	(*SufVector)[enColor::eCrGreen]		= __Colorize((*SufVector)[enColor::eCrGray], GREEN_dcp	);
	(*SufVector)[enColor::eCrBlue]		= __Colorize((*SufVector)[enColor::eCrGray], BLUE_dcp	);
	(*SufVector)[enColor::eCrOrange]	= __Colorize((*SufVector)[enColor::eCrGray], ORANGE_dcp);
	(*SufVector)[enColor::eCrGold]		= __Colorize((*SufVector)[enColor::eCrGray], GOLD_dcp	);
	(*SufVector)[enColor::eCrPink]		= __Colorize((*SufVector)[enColor::eCrGray], Pink_dcp	);
	(*SufVector)[enColor::eCrPurple]	= __Colorize((*SufVector)[enColor::eCrGray], PURPLE_dcp);

	return SufVector;
}

LPDIRECTDRAWSURFACE7 __GetAlphaBlend(LPDIRECTDRAWSURFACE7 pDDSSrc_front, LPDIRECTDRAWSURFACE7 pDDSSrc_back, RECT Destrect,float alpha) 
{
	HRESULT hr;

	LPDIRECTDRAWSURFACE7 ResultDDS = NULL;
	LPDIRECTDRAWSURFACE7 FrontDDS = NULL;
	LPDIRECTDRAWSURFACE7 BackDDS = NULL;

	DDCOLORKEY  ddck_front;
	bool bExitstCK = FALSE;

	int width = Destrect.right - Destrect.left;
	int Height = Destrect.bottom - Destrect.top;

	ResultDDS= CreateSurfaceEX(width, Height, TRUE);
	FrontDDS = CreateSurfaceEX(width, Height, TRUE);
	BackDDS = CreateSurfaceEX(width, Height, TRUE);
	
	//Get Color key
	if (pDDSSrc_front->GetColorKey(DDCKEY_SRCBLT, &ddck_front) == DD_OK)
	{
		bExitstCK = TRUE;
	}

	//Init Back Surface
	hr = BackDDS->Blt(NULL, pDDSSrc_back, &Destrect, DDBLT_WAIT, NULL);
	if (hr != DD_OK) {  DDERRCHK(hr); }
	//Init Front Surface
	if (bExitstCK) 
	{	
		hr = FrontDDS->Blt(NULL, pDDSSrc_front, NULL, DDBLT_KEYSRC | DDBLT_WAIT, NULL);
		if (hr != DD_OK) { DDERRCHK(hr); }
	}

	else 
	{
		hr = FrontDDS->Blt(NULL, pDDSSrc_front, NULL, DDBLT_WAIT, NULL);
		if (hr != DD_OK) { DDERRCHK(hr); }
	}
	


	//Ready Stage
	float Alpha_bk = 1 - alpha;
	
	DDSURFACEDESC2 ddsd_result;
	DDSURFACEDESC2 ddsd_front;
	DDSURFACEDESC2 ddsd_back;

	//--------------------------------------
	//16 bit color
	//--------------------------------------
	WORD Color16_front;
	WORD Color16_back;
	WORD Color16_result;

	STColor16 DColor16_front;
	STColor16 DColor16_back;
	STColor16 DColor16_result;

	//--------------------------------------
	//32bit color
	//--------------------------------------
	DWORD Color32_front;
	DWORD Color32_back;
	DWORD Color32_result;
	 
	STColor32 DColor32_front;
	STColor32 DColor32_back;
	STColor32 DColor32_result;

	//lock three surface
	if (__Lock(ResultDDS, ddsd_result) && __Lock(FrontDDS, ddsd_front) && __Lock(BackDDS, ddsd_back))
	{
		int x = 0; int y = 0;

		for (x = 0; x < width; x++)
		{
			for (y = 0; y < Height; y++)
			{
				switch (g_enPixelFormat)
				{
				case eRGB565:
				{
					//get pixel
					Color16_front = __GetPixel_16(x, y, ddsd_front);
					Color16_back = __GetPixel_16(x, y, ddsd_back);

					//Decompose pixel
					DeComposeColor_RGB565(Color16_back, DColor16_back);
					DeComposeColor_RGB565(Color16_front, DColor16_front);

					//Calculate pixel
					DColor16_result.R = alpha*DColor16_front.R + Alpha_bk*DColor16_back.R;
					DColor16_result.G = alpha*DColor16_front.G + Alpha_bk*DColor16_back.G;
					DColor16_result.B = alpha*DColor16_front.B + Alpha_bk*DColor16_back.B;

					//Compose pixel
					ComposeColor_RGB565(DColor16_result, Color16_result);

					//put color
					__PutPixel_16(x, y, ddsd_result, Color16_result);
				}
				break;
				case eRGB555:
				{
					//get pixel
					Color16_front = __GetPixel_16(x, y, ddsd_front);
					Color16_back = __GetPixel_16(x, y, ddsd_back);

					//Decompose pixel
					DeComposeColor_RGB555(Color16_back, DColor16_back);
					DeComposeColor_RGB555(Color16_front, DColor16_front);

					//Calculate pixel
					DColor16_result.R = alpha*DColor16_front.R + Alpha_bk*DColor16_back.R;
					DColor16_result.G = alpha*DColor16_front.G + Alpha_bk*DColor16_back.G;
					DColor16_result.B = alpha*DColor16_front.B + Alpha_bk*DColor16_back.B;

					//Compose pixel
					ComposeColor_RGB555(DColor16_result, Color16_result);

					//put color
					__PutPixel_16(x, y, ddsd_result, Color16_result);
				}
				break;
				case eRGB888:
				{
					//get pixel
					Color32_front = __GetPixel_32(x, y, ddsd_front);
					Color32_back = __GetPixel_32(x, y, ddsd_back);

					//if it's color key
					if (bExitstCK  && Color32_front == 0)
					{
						__PutPixel_32(x, y, ddsd_result, Color32_back);
						continue;
					}

				
					

					//Decompose pixel
					DeComposeColor_RGB888(Color32_back, DColor32_back);
					DeComposeColor_RGB888(Color32_front, DColor32_front);

					//Calculate pixel
					DColor32_result.R = alpha*DColor32_front.R + Alpha_bk*DColor32_back.R;
					DColor32_result.G = alpha*DColor32_front.G + Alpha_bk*DColor32_back.G;
					DColor32_result.B = alpha*DColor32_front.B + Alpha_bk*DColor32_back.B;

					//Compose pixel
					ComposeColor_RGB888(DColor32_result, Color32_result);

					//put color
					__PutPixel_32(x, y, ddsd_result, Color32_result);
				}
				break;
				case eBGR888:
				{
					//get pixel
					Color32_front = __GetPixel_32(x, y, ddsd_front);
					Color32_back = __GetPixel_32(x, y, ddsd_back);

					//Decompose pixel
					DeComposeColor_BGR888(Color32_back, DColor32_back);
					DeComposeColor_BGR888(Color32_front, DColor32_front);

					//Calculate pixel
					DColor32_result.R = alpha*DColor32_front.R + Alpha_bk*DColor32_back.R;
					DColor32_result.G = alpha*DColor32_front.G + Alpha_bk*DColor32_back.G;
					DColor32_result.B = alpha*DColor32_front.B + Alpha_bk*DColor32_back.B;

					//Compose pixel
					ComposeColor_BGR888(DColor32_result, Color32_result);

					//put color
					__PutPixel_32(x, y, ddsd_result, Color32_result);

				}
				break;
				case eRGBOther:
					break;
				default:
					break;
				}
			}
		}
		__Unlock(ResultDDS);
		__Unlock(FrontDDS);
		__Unlock(BackDDS);
	}

	if (FrontDDS != NULL) 
	{
		FrontDDS->Release();
		FrontDDS = NULL;
	}

	if (BackDDS != NULL)
	{
		BackDDS->Release();
		BackDDS = NULL;
	}

	return ResultDDS;

}

bool __AlphaBlend(LPDIRECTDRAWSURFACE7 pDDSSrc_front, LPDIRECTDRAWSURFACE7 pDDSSrc_back, RECT Destrect, float alpha, LPDIRECTDRAWSURFACE7 *GetSuf)
{
	HRESULT hr;

	LPDIRECTDRAWSURFACE7 FrontDDS = NULL;
	LPDIRECTDRAWSURFACE7 BackDDS = NULL;

	DDCOLORKEY  ddck_front;
	bool bExitstCK = FALSE;

	int width = Destrect.right - Destrect.left;
	int Height = Destrect.bottom - Destrect.top;

	FrontDDS = CreateSurfaceEX(width, Height, TRUE);
	BackDDS = CreateSurfaceEX(width, Height, TRUE);

	if ((*GetSuf) == NULL) 
	{
		(*GetSuf) = CreateSurfaceEX(width, Height, TRUE);	
	}

	//Get Color key
	if (pDDSSrc_front->GetColorKey(DDCKEY_SRCBLT, &ddck_front) == DD_OK)
	{
		bExitstCK = TRUE;
	}

	//Init Back Surface
	hr = BackDDS->Blt(NULL, pDDSSrc_back, &Destrect, DDBLT_WAIT, NULL);
	if (hr != DD_OK) { DDERRCHK(hr); }
	//Init Front Surface
	if (bExitstCK)
	{
		hr = FrontDDS->Blt(NULL, pDDSSrc_front, NULL, DDBLT_KEYSRC | DDBLT_WAIT, NULL);
		if (hr != DD_OK) { DDERRCHK(hr); }
	}

	else
	{
		hr = FrontDDS->Blt(NULL, pDDSSrc_front, NULL, DDBLT_WAIT, NULL);
		if (hr != DD_OK) { DDERRCHK(hr); }
	}



	//Ready Stage
	float Alpha_bk = 1 - alpha;

	DDSURFACEDESC2 ddsd_result;
	DDSURFACEDESC2 ddsd_front;
	DDSURFACEDESC2 ddsd_back;

	//--------------------------------------
	//16 bit color
	//--------------------------------------
	WORD Color16_front;
	WORD Color16_back;
	WORD Color16_result;

	STColor16 DColor16_front;
	STColor16 DColor16_back;
	STColor16 DColor16_result;

	//--------------------------------------
	//32bit color
	//--------------------------------------
	DWORD Color32_front;
	DWORD Color32_back;
	DWORD Color32_result;

	STColor32 DColor32_front;
	STColor32 DColor32_back;
	STColor32 DColor32_result;

	//lock three surface
	if (__Lock((*GetSuf), ddsd_result) && __Lock(FrontDDS, ddsd_front) && __Lock(BackDDS, ddsd_back))
	{
		int x = 0; int y = 0;

		for (x = 0; x < width; x++)
		{
			for (y = 0; y < Height; y++)
			{
				switch (g_enPixelFormat)
				{
				case eRGB565:
				{
					//get pixel
					Color16_front = __GetPixel_16(x, y, ddsd_front);
					Color16_back = __GetPixel_16(x, y, ddsd_back);

					//Decompose pixel
					DeComposeColor_RGB565(Color16_back, DColor16_back);
					DeComposeColor_RGB565(Color16_front, DColor16_front);

					//Calculate pixel
					DColor16_result.R = alpha*DColor16_front.R + Alpha_bk*DColor16_back.R;
					DColor16_result.G = alpha*DColor16_front.G + Alpha_bk*DColor16_back.G;
					DColor16_result.B = alpha*DColor16_front.B + Alpha_bk*DColor16_back.B;

					//Compose pixel
					ComposeColor_RGB565(DColor16_result, Color16_result);

					//put color
					__PutPixel_16(x, y, ddsd_result, Color16_result);
				}
				break;
				case eRGB555:
				{
					//get pixel
					Color16_front = __GetPixel_16(x, y, ddsd_front);
					Color16_back = __GetPixel_16(x, y, ddsd_back);

					//Decompose pixel
					DeComposeColor_RGB555(Color16_back, DColor16_back);
					DeComposeColor_RGB555(Color16_front, DColor16_front);

					//Calculate pixel
					DColor16_result.R = alpha*DColor16_front.R + Alpha_bk*DColor16_back.R;
					DColor16_result.G = alpha*DColor16_front.G + Alpha_bk*DColor16_back.G;
					DColor16_result.B = alpha*DColor16_front.B + Alpha_bk*DColor16_back.B;

					//Compose pixel
					ComposeColor_RGB555(DColor16_result, Color16_result);

					//put color
					__PutPixel_16(x, y, ddsd_result, Color16_result);
				}
				break;
				case eRGB888:
				{
					//get pixel
					Color32_front = __GetPixel_32(x, y, ddsd_front);
					Color32_back = __GetPixel_32(x, y, ddsd_back);

					//if it's color key
					if (bExitstCK  && Color32_front == 0)
					{
						__PutPixel_32(x, y, ddsd_result, Color32_back);
						continue;
					}




					//Decompose pixel
					DeComposeColor_RGB888(Color32_back, DColor32_back);
					DeComposeColor_RGB888(Color32_front, DColor32_front);

					//Calculate pixel
					DColor32_result.R = alpha*DColor32_front.R + Alpha_bk*DColor32_back.R;
					DColor32_result.G = alpha*DColor32_front.G + Alpha_bk*DColor32_back.G;
					DColor32_result.B = alpha*DColor32_front.B + Alpha_bk*DColor32_back.B;

					//Compose pixel
					ComposeColor_RGB888(DColor32_result, Color32_result);

					//put color
					__PutPixel_32(x, y, ddsd_result, Color32_result);
				}
				break;
				case eBGR888:
				{
					//get pixel
					Color32_front = __GetPixel_32(x, y, ddsd_front);
					Color32_back = __GetPixel_32(x, y, ddsd_back);

					//Decompose pixel
					DeComposeColor_BGR888(Color32_back, DColor32_back);
					DeComposeColor_BGR888(Color32_front, DColor32_front);

					//Calculate pixel
					DColor32_result.R = alpha*DColor32_front.R + Alpha_bk*DColor32_back.R;
					DColor32_result.G = alpha*DColor32_front.G + Alpha_bk*DColor32_back.G;
					DColor32_result.B = alpha*DColor32_front.B + Alpha_bk*DColor32_back.B;

					//Compose pixel
					ComposeColor_BGR888(DColor32_result, Color32_result);

					//put color
					__PutPixel_32(x, y, ddsd_result, Color32_result);

				}
				break;
				case eRGBOther:
					break;
				default:
					break;
				}
			}
		}
		__Unlock((*GetSuf));
		__Unlock(FrontDDS);
		__Unlock(BackDDS);
	}

	if (FrontDDS != NULL)
	{
		FrontDDS->Release();
		FrontDDS = NULL;
	}

	if (BackDDS != NULL)
	{
		BackDDS->Release();
		BackDDS = NULL;
	}
	return TRUE;
}


/////////////
//Draw Line//
/////////////





bool __DrawVLineEX(int x, int y1, int y2,int R, int G, int B, LPDIRECTDRAWSURFACE7 pDDSDest)
{
	//Get Surface width & height__________________
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;

	pDDSDest->GetSurfaceDesc(&ddsd);

	//avoid the pixel over the surface size
	if (x > ddsd.dwWidth || x < 0) return TRUE;
	if (y1 == y2)return TRUE;
	if (y1 < 0 && y2 < 0)return TRUE;
	if (y2 > ddsd.dwHeight && y2 > ddsd.dwHeight)return TRUE;

	int fix_y1, fix_y2;

	if (y1 > y2) 
	{
		fix_y1 = y2;
		fix_y2 = y1;
	}
	else
	{
		fix_y1 = y1;
		fix_y2 = y2;
	}


	if (fix_y2 > ddsd.dwHeight) fix_y2 = ddsd.dwHeight;

	//check rgb count
	enPixelFormat ePixelFormat = __CheckRGBBit(pDDSDest);

	int tempY;

	//lock surface
	//__Lock(pDDSDest);

	switch (ePixelFormat)
	{
		case eRGB565: 	
		case eRGB555: 
		{
		/*	for (int tempY = y1; tempY < y2; tempY++)
			{
				__PutPixel_16(x, tempY, __GetPixelRGB_16(R,G,B, ePixelFormat));
			}*/
		}
			break;
		case eRGB888:
		case eBGR888:
		{
			/*for (int tempY = fix_y1; tempY < fix_y2; tempY++)
			{
				__PutPixel_32(x, tempY, RGB(R, G, B));
			}
*/
		}
			break;
		case eRGBOther:
			break;
		default:
			break;
	}
	//unlock surface
	//__Unlock(pDDSDest);
}

