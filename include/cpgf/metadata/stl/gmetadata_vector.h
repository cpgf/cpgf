#ifndef __GMETADATA_VECTOR_H
#define __GMETADATA_VECTOR_H

#include "cpgf/metadata/stl/gmetadata_iterator.h"
#include "cpgf/metadata/stl/private/gmetadata_container_common.h"

#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/stl/private/gmetadata_stl_header.h"

namespace cpgf {

namespace metadata_internal {

template <typename T, typename MetaDefine>
void doBindMetaData_vector(MetaDefine define)
{
	GDefineMetaClass<typename T::iterator> iteratorDefine = GDefineMetaClass<typename T::iterator>::declare("iterator");
	bindMetaData_iterator(iteratorDefine);
	
	metadata_internal::bindMetaData_CommonContainer<T>(define);

	define
		.TEMPLATE _method("assign", (void (T::*)(typename T::size_type, const typename T::value_type &)) &T::assign)
		.TEMPLATE _method("at", (typename T::reference (T::*)(typename T::size_type)) &T::at)
		.TEMPLATE _method("at", (typename T::const_reference (T::*)(typename T::size_type) const) &T::at)
		.TEMPLATE _method("back", (typename T::reference (T::*)()) &T::back)
		.TEMPLATE _method("back", (typename T::const_reference (T::*)() const) &T::back)
		.TEMPLATE _method("capacity", &T::capacity)
		.TEMPLATE _method("front", (typename T::reference (T::*)()) &T::front)
		.TEMPLATE _method("front", (typename T::const_reference (T::*)() const) &T::front)
		.TEMPLATE _method("max_size", (typename T::size_type (T::*)() const) &T::max_size)
		.TEMPLATE _method("pop_back", (void (T::*)()) &T::pop_back)
		.TEMPLATE _method("push_back", (void (T::*)(const typename T::value_type &)) &T::push_back)
		.TEMPLATE _method("reserve", (void (T::*)(typename T::size_type)) &T::reserve)
//		.TEMPLATE _method("resize", (void (T::*) (typename T::size_type, const typename T::value_type &)) &T::resize)
		.TEMPLATE _method("resize", (void (T::*) (typename T::size_type)) &T::resize)

#if QUIRK_CONST_ITERATOR()
		.TEMPLATE _method("insert", (typename T::iterator (T::*)(typename T::const_iterator, typename const T::value_type &)) &T::insert)
		.TEMPLATE _method("insert", (void (T::*)(typename T::const_iterator, typename T::size_type, typename const T::value_type &)) &T::insert)
#else
		.TEMPLATE _method("insert", (typename T::iterator (T::*)(typename T::iterator, const typename T::value_type &)) &T::insert)
		.TEMPLATE _method("insert", (void (T::*)(typename T::iterator, typename T::size_type, const typename T::value_type &)) &T::insert)
#endif
	;
}

} // namespace metadata_internal


template <typename MetaDefine>
void bindMetaData_vector(MetaDefine define)
{
	metadata_internal::doBindMetaData_vector<typename MetaDefine::ClassType>(define);
}


} // namespace cpgf


#include "cpgf/metadata/stl/private/gmetadata_stl_footer.h"


#endif


