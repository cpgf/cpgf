#ifndef __GMETADATA_CONTAINER_COMMON_H
#define __GMETADATA_CONTAINER_COMMON_H

#include "cpgf/metadata/stl/gmetadata_iterator.h"
#include "cpgf/gfunctionextractor.h"

#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/stl/private/gmetadata_stl_header.h"

namespace cpgf {

namespace metadata_internal {

template <typename T, typename MetaDefine>
void bindMetaData_CommonContainer(MetaDefine define, bool scriptable)
{
	(void)scriptable;

	define
		.CPGF_MD_STL_TEMPLATE _constructor<void * ()>()
		.CPGF_MD_STL_TEMPLATE _constructor<void * (const T &)>()
		.CPGF_MD_STL_TEMPLATE _method("begin", (typename T::iterator (T::*)()) &T::begin)
		.CPGF_MD_STL_TEMPLATE _method("begin", (typename T::const_iterator (T::*)() const) &T::begin)
		.CPGF_MD_STL_TEMPLATE _method("clear", (void (T::*)()) &T::clear)
		.CPGF_MD_STL_TEMPLATE _method("empty", (typename T::const_iterator (T::*)() const) &T::empty)
		.CPGF_MD_STL_TEMPLATE _method("_end", (typename T::iterator (T::*)()) &T::end)
		.CPGF_MD_STL_TEMPLATE _method("_end", (typename T::const_iterator (T::*)() const) &T::end)
		.CPGF_MD_STL_TEMPLATE _method("erase", extractFunction1(&T::erase))
		.CPGF_MD_STL_TEMPLATE _method("erase", extractFunction2(&T::erase))
		.CPGF_MD_STL_TEMPLATE _method("rbegin", (typename T::reverse_iterator (T::*)()) &T::rbegin)
		.CPGF_MD_STL_TEMPLATE _method("rbegin", (typename T::const_reverse_iterator (T::*)() const) &T::rbegin)
		.CPGF_MD_STL_TEMPLATE _method("rend", (typename T::reverse_iterator (T::*)()) &T::rend)
		.CPGF_MD_STL_TEMPLATE _method("rend", (typename T::const_reverse_iterator (T::*)() const) &T::rend)
		.CPGF_MD_STL_TEMPLATE _method("size", (typename T::size_type (T::*)() const) &T::size)
		.CPGF_MD_STL_TEMPLATE _method("swap", (void (T::*)(T &)) &T::swap)
	;
}

template <typename T, typename MetaDefine>
void bindMetaData_CommonIterators(MetaDefine define, bool scriptable)
{
	define
		._class(
			bindMetaData_iterator(
				GDefineMetaClass<typename T::iterator>::declare("iterator"), scriptable
			)
		)
		._class(
			bindMetaData_iterator(
				GDefineMetaClass<typename T::const_iterator>::declare("const_iterator"), scriptable
			)
		)
		._class(
			bindMetaData_iterator(
				GDefineMetaClass<typename T::reverse_iterator>::declare("reverse_iterator"), scriptable
			)
		)
		._class(
			bindMetaData_iterator(
				GDefineMetaClass<typename T::const_reverse_iterator>::declare("const_reverse_iterator"), scriptable
			)
		)
	;
}


} // namespace metadata_internal


} // namespace cpgf


#include "cpgf/metadata/stl/private/gmetadata_stl_footer.h"


#endif


