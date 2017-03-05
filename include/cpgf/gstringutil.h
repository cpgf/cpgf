#ifndef CPGF_GSTRINGUTIL_H
#define CPGF_GSTRINGUTIL_H

#include <cstddef>

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


} //namespace cpgf


#endif
