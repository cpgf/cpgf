#ifndef __GMETADATA_VECTOR_H
#define __GMETADATA_VECTOR_H

#include "cpgf/metadata/stl/gmetadata_iterator.h"
#include "cpgf/metadata/stl/private/gmetadata_container_common.h"

#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/stl/private/gmetadata_stl_header.h"

namespace cpgf {

namespace metadata_internal {

template <typename T, typename MetaDefine>
void doBindMetaData_vector(MetaDefine define, bool scriptable, const GMetaDataNameReplacer * replacer)
{
	metadata_internal::bindMetaData_CommonContainer<T>(define, scriptable, replacer);
	metadata_internal::bindMetaData_CommonIterators<T>(define, scriptable, replacer);

	define
		.CPGF_MD_STL_TEMPLATE _method(replaceName("assign", replacer), (void (T::*)(typename T::size_type, const typename T::value_type &)) &T::assign)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("at", replacer), (typename T::reference (T::*)(typename T::size_type)) &T::at)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("at", replacer), (typename T::const_reference (T::*)(typename T::size_type) const) &T::at)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("back", replacer), (typename T::reference (T::*)()) &T::back)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("back", replacer), (typename T::const_reference (T::*)() const) &T::back)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("capacity", replacer), &T::capacity)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("front", replacer), (typename T::reference (T::*)()) &T::front)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("front", replacer), (typename T::const_reference (T::*)() const) &T::front)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("max_size", replacer), (typename T::size_type (T::*)() const) &T::max_size)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("pop_back", replacer), (void (T::*)()) &T::pop_back)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("push_back", replacer), (void (T::*)(const typename T::value_type &)) &T::push_back)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("reserve", replacer), (void (T::*)(typename T::size_type)) &T::reserve)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("resize", replacer), extractFunction2(&T::resize))
		.CPGF_MD_STL_TEMPLATE _method(replaceName("resize", replacer), extractFunction1(&T::resize))

		.CPGF_MD_STL_TEMPLATE _operator<typename T::reference (GMetaSelf, typename T::size_type)>(mopHolder[0])
		.CPGF_MD_STL_TEMPLATE _operator<typename T::const_reference (const GMetaSelf &, typename T::size_type)>(mopHolder[0])

#if CPGF_MD_STL_QUIRK_CONST_ITERATOR()		
		.CPGF_MD_STL_TEMPLATE _method(replaceName("insert", replacer), (typename T::iterator (T::*)(typename T::const_iterator, const typename T::value_type &)) &T::insert)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("insert", replacer), (void (T::*)(typename T::const_iterator, typename T::size_type, const typename T::value_type &)) &T::insert)
#else
		.CPGF_MD_STL_TEMPLATE _method(replaceName("insert", replacer), (typename T::iterator (T::*)(typename T::iterator, const typename T::value_type &)) &T::insert)
		.CPGF_MD_STL_TEMPLATE _method(replaceName("insert", replacer), (void (T::*)(typename T::iterator, typename T::size_type, const typename T::value_type &)) &T::insert)
#endif
	;
}

} // namespace metadata_internal


template <typename MetaDefine>
void bindMetaData_vector(MetaDefine define, bool scriptable, const GMetaDataNameReplacer * replacer = NULL)
{
	metadata_internal::doBindMetaData_vector<typename MetaDefine::ClassType>(define, scriptable, replacer);
}

template <typename MetaDefine>
void bindMetaData_vector(MetaDefine define, const GMetaDataNameReplacer * replacer = NULL)
{
	bindMetaData_vector(define, false, replacer);
}


} // namespace cpgf


#include "cpgf/metadata/stl/private/gmetadata_stl_footer.h"


#endif


