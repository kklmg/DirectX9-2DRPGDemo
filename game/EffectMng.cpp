#include"EffectMng.h"
#include"Global_data.h"
#include"blit.h"
#include"ddutil.h"

CEffectMng::CEffectMng() 
{
	m_enTrgFadeIn = enTriggerState::eDefault;
	m_enTrgFadeOut = enTriggerState::eDefault;
	m_enTrgFadeLoop = enTriggerState::eDefault;
	m_enTrgFadeTo = enTriggerState::eDefault;

	m_pDDSVBuffer = NULL;
	m_pDDSVBuffer2 = NULL; 
	m_pDDSSBuffer = NULL; 
	m_pDDSSBuffer2 = NULL;
}

CEffectMng::~CEffectMng() 
{
}

bool CEffectMng::Init() 
{
	LookUpTableInit();



	return TRUE;
}

void CEffectMng::LookUpTableInit()
{
	switch (g_enPixelFormat)
	{
		//case Bpp 16
		case eRGB555:
		case eRGB565:
		{
			////Create Fade Look Up Table 
			for (DWORD dwDepth = 0; dwDepth < 64; dwDepth++)
			{
				for (DWORD dwRate = 0; dwRate <= 100; dwRate++)
				{
					m_dwFADELUT_16[dwDepth][dwRate] = dwDepth * dwRate / 100;
				}
			}
		}break;

		//case Bpp 32
		case eBGR888:
		case eRGB888:
		{
			//// Fade Look Up Table 생성
			for (DWORD dwDepth = 0; dwDepth < 255; dwDepth++)
			{
				for (DWORD dwRate = 0; dwRate <= 100; dwRate++)
				{
					m_dwFADELUT_32[dwDepth][dwRate] = dwDepth * dwRate / 100;
				}
			}
		}break;


		default:
			break;
	}

}


BOOL CEffectMng::CreateFastVBuffer(DWORD dwWidth, DWORD dwHeight, COLORREF rgb)
{
	HRESULT ddrval;

	if (m_pDDSVBuffer)
	{
		ddrval = m_pDDSVBuffer->Release();
		if (ddrval != DD_OK) { DDERRCHK(ddrval); return FALSE; }
	}

	m_pDDSVBuffer = CreateSurfaceEX(dwWidth, dwHeight, TRUE);
	if (!m_pDDSVBuffer) return FALSE;

	if (BPP >= 16 && rgb != NULL)
	{
		ddrval = DDSetColorKey(m_pDDSVBuffer, rgb);
		if (ddrval != DD_OK) DDERRCHK(ddrval);
	}

	return TRUE;
}

BOOL CEffectMng::CreateFastVBuffer2(DWORD dwWidth, DWORD dwHeight, COLORREF rgb)
{
	HRESULT ddrval;

	if (m_pDDSVBuffer2)
	{
		ddrval = m_pDDSVBuffer2->Release();
		if (ddrval != DD_OK) { DDERRCHK(ddrval); return FALSE; }
	}

	m_pDDSVBuffer2 = CreateSurfaceEX(dwWidth, dwHeight, TRUE);
	if (!m_pDDSVBuffer2) return FALSE;

	if (BPP >= 16 && rgb != NULL)
	{
		ddrval = DDSetColorKey(m_pDDSVBuffer2, rgb);
		if (ddrval != DD_OK) DDERRCHK(ddrval);
	}

	return TRUE;
}

BOOL CEffectMng::CreateFastSBuffer(DWORD dwWidth, DWORD dwHeight, COLORREF rgb)
{
	HRESULT ddrval;

	if (m_pDDSSBuffer)
	{
		ddrval = m_pDDSSBuffer->Release();
		if (ddrval != DD_OK) { DDERRCHK(ddrval); return FALSE; }
	}

	m_pDDSSBuffer = CreateSurfaceEX(dwWidth, dwHeight, FALSE);
	if (!m_pDDSSBuffer) return FALSE;

	if (BPP >= 16)
	{
		ddrval = DDSetColorKey(m_pDDSSBuffer, rgb);
		if (ddrval != DD_OK) DDERRCHK(ddrval);
	}
	return TRUE;
}

BOOL CEffectMng::CreateFastSBuffer2(DWORD dwWidth, DWORD dwHeight, COLORREF rgb)
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


void CEffectMng::PlayFadeIn() 
{
	m_enTrgFadeIn = enTriggerState::eRunning;
}

void CEffectMng::PlayFadeOut() 
{
	m_enTrgFadeOut = enTriggerState::eRunning;
}

