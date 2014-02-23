#ifndef CPGF_TEST_REFLECTION_COMMON_H
#define CPGF_TEST_REFLECTION_COMMON_H


#include "../unittestbase.h"
#include "../testmetatraits.h"


#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"

#include "cpgf/gmetaapiutil.h"
#include "cpgf/gexception.h"

#include <string>


#define CLASS_DATA TestData_Reflection
#define NC_DATA TestData_Reflection_NoCopyable

#define EXCEPT_META(...) GBEGIN_EXCEPTION __VA_ARGS__; GEND_EXCEPTION(const GException &)
#define EXCEPT_VARIANT(...) GBEGIN_EXCEPTION __VA_ARGS__; GEND_EXCEPTION(const GException &)


#define NAME2(cls) GPP_STRINGIZE(test_Reflect_ ## cls)
#define NAME(cls) NAME2(cls)

#define FIELD(f) pointerAssign(field, metaClass->getField(# f))
#define METHOD(f) pointerAssign(method, metaClass->getMethod(# f))
#define OPERA(f) pointerAssign(opera, metaClass->getOperator(f))

#define FIELD_HI(f) pointerAssign(field, metaClass->getFieldInHierarchy(# f, &pobj))
#define METHOD_HI(f) pointerAssign(method, metaClass->getMethodInHierarchy(# f, &pobj))
#define OPERA_HI(f) pointerAssign(opera, metaClass->getOperatorInHierarchy(f, &pobj))


class CLASS_DATA
{
public:
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


class NC_DATA
{
public:
	NC_DATA() : s(""), i(0) {
	}
	
	NC_DATA(const std::string s, int i) : s(s), i(i) {
	}

	explicit NC_DATA(const std::string s) : s(s), i(0) {
	}

	explicit NC_DATA(int i) : s(""), i(i) {
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
	
private:
	NC_DATA(const NC_DATA &);
	NC_DATA & operator = (const NC_DATA &);
};


inline const cpgf::GMetaOperator * getOperator(cpgf::GMetaOpType op, const cpgf::GMetaClass * metaClass, int index = 0)
{
	if(index == 0) {
		return metaClass->getOperator(op);
	}
	
	for(unsigned int i = 0; i < metaClass->getOperatorCount(); ++i) {
		const cpgf::GMetaOperator * meta = metaClass->getOperatorAt(i);

		if(meta->getOperator() == op) {
			if(index <= 0) {
				return meta;
			}

			--index;
		}
	}

	return NULL;
}

inline cpgf::IMetaOperator * getOperator(cpgf::GMetaOpType op, const cpgf::GScopedInterface<cpgf::IMetaClass> & metaClass, int index = 0)
{
	if(index == 0) {
		return metaClass->getOperator(op);
	}
	
	for(unsigned int i = 0; i < metaClass->getOperatorCount(); ++i) {
		cpgf::GScopedInterface<cpgf::IMetaOperator> meta(metaClass->getOperatorAt(i));

		if(meta->getOperator() == op) {
			if(index <= 0) {
				return meta.take();
			}

			--index;
		}
	}

	return NULL;
}


#endif
