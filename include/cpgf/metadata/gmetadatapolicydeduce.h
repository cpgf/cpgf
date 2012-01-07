#ifndef __GMETADATAPOLICYDEDUCE_H
#define __GMETADATAPOLICYDEDUCE_H


#include "cpgf/gmetapolicy.h"

#include <string>


namespace cpgf {

template <typename T>
struct GMetaDataPolicyDeduce
{
	typedef GMetaPolicyDefault Result;
};

template <>
struct GMetaDataPolicyDeduce <std::string>
{
	typedef GMetaPolicyCopyAllConstReference Result;
};

template <>
struct GMetaDataPolicyDeduce <const std::string &>
{
	typedef GMetaPolicyCopyAllConstReference Result;
};



} // namespace cpgf



#endif