void CEffectMng::PlayFadeLoop() 
{
	m_enTrgFadeLoop = enTriggerState::eRunning;
}

bool CEffectMng::IsFadeInEnd()
{
	if (m_enTrgFadeIn == eEnd)return TRUE;
	else return FALSE;
}

bool CEffectMng::IsFadeOutEnd()
{
	if (m_enTrgFadeOut == eEnd)return TRUE;
	else return FALSE;
}

bool CEffectMng::IsFadeLoopEnd()
{
	if (m_enTrgFadeLoop == eEnd)return TRUE;
	else return FALSE;
}

void CEffectMng::SceneEffectUpdate()
{
	TriFadeIn(g_pDDSBack, 100, 0, 5);

	TriFadeOut(g_pDDSBack, 0, 100, 5);

	TriFadeLoop(g_pDDSBack, 0, 100, 8);
}


bool CEffectMng::TriFadeIn(LPDIRECTDRAWSURFACE7 lpDDSDst,DWORD startRate,DWORD EndRate,DWORD Speed)
{
	LPDIRECTDRAWSURFACE7 tempSurface = NULL;

	if (m_enTrgFadeIn == enTriggerState::eRunning) 
	{
		//Rate setting
		if (m_rateFadeIn.StartRate != startRate) 
		{
			m_rateFadeIn.StartRate = startRate;
			m_rateFadeIn.CurRate = startRate;
		}
		if (m_rateFadeIn.EndRate != EndRate) m_rateFadeIn.EndRate = EndRate;

		if (CreateAndCopySurface(lpDDSDst, &tempSurface, TRUE, FALSE)) 
		{
			//play effect
			//FadeInOutEx(lpDDSDst, tempSurface, m_rateFadeIn.CurRate);
			FadeInOutEx_Self(tempSurface, m_rateFadeIn.CurRate);
			__BltCpy(lpDDSDst, tempSurface);
		}

		//end
		if (m_rateFadeIn.CurRate <= m_rateFadeIn.EndRate) 
		{
			m_enTrgFadeIn = enTriggerState::eEnd;
			m_rateFadeIn.CurRate = startRate;
		}

		//
		m_rateFadeIn.CurRate-=Speed;
		if (m_rateFadeIn.CurRate <= 0)m_rateFadeIn.CurRate = 0;

		SAFE_RELEASE_DDSURFACE(tempSurface);
		
		return TRUE;
	}
	return FALSE;
}

bool CEffectMng::TriFadeOut(LPDIRECTDRAWSURFACE7 lpDDSDst,DWORD startRate, DWORD EndRate, DWORD Speed)
{
	LPDIRECTDRAWSURFACE7 tempSurface = NULL;

	if (m_enTrgFadeOut == enTriggerState::eRunning)
	{
		//Rate setting
		if (m_rateFadeOut.StartRate != startRate)
		{
			m_rateFadeOut.StartRate = startRate;
			m_rateFadeOut.CurRate = startRate;
		}
		if (m_rateFadeOut.EndRate != EndRate) m_rateFadeOut.EndRate = EndRate;

		if (CreateAndCopySurface(lpDDSDst, &tempSurface, TRUE, FALSE))
		{
			//play effect
			//FadeInOutEx(lpDDSDst, tempSurface, m_rateFadeOut.CurRate);
			FadeInOutEx_Self(tempSurface, m_rateFadeOut.CurRate);
			__BltCpy(lpDDSDst, tempSurface);
		}

		//end
		if (m_rateFadeOut.CurRate >= m_rateFadeOut.EndRate)
		{
			m_enTrgFadeOut = enTriggerState::eEnd;
			m_rateFadeOut.CurRate = startRate;
		}

		//
		m_rateFadeOut.CurRate += Speed;

		SAFE_RELEASE_DDSURFACE(tempSurface);

		return TRUE;
	}
	return FALSE;
}

