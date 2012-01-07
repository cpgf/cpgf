#ifndef __GMETADATA_CONTAINER_COMMON_H
#define __GMETADATA_CONTAINER_COMMON_H

#include "cpgf/metadata/stl/gmetadata_iterator.h"
#include "cpgf/gfunctionextractor.h"

#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/stl/private/gmetadata_stl_header.h"

namespace cpgf {

namespace metadata_internal {

template <typename T, typename MetaDefine>
void buildMetaData_CommonContainer(MetaDefine define, bool scriptable, const GMetaDataNameReplacer * replacer)
{
	(void)scriptable;

	define
		.CPGF_MD_STL_TEMPLATE _constructor<void * ()>()
		.CPGF_MD_STL_TEMPLATE _constructor<void * (const T &)>()
		.CPGF_MD_STL_TEMPLATE _method(replaceName("begin", replacer), (typename T::iterator (T::*)()) &T::begin)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("begin", replacer), (typename T::const_iterator (T::*)() const) &T::begin)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("clear", replacer), &T::clear)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("empty", replacer), &T::empty)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("end", replacer), (typename T::iterator (T::*)()) &T::end)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("end", replacer), (typename T::const_iterator (T::*)() const) &T::end)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("erase", replacer), extractFunction1(&T::erase))
		.CPGF_MD_STL_TEMPLATE _method(replaceName("erase", replacer), extractFunction2(&T::erase))
		.CPGF_MD_STL_TEMPLATE _method(replaceName("rbegin", replacer), (typename T::reverse_iterator (T::*)()) &T::rbegin)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("rbegin", replacer), (typename T::const_reverse_iterator (T::*)() const) &T::rbegin)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("rend", replacer), (typename T::reverse_iterator (T::*)()) &T::rend)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("rend", replacer), (typename T::const_reverse_iterator (T::*)() const) &T::rend)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("size", replacer), &T::size)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("swap", replacer), (void (T::*)(T &)) &T::swap)
	;
}

template <typename T, typename MetaDefine>
void buildMetaData_CommonIterators(MetaDefine define, bool scriptable, const GMetaDataNameReplacer * replacer)
{
	define
		._class(
			buildMetaData_iterator(
				GDefineMetaClass<typename T::iterator>::declare(replaceName("iterator", replacer)), scriptable
			)
		)
		._class(
			buildMetaData_iterator(
				GDefineMetaClass<typename T::const_iterator>::declare(replaceName("const_iterator", replacer)), scriptable
			)
		)
		._class(
			buildMetaData_iterator(
				GDefineMetaClass<typename T::reverse_iterator>::declare(replaceName("reverse_iterator", replacer)), scriptable
			)
		)
		._class(
			buildMetaData_iterator(
				GDefineMetaClass<typename T::const_reverse_iterator>::declare(replaceName("const_reverse_iterator", replacer)), scriptable
			)
		)
	;
}


} // namespace metadata_internal


} // namespace cpgf


#include "cpgf/metadata/stl/private/gmetadata_stl_footer.h"


#endif


