#include "cpgf/gstringutil.h"

#include <cstring>

namespace cpgf {

bool GCStringCompare::operator () (const char * a, const char * b) const
{
	return strcmp(a, b) < 0;
}

bool GCStringEqual::operator () (const char * a, const char * b) const
{
	return strcmp(a, b) == 0;
}

// http://isthe.com/chongo/tech/comp/fnv/#FNV-param
template <int size>
struct HashParameter
{
};

template <>
struct HashParameter <8>
{
	// 64 bits
	static constexpr size_t prime = (size_t)1099511628211ULL;
	static constexpr size_t offsetBasis = (size_t)14695981039346656037ULL;
	static constexpr size_t shift = 3;
	static constexpr size_t size = 8;
};

template <>
struct HashParameter <4>
{
	// 32 bits
	static constexpr size_t prime = 16777619U;
	static constexpr size_t offsetBasis = 2166136261U;
	static constexpr size_t shift = 2;
	static constexpr size_t size = 4;
};

size_t GCStringHash::operator () (const char * s) const
{
	const size_t length = strlen(s);
	const size_t count = (length >> HashParameter<sizeof(size_t)>::shift);
	size_t result = HashParameter<sizeof(size_t)>::offsetBasis;
	size_t i = 0;
	for(; i < count * HashParameter<sizeof(size_t)>::size; i += HashParameter<sizeof(size_t)>::size) {
		result ^= *(const size_t *)(s + i);
		result *= HashParameter<sizeof(size_t)>::prime;
	}
	for(; i < length; ++i) {
		result ^= (size_t)s[i];
		result *= HashParameter<sizeof(size_t)>::prime;
	}
	return result;
}


} //namespace cpgf

