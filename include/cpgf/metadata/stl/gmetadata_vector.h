#ifndef __GMETADATA_VECTOR_H
#define __GMETADATA_VECTOR_H

#include "cpgf/metadata/stl/gmetadata_iterator.h"
#include "cpgf/metadata/stl/private/gmetadata_container_common.h"

#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/stl/private/gmetadata_stl_header.h"

namespace cpgf {

namespace metadata_internal {

template <typename T, typename MetaDefine>
void doBindMetaData_vector(MetaDefine define, bool scriptable)
{
	metadata_internal::bindMetaData_CommonContainer<T>(define, scriptable);
	metadata_internal::bindMetaData_CommonIterators<T>(define, scriptable);

	define
		.CPGF_MD_STL_TEMPLATE _method("assign", (void (T::*)(typename T::size_type, const typename T::value_type &)) &T::assign)
		.CPGF_MD_STL_TEMPLATE _method("at", (typename T::reference (T::*)(typename T::size_type)) &T::at)
		.CPGF_MD_STL_TEMPLATE _method("at", (typename T::const_reference (T::*)(typename T::size_type) const) &T::at)
		.CPGF_MD_STL_TEMPLATE _method("back", (typename T::reference (T::*)()) &T::back)
		.CPGF_MD_STL_TEMPLATE _method("back", (typename T::const_reference (T::*)() const) &T::back)
		.CPGF_MD_STL_TEMPLATE _method("capacity", &T::capacity)
		.CPGF_MD_STL_TEMPLATE _method("front", (typename T::reference (T::*)()) &T::front)
		.CPGF_MD_STL_TEMPLATE _method("front", (typename T::const_reference (T::*)() const) &T::front)
		.CPGF_MD_STL_TEMPLATE _method("max_size", (typename T::size_type (T::*)() const) &T::max_size)
		.CPGF_MD_STL_TEMPLATE _method("pop_back", (void (T::*)()) &T::pop_back)
		.CPGF_MD_STL_TEMPLATE _method("push_back", (void (T::*)(const typename T::value_type &)) &T::push_back)
		.CPGF_MD_STL_TEMPLATE _method("reserve", (void (T::*)(typename T::size_type)) &T::reserve)
		.CPGF_MD_STL_TEMPLATE _method("resize", extractFunction2(&T::resize))
		.CPGF_MD_STL_TEMPLATE _method("resize", extractFunction1(&T::resize))

#if CPGF_MD_STL_QUIRK_CONST_ITERATOR()		
		.CPGF_MD_STL_TEMPLATE _method("insert", (typename T::iterator (T::*)(typename T::const_iterator, const typename T::value_type &)) &T::insert)
		.CPGF_MD_STL_TEMPLATE _method("insert", (void (T::*)(typename T::const_iterator, typename T::size_type, const typename T::value_type &)) &T::insert)
#else
		.CPGF_MD_STL_TEMPLATE _method("insert", (typename T::iterator (T::*)(typename T::iterator, const typename T::value_type &)) &T::insert)
		.CPGF_MD_STL_TEMPLATE _method("insert", (void (T::*)(typename T::iterator, typename T::size_type, const typename T::value_type &)) &T::insert)
#endif
	;
}

} // namespace metadata_internal


template <typename MetaDefine>
void bindMetaData_vector(MetaDefine define, bool scriptable)
{
	metadata_internal::doBindMetaData_vector<typename MetaDefine::ClassType>(define, scriptable);
}

template <typename MetaDefine>
void bindMetaData_vector(MetaDefine define)
{
	bindMetaData_vector(define, false);
}


} // namespace cpgf


#include "cpgf/metadata/stl/private/gmetadata_stl_footer.h"


#endif