bool CEffectMng::TriFadeLoop(LPDIRECTDRAWSURFACE7 lpDDSDst,DWORD startRate, DWORD EndRate, DWORD Speed) 
{
	LPDIRECTDRAWSURFACE7 tempSurface = NULL;

	if (m_enTrgFadeLoop == enTriggerState::eRunning)
	{
		//Rate setting
		if (m_rateFadeOut.StartRate != startRate)
		{
			m_rateFadeOut.StartRate = startRate;
			m_rateFadeOut.CurRate = startRate;
		}
		if (m_rateFadeOut.EndRate != startRate) m_rateFadeOut.EndRate = EndRate;
		if (m_rateFadeOut.StartRate != startRate) m_rateFadeOut.StartRate = startRate;

		if (CreateAndCopySurface(lpDDSDst, &tempSurface, TRUE, FALSE))
		{
			//play effect
			FadeInOutEx(lpDDSDst, tempSurface, m_rateFadeOut.CurRate);
		}

		//end
		if (m_rateFadeOut.CurRate >= m_rateFadeOut.EndRate)
		{
			m_enTrgFadeLoop = enTriggerState::eEnd;
			m_rateFadeOut.CurRate = startRate;
		}

		//
		m_rateFadeOut.CurRate++;

		SAFE_RELEASE_DDSURFACE(tempSurface);
	}
	return FALSE;
}

void CEffectMng::FadeInOutEx(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc, DWORD dwRate)
{
	//surface desc
	DDSURFACEDESC2 ddsdEffectSrc, ddsdEffectDst;

	//DirectDraw function returned result
	HRESULT ddrval;

	//Init Effect Source Surface Desc
	ZeroMemory(&ddsdEffectSrc, sizeof(ddsdEffectSrc));
	ddsdEffectSrc.dwSize = sizeof(ddsdEffectSrc);

	//Lock Source Surface
	ddrval = lpDDSSrc->Lock(NULL, &ddsdEffectSrc, DDLOCK_READONLY | DDLOCK_WAIT, NULL);
	if (ddrval != DD_OK) { DDERRCHK(ddrval); return; }


	//Init Effect Destination Surface Desc
	ZeroMemory(&ddsdEffectDst, sizeof(ddsdEffectDst));
	ddsdEffectDst.dwSize = sizeof(ddsdEffectDst);

	//lock Destination Surface
	ddrval = lpDDSDst->Lock(NULL, &ddsdEffectDst, DDLOCK_WRITEONLY | DDLOCK_WAIT, NULL);

	if (ddrval != DD_OK)
	{
		//// 소스표면은 Lock 상태이므로 Unlock
		lpDDSSrc->Unlock(NULL);
		DDERRCHK(ddrval);
		return;
	}

	switch (g_enPixelFormat)
	{
		case eRGB565:
		{
			//Get Destination Surface Address
			LPBYTE lpDDS_dst = (LPBYTE)ddsdEffectDst.lpSurface;

			//Get Source Surface Address
			LPBYTE lpDDS_src = (LPBYTE)ddsdEffectSrc.lpSurface;

			//Traversal Pixel 
			for (int x = 0; x < ddsdEffectDst.dwWidth; x++)
			{
				for (int y = 0; y < ddsdEffectDst.dwHeight; y++)
				{
					//Get the pixel address
					WORD* pPixel = (WORD*)(lpDDS_src + y*ddsdEffectSrc.lPitch + (x * 2));

					//Get destination address
					WORD* lpDst = (WORD*)(lpDDS_dst + y*ddsdEffectDst.lPitch + (x * 2));

					//Result Color 
					WORD result = 0;

					//Decompose RGB565
					WORD R = (((*pPixel) >> 11) & 31);
					WORD G = (((*pPixel) >> 5) & 63);
					WORD B = ((*pPixel) & 31);

					//Consult Look Up Table
					R = m_dwFADELUT_16[R][dwRate];
					G = m_dwFADELUT_16[G][dwRate];
					B = m_dwFADELUT_16[B][dwRate];

					//compose RGB
					result = ((R << 11) | (G << 5) | B);

					//put pixel to destination surface
					*lpDst = result;
				}
			}

		}
			break;
		case eRGB555:
		{
			//Get Destination Surface Address
			LPBYTE lpDDS_dst = (LPBYTE)ddsdEffectDst.lpSurface;

			//Get Source Surface Address
			LPBYTE lpDDS_src = (LPBYTE)ddsdEffectSrc.lpSurface;

			//Traversal Pixel 
			for (int x = 0; x < ddsdEffectDst.dwWidth; x++)
			{
				for (int y = 0; y < ddsdEffectDst.dwHeight; y++)
				{
					//Get the pixel address
					WORD* pPixel = (WORD*)(lpDDS_src + y*ddsdEffectSrc.lPitch + (x * 2));

					//Get destination address
					WORD* lpDst = (WORD*)(lpDDS_dst + y*ddsdEffectDst.lPitch + (x * 2));

					//Result Color 
					WORD result = 0;

					//Decompose RGB565
					WORD R = (((*pPixel) >> 10) & 31);
					WORD G = (((*pPixel) >> 5) & 31);
					WORD B = ((*pPixel) & 31);

					//Consult Look Up Table
					R = m_dwFADELUT_16[R][dwRate];
					G = m_dwFADELUT_16[G][dwRate];
					B = m_dwFADELUT_16[B][dwRate];

					//compose RGB
					result = ((R << 10) | (G << 5) | B);

					//put pixel to destination surface
					*lpDst = result;
				}
			}
		}
			break;
		case eRGB888: 
		{
			//Get Source Surface Address
			LPBYTE lpDDS_dst = (LPBYTE)ddsdEffectDst.lpSurface;

			//Get Destination Surface Address
			LPBYTE lpDDS_src = (LPBYTE)ddsdEffectSrc.lpSurface;

			//Traversal Pixel 
			for (int x = 0; x < ddsdEffectDst.dwWidth; x++)
			{
				for (int y = 0; y < ddsdEffectDst.dwHeight; y++)
				{
					//Get destination address
					DWORD* pPixel = (DWORD*)(lpDDS_src + y*ddsdEffectSrc.lPitch + (x * 4));

					//Get the pixel address
					DWORD* lpDst = (DWORD*)(lpDDS_dst + y*ddsdEffectDst.lPitch + (x * 4));


					//result color 
					DWORD result;

					//decompose pixel
					BYTE A = (*pPixel) >> 24;
					BYTE R = (*pPixel) >> 16;
					BYTE G = (*pPixel) >> 8;
					BYTE B = (*pPixel);

					//calculate RGB
					R = m_dwFADELUT_32[R][dwRate];
					G = m_dwFADELUT_32[G][dwRate];
					B = m_dwFADELUT_32[B][dwRate];

					//compose RGB
					result = ((A << 24) | (R << 16) | (G << 8) | B);

					//put pixel to destination surface
					*lpDst = result;
				}
			}
		}
			break;
		case eBGR888:
		{
			//Get Destination Surface Address
			LPBYTE lpDDS_dst = (LPBYTE)ddsdEffectDst.lpSurface;

			//Get Source Surface Address
			LPBYTE lpDDS_src = (LPBYTE)ddsdEffectSrc.lpSurface;

			//Traversal Pixel 
			for (int x = 0; x < ddsdEffectDst.dwWidth; x++)
			{
				for (int y = 0; y < ddsdEffectDst.dwHeight; y++)
				{
					//Get the pixel address
					DWORD* pPixel = (DWORD*)(lpDDS_src + y*ddsdEffectSrc.lPitch + (x * 4));

					//Get destination address
					DWORD* lpDst = (DWORD*)(lpDDS_dst + y*ddsdEffectDst.lPitch + (x * 4));

					//result color 
					DWORD result;

					//decompose pixel
					BYTE A = (*pPixel) >> 24;
					BYTE R = (*pPixel);
					BYTE G = (*pPixel) >> 8;
					BYTE B = (*pPixel) >> 16;

					//calculate RGB
					R = m_dwFADELUT_32[R][dwRate];
					G = m_dwFADELUT_32[G][dwRate];
					B = m_dwFADELUT_32[B][dwRate];

					//compose RGB
					result = ((A << 24) | (B << 16) | (G << 8) | R);

					//put pixel to destination surface
					*lpDst = result;
				}
			}
		}
			break;
		case eRGBOther:
			break;
		default:
			break;
	}

	lpDDSDst->Unlock(NULL);
	lpDDSSrc->Unlock(NULL);

	return;
}

