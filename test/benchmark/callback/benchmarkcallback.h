#ifndef CPGF_BENCHMARKCALLBACK_H
#define CPGF_BENCHMARKCALLBACK_H

#include <time.h>

#if defined(_MSC_VER)
	#define NON_INLINE __declspec(noinline)
	#define INT3 __asm int 3
#else
// gcc
	#define NON_INLINE __attribute__((noinline))
	#define INT3 asm("int $0x03")
#endif


void runBenchmarkCallback();

extern int globalVar;

struct ObjectParam
{
	ObjectParam(int n) : n(n) {}
	ObjectParam(const ObjectParam & other) : n(other.n) {}
	ObjectParam & operator = (const ObjectParam & other) {
		this->n = other.n;

		return *this;
	}

	int n;
};

typedef int ParamType;
//typedef ObjectParam ParamType;

inline int getValue(const int & n) {
	return n;
}

inline int getValue(const ObjectParam & n) {
	return n.n;
}

class BenchmarkTestObject {
public:
	NON_INLINE void noninlinedMember1(const ParamType & n) const;
	NON_INLINE void noninlinedMember2(const ParamType & n) const;

	inline void inlinedMember1(const ParamType & n) const {
		globalVar += getValue(n);
	}

	inline void inlinedMember2(const ParamType & n) const {
		globalVar += getValue(n);
	}

	virtual void virtualMember1(const ParamType & /*n*/) const {
	}

	virtual void virtualMember2(const ParamType & /*n*/) const {
	}

	void operator () (const ParamType & n) const {
		globalVar += getValue(n);
	}
};

class BenchmarkTestObject2 {
public:
	void operator () (const ParamType & n) const {
		globalVar += getValue(n);
	}
};

class BenchmarkTestObjectExtend : public BenchmarkTestObject {
public:
	virtual void virtualMember1(const ParamType & /*n*/) const {
	}
};



#endif
