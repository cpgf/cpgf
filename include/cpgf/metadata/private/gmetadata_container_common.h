#ifndef __GMETADATA_CONTAINER_COMMON_H
#define __GMETADATA_CONTAINER_COMMON_H

#include "cpgf/metadata/stl/gmetadata_iterator.h"
#include "cpgf/gfunctionextractor.h"

#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/private/gmetadata_header.h"

namespace cpgf {

namespace metadata_internal {

template <typename T, typename MetaDefine, typename Policy>
void buildMetaData_CommonContainer(const GMetaDataConfigFlags & /*config*/, MetaDefine define, const Policy & /*policy*/, const GMetaDataNameReplacer * replacer)
{
	define
		.CPGF_MD_TEMPLATE _constructor<void * ()>()
		.CPGF_MD_TEMPLATE _constructor<void * (const T &)>()
		.CPGF_MD_TEMPLATE _method(replaceName("begin", replacer), (typename T::iterator (T::*)()) &T::begin)
		.CPGF_MD_TEMPLATE _method(replaceName("begin", replacer), (typename T::const_iterator (T::*)() const) &T::begin)
		.CPGF_MD_TEMPLATE _method(replaceName("clear", replacer), &T::clear)
		.CPGF_MD_TEMPLATE _method(replaceName("empty", replacer), &T::empty)
		.CPGF_MD_TEMPLATE _method(replaceName("end", replacer), (typename T::iterator (T::*)()) &T::end)
		.CPGF_MD_TEMPLATE _method(replaceName("end", replacer), (typename T::const_iterator (T::*)() const) &T::end)
		.CPGF_MD_TEMPLATE _method(replaceName("erase", replacer), extractFunction1(&T::erase))
		.CPGF_MD_TEMPLATE _method(replaceName("erase", replacer), extractFunction2(&T::erase))
		.CPGF_MD_TEMPLATE _method(replaceName("rbegin", replacer), (typename T::reverse_iterator (T::*)()) &T::rbegin)
		.CPGF_MD_TEMPLATE _method(replaceName("rbegin", replacer), (typename T::const_reverse_iterator (T::*)() const) &T::rbegin)
		.CPGF_MD_TEMPLATE _method(replaceName("rend", replacer), (typename T::reverse_iterator (T::*)()) &T::rend)
		.CPGF_MD_TEMPLATE _method(replaceName("rend", replacer), (typename T::const_reverse_iterator (T::*)() const) &T::rend)
		.CPGF_MD_TEMPLATE _method(replaceName("size", replacer), &T::size)
		.CPGF_MD_TEMPLATE _method(replaceName("swap", replacer), (void (T::*)(T &)) &T::swap)
	;
}

template <typename T, typename MetaDefine, typename Policy>
void buildMetaData_CommonIterators(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy, const GMetaDataNameReplacer * replacer)
{
	define
		.CPGF_MD_TEMPLATE _class(
			buildMetaData_iterator(
				config, GDefineMetaClass<typename T::iterator>::declare(replaceName("iterator", replacer)), policy, replacer
			)
		)
		.CPGF_MD_TEMPLATE _class(
			buildMetaData_constIterator(
				config, GDefineMetaClass<typename T::const_iterator>::declare(replaceName("const_iterator", replacer)), policy, replacer
			)
		)
		.CPGF_MD_TEMPLATE _class(
			buildMetaData_iterator(
				config, GDefineMetaClass<typename T::reverse_iterator>::declare(replaceName("reverse_iterator", replacer)), policy, replacer
			)
		)
		.CPGF_MD_TEMPLATE _class(
			buildMetaData_constIterator(
				config, GDefineMetaClass<typename T::const_reverse_iterator>::declare(replaceName("const_reverse_iterator", replacer)), policy, replacer
			)
		)
	;
}


} // namespace metadata_internal


} // namespace cpgf


#include "cpgf/metadata/private/gmetadata_footer.h"


#endif


