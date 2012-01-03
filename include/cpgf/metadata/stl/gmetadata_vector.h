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
		.CPGF_MD_STL_TEMPLATE _method("assign", (void (T::*)(typename T::size_type, const typename T::value_type &)) &T::assign)
		.CPGF_MD_STL_TEMPLATE _method("at", (typename T::reference (T::*)(typename T::size_type)) &T::at)
		.CPGF_MD_STL_TEMPLATE _method("at", (typename T::const_reference (T::*)(typename T::size_type) const) &T::at)
		.CPGF_MD_STL_TEMPLATE _method("back", (typename T::reference (T::*)()) &T::back)
		.CPGF_MD_STL_TEMPLATE _method("back", (typename T::const_reference (T::*)() const) &T::back)
		.CPGF_MD_STL_TEMPLATE _method("capacity", &T::capacity)
		.CPGF_MD_STL_TEMPLATE _method("front", (typename T::reference (T::*)()) &T::front)
		.CPGF_MD_STL_TEMPLATE _method("front", (typename T::const_reference (T::*)() const) &T::front)
		.CPGF_MD_STL_TEMPLATE _method("insert", extractFunction2(&T::insert))
		.CPGF_MD_STL_TEMPLATE _method("insert", extractFunction3(&T::insert))
		.CPGF_MD_STL_TEMPLATE _method("max_size", (typename T::size_type (T::*)() const) &T::max_size)
		.CPGF_MD_STL_TEMPLATE _method("pop_back", (void (T::*)()) &T::pop_back)
		.CPGF_MD_STL_TEMPLATE _method("push_back", (void (T::*)(const typename T::value_type &)) &T::push_back)
		.CPGF_MD_STL_TEMPLATE _method("reserve", (void (T::*)(typename T::size_type)) &T::reserve)
		.CPGF_MD_STL_TEMPLATE _method("resize", extractFunction2(&T::resize))
		.CPGF_MD_STL_TEMPLATE _method("resize", extractFunction1(&T::resize))
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


