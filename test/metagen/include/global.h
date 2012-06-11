#ifndef __GLOBAL_H
#define __GLOBAL_H

#include <string>

enum GlobalEnum {
	globalEnumA = 5, globalEnumB = 8
};

const char * getCString(int index);
std::string getStdString(const char * s);

const wchar_t * getCWideString(int index);
std::wstring getStdWideString(const wchar_t * s);

extern int globalInt;
extern const char * globalString;
extern std::string globalStdString;

#endif

