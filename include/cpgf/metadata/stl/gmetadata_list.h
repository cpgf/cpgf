#ifndef __GMETADATA_LIST_H
#define __GMETADATA_LIST_H


#include "cpgf/metadata/stl/gmetadata_iterator.h"
#include "cpgf/metadata/private/gmetadata_container_common.h"

#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/private/gmetadata_header.h"


namespace cpgf {

namespace metadata_internal {

template <typename T, typename MetaDefine, typename Policy>
void doBuildMetaData_list(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy)
{
	metadata_internal::buildMetaData_CommonContainer<T>(config, define, policy);
	metadata_internal::buildMetaData_CommonIterators<T>(config, define, policy);

	define
		.CPGF_MD_TEMPLATE _method("assign", (void (T::*)(typename T::size_type, const typename T::value_type &)) &T::assign, policy)
		.CPGF_MD_TEMPLATE _method("back", (typename T::reference (T::*)()) &T::back, policy)
		.CPGF_MD_TEMPLATE _method("back", (typename T::const_reference (T::*)() const) &T::back, policy)
		.CPGF_MD_TEMPLATE _method("front", (typename T::reference (T::*)()) &T::front, policy)
		.CPGF_MD_TEMPLATE _method("front", (typename T::const_reference (T::*)() const) &T::front, policy)
		.CPGF_MD_TEMPLATE _method("max_size", (typename T::size_type (T::*)() const) &T::max_size)
		.CPGF_MD_TEMPLATE _method("pop_back", (void (T::*)()) &T::pop_back)
		.CPGF_MD_TEMPLATE _method("push_back", (void (T::*)(const typename T::value_type &)) &T::push_back, policy)
		.CPGF_MD_TEMPLATE _method("resize", selectFunctionByArity2(&T::resize), policy)
		.CPGF_MD_TEMPLATE _method("resize", (void (T::*)(typename T::size_type)) &T::resize, policy)
		.CPGF_MD_TEMPLATE _method("remove", &T::remove, policy)
		.CPGF_MD_TEMPLATE _method("unique", (void (T::*)()) &T::unique)
		.CPGF_MD_TEMPLATE _method("merge", (void (T::*)(T &)) &T::merge)
		.CPGF_MD_TEMPLATE _method("sort", (void (T::*)()) &T::sort)
		.CPGF_MD_TEMPLATE _method("reverse", (void (T::*)()) &T::reverse)

		.CPGF_MD_TEMPLATE _method("insert", (typename T::iterator (T::*)(typename T::const_iterator, const typename T::value_type &)) &T::insert, policy)
		.CPGF_MD_TEMPLATE _method("insert", (typename T::iterator (T::*)(typename T::const_iterator, typename T::value_type &&)) &T::insert, policy)
		.CPGF_MD_TEMPLATE _method("insert", (typename T::iterator (T::*)(typename T::const_iterator, typename T::size_type, const typename T::value_type &)) &T::insert, policy)
		
		.CPGF_MD_TEMPLATE _method("splice", (void (T::*)(typename T::const_iterator, T &)) &T::splice)
		.CPGF_MD_TEMPLATE _method("splice", (void (T::*)(typename T::const_iterator, T &&)) &T::splice)
		.CPGF_MD_TEMPLATE _method("splice", (void (T::*)(typename T::const_iterator, T &, typename T::const_iterator)) &T::splice)
		.CPGF_MD_TEMPLATE _method("splice", (void (T::*)(typename T::const_iterator, T &&, typename T::const_iterator)) &T::splice)
		.CPGF_MD_TEMPLATE _method("splice", (void (T::*)(typename T::const_iterator, T &, typename T::const_iterator, typename T::const_iterator)) &T::splice)
		.CPGF_MD_TEMPLATE _method("splice", (void (T::*)(typename T::const_iterator, T &&, typename T::const_iterator, typename T::const_iterator)) &T::splice)
	;
}

} // namespace metadata_internal


template <typename MetaDefine, typename Policy>
void buildMetaData_list(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy)
{
	metadata_internal::doBuildMetaData_list<typename MetaDefine::ClassType>(config, define, policy);
}

template <typename MetaDefine, typename Policy>
void buildMetaData_list(MetaDefine define, const Policy & policy)
{
	buildMetaData_list(mdcScriptable, define, policy);
}


} // namespace cpgf


#include "cpgf/metadata/private/gmetadata_footer.h"



#endif


