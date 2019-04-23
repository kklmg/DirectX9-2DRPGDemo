#include"Text.h"


void __CreateFont(LPCTSTR FontName, int nWidth, int nWeight)
{
	gFont = CreateFont(nWidth, 0,
		0, 0,
		nWeight,
		FALSE,
		FALSE,
		FALSE,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		NONANTIALIASED_QUALITY,
		VARIABLE_PITCH,
		FontName);
}

void __DestroyFont()
{
	if (gFont)
	{
		DeleteObject(gFont);
		gFont = NULL;
	}
}

void __PutText(int x, int y, COLORREF Color, LPCTSTR pString)
{
	SetBkMode(gHdc, TRANSPARENT);
	HFONT* pOldFont = (HFONT*)SelectObject(gHdc, gFont);
	SetTextColor(gHdc, Color);
	TextOut(gHdc, x, y, pString, strlen(pString));
	SelectObject(gHdc, pOldFont);
}

void __PutTextf(int x, int y, COLORREF Color, char *format, ...)
{
	va_list argptr;
	char    buf[512];

	va_start(argptr, format);
	vsprintf(buf, format, argptr);
	va_end(argptr);

	SetBkMode(gHdc, TRANSPARENT);
	HFONT* pOldFont = (HFONT*)SelectObject(gHdc, gFont);

	///// �׸���
	SetTextColor(gHdc, RGB(0, 0, 0));
	TextOut(gHdc, x + 1, y + 1, buf, strlen(buf));

	///// ��Ʈ
	SetTextColor(gHdc, Color);
	TextOut(gHdc, x, y, buf, strlen(buf));
	SelectObject(gHdc, pOldFont);
}