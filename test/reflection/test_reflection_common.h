#ifndef __TEST_REFLECTION_COMMON_H
#define __TEST_REFLECTION_COMMON_H


#include "../unittestbase.h"


#include "cpgf/gmetaclass.h"
#include "cpgf/gmetaapiservice.h"
#include "cpgf/gmetaapiutil.h"

#include <string>


#define CLASS_DATA TestData_Reflection

#define EXCEPT_META(...) GBEGIN_EXCEPTION __VA_ARGS__; GEND_EXCEPTION(const GMetaException &)
#define EXCEPT_VARIANT(...) GBEGIN_EXCEPTION __VA_ARGS__; GEND_EXCEPTION(const GVariantException &)


struct CLASS_DATA {
	CLASS_DATA() : s(""), i(0) {
	}
	
	CLASS_DATA(const std::string s, int i) : s(s), i(i) {
	}

	explicit CLASS_DATA(const std::string s) : s(s), i(0) {
	}

	explicit CLASS_DATA(int i) : s(""), i(i) {
	}

	void reset() {
		s = "";
		i = 0;
	}
	
	bool operator == (const CLASS_DATA & other) const /**/ {
		return this->s == other.s && this->i == other.i;
	}
	
	std::string s;
	int i;
};



#endif
