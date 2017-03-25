#include "test_callback_common.h"

#include <functional>
#include <string>

using namespace cpgf;

namespace {

GTEST(Callback_Functor_STL_Function)
{
	{
		std::function<int ()> func = []() { return 38; };
		GCallback<int ()> cb(func);
		GCHECK(cb() == 38);
	}

	{
		std::function<int (const int, const int)> func = [](const int a, const int b) { return a * b; };
		GCallback<int (int, float)> cb(func);
		GCHECK(cb(5, 6) == 30);
	}

	{
		std::function<std::string ()> func = []() { return "def"; };
		GCallback<std::string ()> cb(func);
		GCHECK(cb() == "def");
	}

	{
		GCallback<int ()> cb = []() { return 38; };
		std::function<int ()> func(cb);
		GCHECK(func() == 38);
	}

	{
		GCallback<int (int, float)> cb = [](const int a, const int b) { return a * b; };
		std::function<int (const int, const int)> func(cb);
		GCHECK(cb(5, 6) == 30);
	}

	{
		GCallback<std::string ()> cb = []() { return "def"; };
		std::function<std::string ()> func = cb;
		GCHECK(cb() == "def");
	}

	{
		GCallback<int (const GCallback<int ()> &)> cb = [](const GCallback<int ()> & cb) { return cb(); };
		GCallback<int ()> p = []()->int { return 38; };
		GCHECK(cb(p) == 38);
	}

};


} //unnamed namespace
