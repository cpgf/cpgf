#include "../include/global.h"

#include <string>

using namespace std;

int globalInt = 38;
const char * globalString = "global";
std::string globalStdString = "globalStd";
std::wstring globalStdWideString = L"globalStdWide";
int *** globalRaw = (int ***)0x6abcdef;

const char * getCString(int index)
{
	const char * result;

	if(index == 0) {
		result = "zero";
	}
	else {
		result = "one";
	}

	return result;
}

std::string getStdString(const char * s)
{
	string result;
	result = "a-";
	result = result + s;
	return result;
}

const wchar_t * getCWideString(int index)
{
	const wchar_t * result;

	if(index == 0) {
		result = L"WideZero";
	}
	else {
		result = L"WideOne";
	}

	return result;
}

std::wstring getStdWideString(const wchar_t * s)
{
	wstring result;
	result = L"W-";
	result = result + s;
	return result;
}

bool checkGlobalRaw(const void *** value)
{
	return (void *)value == (void *)globalRaw;
}


