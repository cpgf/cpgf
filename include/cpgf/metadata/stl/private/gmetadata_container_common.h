#ifndef __GMETADATA_CONTAINER_COMMON_H
#define __GMETADATA_CONTAINER_COMMON_H

#include "cpgf/metadata/stl/gmetadata_iterator.h"

#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/stl/private/gmetadata_stl_header.h"

namespace cpgf {

namespace metadata_internal {

template <typename T, typename MetaDefine>
void bindMetaData_CommonContainer(MetaDefine define)
{
	define
		.TEMPLATE _constructor<void * ()>()
		.TEMPLATE _constructor<void * (const T &)>()
		.TEMPLATE _method("begin", (typename T::iterator (T::*)()) &T::begin)
		.TEMPLATE _method("begin", (typename T::const_iterator (T::*)() const) &T::begin)
		.TEMPLATE _method("clear", (void (T::*)()) &T::clear)
		.TEMPLATE _method("empty", (typename T::const_iterator (T::*)() const) &T::empty)
		.TEMPLATE _method("end", (typename T::iterator (T::*)()) &T::end)
		.TEMPLATE _method("end", (typename T::const_iterator (T::*)() const) &T::end)
		.TEMPLATE _method("rbegin", (typename T::reverse_iterator (T::*)()) &T::rbegin)
		.TEMPLATE _method("rbegin", (typename T::const_reverse_iterator (T::*)() const) &T::rbegin)
		.TEMPLATE _method("rend", (typename T::reverse_iterator (T::*)()) &T::rend)
		.TEMPLATE _method("rend", (typename T::const_reverse_iterator (T::*)() const) &T::rend)
		.TEMPLATE _method("size", (typename T::size_type (T::*)() const) &T::size)
		.TEMPLATE _method("swap", (void (T::*)(T &)) &T::swap)

#if QUIRK_CONST_ITERATOR()
		.TEMPLATE _method("erase", (typename T::iterator (T::*)(typename T::const_iterator)) &T::erase)
		.TEMPLATE _method("erase", (typename T::iterator (T::*)(typename T::const_iterator, typename T::const_iterator)) &T::erase)
#else
		.TEMPLATE _method("erase", (typename T::iterator (T::*)(typename T::iterator)) &T::erase)
		.TEMPLATE _method("erase", (typename T::iterator (T::*)(typename T::iterator, typename T::iterator)) &T::erase)
#endif
	;
}

} // namespace metadata_internal


} // namespace cpgf


#include "cpgf/metadata/stl/private/gmetadata_stl_footer.h"


#endif


