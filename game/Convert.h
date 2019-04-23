#pragma once
#include<Windows.h>
#include<iostream>
#include<string>

namespace Convert 
{
	//------------------------------------
	//String to Figure
	//------------------------------------
	int StrToInt(LPCSTR str);
	WORD StrToWord(LPCSTR str);
	DWORD StrToDWord(LPCSTR str);

	float StrToFloat(LPCSTR str);
	double StrToDouble(LPCSTR str);

	//------------------------------------
	//Figure to String
	//------------------------------------
	std::string IntToStr(int data);
	std::string WordToStr(WORD data);
	std::string DwordToStr(DWORD data);

	std::string FloatToStr(DWORD data);
	std::string DoubleToStr(DWORD data);
}

