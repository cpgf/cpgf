#include "test_callback_common.h"

using namespace cpgf;

namespace {

int globalValue = 0;

void func_void_void()
{
	globalValue = 38;
}

GTEST(Callback_Global_void_void)
{
	globalValue = 0;
	GCHECK(globalValue == 0);
	GCallback<void ()> cb(&func_void_void);
	cb();
	GCHECK(globalValue == 38);

	globalValue = 0;
	GCHECK(globalValue == 0);
	auto cb2 = cb;
	cb2();
	GCHECK(globalValue == 38);

	globalValue = 0;
	GCHECK(globalValue == 0);
	auto cb3 = makeCallback(&func_void_void);
	cb3();
	GCHECK(globalValue == 38);
};

void func_void_int(int a)
{
	globalValue = 38 + a;
}

GTEST(Callback_Global_void_int)
{
	globalValue = 0;
	GCHECK(globalValue == 0);
	GCallback<void (int)> cb(&func_void_int);
	cb(15);
	GCHECK(globalValue == 38 + 15);

	globalValue = 0;
	GCHECK(globalValue == 0);
	auto cb2 = cb;
	cb2(33);
	GCHECK(globalValue == 38 + 33);

	globalValue = 0;
	GCHECK(globalValue == 0);
	auto cb3 = makeCallback(&func_void_int);
	cb3(-5);
	GCHECK(globalValue == 38 + -5);
}

int func_int_int_string(const int a, const std::string & s)
{
	return a + s.size();
}

GTEST(Callback_Global_int_int_string)
{
	GCallback<int (int, std::string)> cb(&func_int_int_string);
	GCHECK(cb(5, "abc") == 5 + 3);

	GCallback<int (int, const std::string)> cb2(&func_int_int_string);
	GCHECK(cb2(8, "abcde") == 8 + 5);

	GCallback<int (int, const std::string &)> cb3(&func_int_int_string);
	GCHECK(cb3(3, "abcdef") == 3 + 6);

	GCallback<int (int, std::string &&)> cb4(&func_int_int_string);
	GCHECK(cb4(16, "ab") == 16 + 2);
}

int func_many_parameters(
		int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10,
		int a11, int a12, int a13, int a14, int a15, int a16, int a17, int a18, int a19, int a20,
		int a21, int a22, int a23, int a24, int a25, int a26, int a27, int a28, int a29, int a30,
		int a31, int a32, int a33, int a34, int a35, int a36, int a37, int a38, int a39, int a40,
		int a41, int a42, int a43, int a44, int a45, int a46, int a47, int a48, int a49, int a50,
		int a51, int a52, int a53, int a54, int a55, int a56, int a57, int a58, int a59, int a60
	)
{
	return a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9 + a10
		+ a11 + a12 + a13 + a14 + a15 + a16 + a17 + a18 + a19 + a20
		+ a21 + a22 + a23 + a24 + a25 + a26 + a27 + a28 + a29 + a30
		+ a31 + a32 + a33 + a34 + a35 + a36 + a37 + a38 + a39 + a40
		+ a41 + a42 + a43 + a44 + a45 + a46 + a47 + a48 + a49 + a50
		+ a51 + a52 + a53 + a54 + a55 + a56 + a57 + a58 + a59 + a60
	;
}

GTEST(Callback_Global_Many_Parameters)
{
	auto cb = makeCallback(&func_many_parameters);
	const int result = cb(
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
		31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
		51, 52, 53, 54, 55, 56, 57, 58, 59, 60
	);
	GCHECK(result == 1830);
}

std::string func_rvalue_reference(std::string && s)
{
	return std::move(s);
}

GTEST(Callback_Global_rvalue_reference)
{
	auto cb = makeCallback(&func_rvalue_reference);
	std::string s("abc");
	
	GCHECK(s == "abc");
	
	std::string ts = cb(std::move(s));
	GCHECK(s == "");
	GCHECK(ts == "abc");
}


} //unnamed namespace
