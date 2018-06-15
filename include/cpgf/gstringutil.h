#ifndef CPGF_GSTRINGUTIL_H
#define CPGF_GSTRINGUTIL_H

#include <cstddef>
#include <string>

namespace cpgf {

struct GCStringCompare
{
	bool operator () (const char * a, const char * b) const;
};

struct GCStringEqual
{
	bool operator () (const char * a, const char * b) const;
};

struct GCStringHash
{
	size_t operator () (const char * s) const;
};

std::wstring stringToWideString(const std::string & s);
std::wstring stringToWideString(const char * s);
std::string wideStringToString(const std::wstring & ws);
std::string wideStringToString(const wchar_t * ws);


} //namespace cpgf


#endif