void CEffectMng::FadeInOutEx_Self(LPDIRECTDRAWSURFACE7 lpDDSSrc, DWORD dwRate) 
{
	//surface desc
	DDSURFACEDESC2 ddsdEffectSrc;

	//DirectDraw function returned result
	HRESULT ddrval;

	//Init Effect Source Surface Desc
	ZeroMemory(&ddsdEffectSrc, sizeof(ddsdEffectSrc));
	ddsdEffectSrc.dwSize = sizeof(ddsdEffectSrc);

	//Lock Source Surface
	ddrval = lpDDSSrc->Lock(NULL, &ddsdEffectSrc,  DDLOCK_WAIT, NULL);
	if (ddrval != DD_OK) { DDERRCHK(ddrval); return; }


	switch (g_enPixelFormat)
	{
		case eRGB565:
		{
			//Get Surface Address
			LPBYTE lpDDS_src = (LPBYTE)ddsdEffectSrc.lpSurface;

			//Traversal Pixel 
			for (int x = 0; x < ddsdEffectSrc.dwWidth; x++)
			{
				for (int y = 0; y < ddsdEffectSrc.dwHeight; y++)
				{
					//Get the pixel address
					WORD* pPixel = (WORD*)(lpDDS_src + y*ddsdEffectSrc.lPitch + (x * 2));

					//Result Color 
					WORD result = 0;

					//Decompose RGB565
					WORD R = (((*pPixel) >> 11) & 31);
					WORD G = (((*pPixel) >> 5) & 63);
					WORD B = ((*pPixel) & 31);

					//Consult Look Up Table
					R = m_dwFADELUT_16[R][dwRate];
					G = m_dwFADELUT_16[G][dwRate];
					B = m_dwFADELUT_16[B][dwRate];

					//compose RGB
					result = ((R << 11) | (G << 5) | B);

					//put pixel to destination surface
					*pPixel = result;
				}
			}

		}
			break;
		case eRGB555:
		{
			//Get Source Surface Address
			LPBYTE lpDDS_src = (LPBYTE)ddsdEffectSrc.lpSurface;

			//Traversal Pixel 
			for (int x = 0; x < ddsdEffectSrc.dwWidth; x++)
			{
				for (int y = 0; y < ddsdEffectSrc.dwHeight; y++)
				{
					//Get the pixel address
					WORD* pPixel = (WORD*)(lpDDS_src + y*ddsdEffectSrc.lPitch + (x * 2));

					//Result Color 
					WORD result = 0;

					//Decompose RGB565
					WORD R = (((*pPixel) >> 10) & 31);
					WORD G = (((*pPixel) >> 5) & 31);
					WORD B = ((*pPixel) & 31);

					//Consult Look Up Table
					R = m_dwFADELUT_16[R][dwRate];
					G = m_dwFADELUT_16[G][dwRate];
					B = m_dwFADELUT_16[B][dwRate];

					//compose RGB
					result = ((R << 10) | (G << 5) | B);

					//put pixel to destination surface
					*pPixel = result;
				}
			}
		}
			break;
		case eRGB888: 
		{
			//Get Source Surface Address
			LPBYTE lpDDS_src = (LPBYTE)ddsdEffectSrc.lpSurface;

			//Traversal Pixel 
			for (int x = 0; x < ddsdEffectSrc.dwWidth; x++)
			{
				for (int y = 0; y < ddsdEffectSrc.dwHeight; y++)
				{
					//Get the pixel address
					DWORD* pPixel = (DWORD*)(lpDDS_src + y*ddsdEffectSrc.lPitch + (x * 4));

					//result color 
					DWORD result;

					//decompose pixel
					BYTE A = (*pPixel) >> 24;
					BYTE R = (*pPixel) >> 16;
					BYTE G = (*pPixel) >> 8;
					BYTE B = (*pPixel);

					//calculate RGB
					R = m_dwFADELUT_32[R][dwRate];
					G = m_dwFADELUT_32[G][dwRate];
					B = m_dwFADELUT_32[B][dwRate];

					//compose RGB
					result = ((A << 24) | (R << 16) | (G << 8) | B);

					//put pixel to destination surface
					*pPixel = result;
				}
			}
		}
			break;
		case eBGR888:
		{
			//Get Source Surface Address
			LPBYTE lpDDS_src = (LPBYTE)ddsdEffectSrc.lpSurface;

			//Traversal Pixel 
			for (int x = 0; x < ddsdEffectSrc.dwWidth; x++)
			{
				for (int y = 0; y < ddsdEffectSrc.dwHeight; y++)
				{
					//Get the pixel address
					DWORD* pPixel = (DWORD*)(lpDDS_src + y*ddsdEffectSrc.lPitch + (x * 4));

					//result color 
					DWORD result;

					//decompose pixel
					BYTE A = (*pPixel) >> 24;
					BYTE R = (*pPixel);
					BYTE G = (*pPixel) >> 8;
					BYTE B = (*pPixel) >> 16;

					//calculate RGB
					R = m_dwFADELUT_32[R][dwRate];
					G = m_dwFADELUT_32[G][dwRate];
					B = m_dwFADELUT_32[B][dwRate];

					//compose RGB
					result = ((A << 24) | (B << 16) | (G << 8) | R);

					//put pixel to destination surface
					*pPixel = result;
				}
			}
		}
			break;
		case eRGBOther:
			break;
		default:
			break;
	}
	lpDDSSrc->Unlock(NULL);

	return;


}