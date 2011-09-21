// dll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include <string>
#include <iostream>

#include "../../../../framework/gcallback.h"

using namespace cpgf;
using namespace std;

typedef GCallback<int, string> CallbackType;

GCallbackList<int, string> cbList;

__declspec(dllexport)
void addCallback(const CallbackType & cb)
{
	cbList.addCallback(cb);

	cout << "Adding callback: result = " << cb("add me") << endl;
}

__declspec(dllexport)
void executeCallbackList()
{
	cbList.dispatch("abc");
}
