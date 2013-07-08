#ifndef __GMETADATA_CONTAINER_COMMON_H
#define __GMETADATA_CONTAINER_COMMON_H

#include "cpgf/metadata/stl/gmetadata_iterator.h"
#include "cpgf/gselectfunctionbyarity.h"

#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/private/gmetadata_header.h"

namespace cpgf {

namespace metadata_internal {

template <typename T, typename MetaDefine, typename Policy>
void buildMetaData_CommonContainer(const GMetaDataConfigFlags & /*config*/, MetaDefine define, const Policy & /*policy*/)
{
	define
		.CPGF_MD_TEMPLATE _constructor<void * ()>()
		.CPGF_MD_TEMPLATE _constructor<void * (const T &)>()
		.CPGF_MD_TEMPLATE _method("begin", (typename T::iterator (T::*)()) &T::begin)
		.CPGF_MD_TEMPLATE _method("begin", (typename T::const_iterator (T::*)() const) &T::begin)
		.CPGF_MD_TEMPLATE _method("clear", &T::clear) // clear is Dict's default function in Python
		.CPGF_MD_TEMPLATE _method("_clear", &T::clear)
		.CPGF_MD_TEMPLATE _method("empty", &T::empty)
		.CPGF_MD_TEMPLATE _method("end", (typename T::iterator (T::*)()) &T::end)
		.CPGF_MD_TEMPLATE _method("end", (typename T::const_iterator (T::*)() const) &T::end) // end is keyword in Lua
		.CPGF_MD_TEMPLATE _method("_end", (typename T::iterator (T::*)()) &T::end)
		.CPGF_MD_TEMPLATE _method("_end", (typename T::const_iterator (T::*)() const) &T::end)
		.CPGF_MD_TEMPLATE _method("erase", selectFunctionByArity1(&T::erase))
		.CPGF_MD_TEMPLATE _method("erase", selectFunctionByArity2(&T::erase))
		.CPGF_MD_TEMPLATE _method("rbegin", (typename T::reverse_iterator (T::*)()) &T::rbegin)
		.CPGF_MD_TEMPLATE _method("rbegin", (typename T::const_reverse_iterator (T::*)() const) &T::rbegin)
		.CPGF_MD_TEMPLATE _method("rend", (typename T::reverse_iterator (T::*)()) &T::rend)
		.CPGF_MD_TEMPLATE _method("rend", (typename T::const_reverse_iterator (T::*)() const) &T::rend)
		.CPGF_MD_TEMPLATE _method("size", &T::size)
		.CPGF_MD_TEMPLATE _method("swap", (void (T::*)(T &)) &T::swap)
	;
}

template <typename T, typename MetaDefine, typename Policy>
void buildMetaData_CommonIterators(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy)
{
	define
		.CPGF_MD_TEMPLATE _class(
			buildMetaData_iterator(
				config, GDefineMetaClass<typename T::iterator>::declare("iterator"), policy
			)
		)
		.CPGF_MD_TEMPLATE _class(
			buildMetaData_constIterator(
				config, GDefineMetaClass<typename T::const_iterator>::declare("const_iterator"), policy
			)
		)
		.CPGF_MD_TEMPLATE _class(
			buildMetaData_iterator(
				config, GDefineMetaClass<typename T::reverse_iterator>::declare("reverse_iterator"), policy
			)
		)
		.CPGF_MD_TEMPLATE _class(
			buildMetaData_constIterator(
				config, GDefineMetaClass<typename T::const_reverse_iterator>::declare("const_reverse_iterator"), policy
			)
		)
	;
}


} // namespace metadata_internal


} // namespace cpgf


#include "cpgf/metadata/private/gmetadata_footer.h"


#endif


