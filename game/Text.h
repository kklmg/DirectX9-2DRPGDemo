#pragma once
#include<Windows.h>
#include"Global_data.h"

void __CreateFont(LPCTSTR FontName, int nWidth, int nWeight);

void __DestroyFont();

void __PutText(int x, int y, COLORREF Color, LPCTSTR pString);

void __PutTextf(int x, int y, COLORREF Color, char *format, ...);