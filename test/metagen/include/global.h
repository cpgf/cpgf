#ifndef CPGF_GLOBAL_H
#define CPGF_GLOBAL_H

#include <string>

enum GlobalEnum {
	globalEnumA = 5, globalEnumB = 8
};


extern int globalInt;
extern const char * globalString;
extern std::string globalStdString;
extern std::wstring globalStdWideString;
extern int *** globalRaw;


const char * getCString(int index);
std::string getStdString(const char * s);

const wchar_t * getCWideString(int index);
std::wstring getStdWideString(const wchar_t * s);

bool checkGlobalRaw(const void *** value);


#endif

