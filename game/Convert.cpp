#include"Convert.h"


//------------------------------------
//String to Figure
//------------------------------------
int Convert::StrToInt(LPCSTR str)
{
	int result;
	sscanf(str, "%d", &result);
	return result;
}
WORD Convert::StrToWord(LPCSTR str)
{
	WORD result;
	sscanf(str, "%hu", &result);
	return result;
}
DWORD Convert::StrToDWord(LPCSTR str)
{
	DWORD result;
	sscanf(str, "%u", &result);
	return result;
}


float Convert::StrToFloat(LPCSTR str)
{
	float result;
	sscanf(str, "%f", &result);
	return result;
}
double Convert::StrToDouble(LPCSTR str)
{
	double result;
	sscanf(str, "%lf", &result);
	return result;
}

//------------------------------------
//Figure to String
//------------------------------------
std::string Convert::IntToStr(int data)
{
	char str[255];
	sprintf(str, "%d", data);
	return str;
}
std::string Convert::WordToStr(WORD data)
{
	char str[255];
	sprintf(str, "%hu\0", data);

	return str;
}
std::string Convert::DwordToStr(DWORD data)
{
	char str[255];
	sprintf(str, "%u", data);
	return str;
}

std::string Convert::FloatToStr(DWORD data)
{
	char str[255];
	sprintf(str, "%f", data);
	return str;
}
std::string Convert::DoubleToStr(DWORD data)
{
	char str[255];
	sprintf(str, "%lf", data);
	return str;
}


