#include "../include/global.h"

#include <string>

using namespace std;

int globalInt = 38;
const char * globalString = "global";
std::string globalStdString = "globalStd";

const char * hello(const char * s)
{
	static string result;
	result = "123";
	result = result + s;
	return result.c_str();
}

std::string getStdString(int index)
{
	string result;

	if(index == 0) {
		result = "zero";
	}
	else {
		result = "one";
	}

	return result;
}
