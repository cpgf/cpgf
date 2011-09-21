// exe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>
#include <iostream>
#include <conio.h>

#include "../../../../framework/gcallback.h"

using namespace cpgf;
using namespace std;

typedef GCallback<int, std::string> CallbackType;

#pragma comment(lib, "../debug/dll.lib")

void addCallback(const CallbackType & cb);
void executeCallbackList();

int callback1(string s)
{
	cout << s << endl;

	return 3;
}

int _tmain(int argc, _TCHAR* argv[])
{
	addCallback(makeCallback(&callback1));
	executeCallbackList();

	_getch();

	return 0;
}

