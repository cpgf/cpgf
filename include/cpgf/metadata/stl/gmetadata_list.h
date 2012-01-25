#ifndef __GMETADATA_LIST_H
#define __GMETADATA_LIST_H


#include "cpgf/metadata/stl/gmetadata_iterator.h"
#include "cpgf/metadata/private/gmetadata_container_common.h"

#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/private/gmetadata_header.h"


namespace cpgf {

namespace metadata_internal {

template <typename T, typename MetaDefine, typename Policy>
void doBuildMetaData_list(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy, const GMetaDataNameReplacer * replacer)
{
	metadata_internal::buildMetaData_CommonContainer<T>(config, define, policy, replacer);
	metadata_internal::buildMetaData_CommonIterators<T>(config, define, policy, replacer);

	define
		.CPGF_MD_TEMPLATE _method(replaceName("assign", replacer), (void (T::*)(typename T::size_type, const typename T::value_type &)) &T::assign, policy)
		.CPGF_MD_TEMPLATE _method(replaceName("back", replacer), (typename T::reference (T::*)()) &T::back, policy)
		.CPGF_MD_TEMPLATE _method(replaceName("back", replacer), (typename T::const_reference (T::*)() const) &T::back, policy)
		.CPGF_MD_TEMPLATE _method(replaceName("front", replacer), (typename T::reference (T::*)()) &T::front, policy)
		.CPGF_MD_TEMPLATE _method(replaceName("front", replacer), (typename T::const_reference (T::*)() const) &T::front, policy)
		.CPGF_MD_TEMPLATE _method(replaceName("max_size", replacer), (typename T::size_type (T::*)() const) &T::max_size)
		.CPGF_MD_TEMPLATE _method(replaceName("pop_back", replacer), (void (T::*)()) &T::pop_back)
		.CPGF_MD_TEMPLATE _method(replaceName("push_back", replacer), (void (T::*)(const typename T::value_type &)) &T::push_back, policy)
		.CPGF_MD_TEMPLATE _method(replaceName("resize", replacer), extractFunction2(&T::resize), policy)
		.CPGF_MD_TEMPLATE _method(replaceName("resize", replacer), (void (T::*)(typename T::size_type)) &T::resize, policy)
		.CPGF_MD_TEMPLATE _method(replaceName("remove", replacer), &T::remove, policy)
		.CPGF_MD_TEMPLATE _method(replaceName("unique", replacer), (void (T::*)()) &T::unique)
		.CPGF_MD_TEMPLATE _method(replaceName("merge", replacer), (void (T::*)(T &)) &T::merge)
		.CPGF_MD_TEMPLATE _method(replaceName("sort", replacer), (void (T::*)()) &T::sort)
		.CPGF_MD_TEMPLATE _method(replaceName("reverse", replacer), (void (T::*)()) &T::reverse)

#if CPGF_MD_STL_QUIRK_CONST_ITERATOR()		
		.CPGF_MD_TEMPLATE _method(replaceName("insert", replacer), (typename T::iterator (T::*)(typename T::const_iterator, const typename T::value_type &)) &T::insert, policy)
		.CPGF_MD_TEMPLATE _method(replaceName("insert", replacer), (void (T::*)(typename T::const_iterator, typename T::size_type, const typename T::value_type &)) &T::insert, policy)
		
		.CPGF_MD_TEMPLATE _method(replaceName("splice", replacer), (void (T::*)(typename T::const_iterator, T &)) &T::splice)
		.CPGF_MD_TEMPLATE _method(replaceName("splice", replacer), (void (T::*)(typename T::const_iterator, T &, typename T::const_iterator)) &T::splice)
		.CPGF_MD_TEMPLATE _method(replaceName("splice", replacer), (void (T::*)(typename T::const_iterator, T &, typename T::const_iterator, typename T::const_iterator)) &T::splice)
#else
		.CPGF_MD_TEMPLATE _method(replaceName("insert", replacer), (typename T::iterator (T::*)(typename T::iterator, const typename T::value_type &)) &T::insert, policy)
		.CPGF_MD_TEMPLATE _method(replaceName("insert", replacer), (void (T::*)(typename T::iterator, typename T::size_type, const typename T::value_type &)) &T::insert, policy)
		
		.CPGF_MD_TEMPLATE _method(replaceName("splice", replacer), (void (T::*)(typename T::iterator, T &)) &T::splice)
		.CPGF_MD_TEMPLATE _method(replaceName("splice", replacer), (void (T::*)(typename T::iterator, T &, typename T::iterator)) &T::splice)
		.CPGF_MD_TEMPLATE _method(replaceName("splice", replacer), (void (T::*)(typename T::iterator, T &, typename T::iterator, typename T::iterator)) &T::splice)
#endif
	;
}

} // namespace metadata_internal


template <typename MetaDefine, typename Policy>
void buildMetaData_list(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy, const GMetaDataNameReplacer * replacer = NULL)
{
	metadata_internal::doBuildMetaData_list<typename MetaDefine::ClassType>(config, define, policy, replacer);
}

template <typename MetaDefine, typename Policy>
void buildMetaData_list(MetaDefine define, const Policy & policy, const GMetaDataNameReplacer * replacer = NULL)
{
	buildMetaData_list(mdcScriptable, define, policy, replacer);
}


} // namespace cpgf


#include "cpgf/metadata/private/gmetadata_footer.h"



#endif


