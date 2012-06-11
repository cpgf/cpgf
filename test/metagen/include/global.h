#ifndef __GLOBAL_H
#define __GLOBAL_H

#include <string>

enum GlobalEnum {
	globalEnumA = 5, globalEnumB = 8
};

const char * hello(const char * s);
std::string getStdString(int index);

extern int globalInt;
extern const char * globalString;
extern std::string globalStdString;

#endif

