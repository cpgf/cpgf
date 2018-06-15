#include "test_misc_common.h"
#include "cpgf/gstringutil.h"

#include <iostream>
#include <string>

using namespace std;
using namespace cpgf;

namespace Test_StringUtil { namespace {

GTEST(Test_wideStringToString)
{
	wstring ws = L"abcdef";
	string s = wideStringToString(ws);
	GCHECK(s == "abcdef");
}

GTEST(Test_cStyleWideStringToString)
{
	const wchar_t * ws = L"abcdef";
	string s = wideStringToString(ws);
	GCHECK(s == "abcdef");
}

GTEST(Test_stringToWideString)
{
	string s = "abcdef";
	wstring ws = stringToWideString(s);
	GCHECK(ws == L"abcdef");
}

GTEST(Test_cStyleStringToWideString)
{
	const char * s = "abcdef";
	wstring ws = stringToWideString(s);
	GCHECK(ws == L"abcdef");
}


} }
