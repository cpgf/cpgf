#ifndef __GMETADATA_ITERATOR_H
#define __GMETADATA_ITERATOR_H

#include "cpgf/gtypetraits.h"
#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"

namespace cpgf {

namespace metadata_internal {

template <typename T>
typename T::reference scriptableIterator_value(T * it)
{
	return *(*it);
}

template <typename T>
void scriptableIterator_set(T * it, const typename T::value_type & value)
{
	*(*it) = value;
}

template <typename T>
T scriptableIterator_inc(T * it)
{
	return ++(*it);
}

template <typename T>
T scriptableIterator_dec(T * it)
{
	return --(*it);
}

template <typename T>
T scriptableIterator_skip(T * it, int n)
{
	(*it) += n;
	return *it;
}

template <typename T>
bool scriptableIterator_equals(T * it, T * other)
{
	return (*it) == (*other);
}

template <typename T>
bool scriptableIterator_less(T * it, T * other)
{
	return (*it) < (*other);
}

template <typename T>
bool scriptableIterator_lessOrEqual(T * it, T * other)
{
	return (*it) <= (*other);
}

template <typename T>
bool scriptableIterator_greater(T * it, T * other)
{
	return (*it) > (*other);
}

template <typename T>
bool scriptableIterator_greaterOrEqual(T * it, T * other)
{
	return (*it) >= (*other);
}

template <typename T, typename MetaDefine, typename Policy>
void doBuildIteratorCommon(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & /*policy*/)
{
	define
		.CPGF_MD_TEMPLATE _operator<bool (GMetaSelf, const T &)>(mopHolder == mopHolder)
		.CPGF_MD_TEMPLATE _operator<bool (GMetaSelf, const T &)>(mopHolder != mopHolder)
		.CPGF_MD_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(++mopHolder)
		.CPGF_MD_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(mopHolder++)
	;

	if(metaDataConfigIsScriptable(config)) {
		define
			.CPGF_MD_TEMPLATE _method("inc", &scriptableIterator_inc<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_TEMPLATE _method("equals", &scriptableIterator_equals<T>, GMetaPolicyExplicitThis())
		;
	}
}

template <typename T, typename MetaDefine, typename Policy>
void doBuildIterator(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy, const std::input_iterator_tag &)
{
	doBuildIteratorCommon<T>(config, define, policy);
}

template <typename T, typename MetaDefine, typename Policy>
void doBuildIterator(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy, const std::output_iterator_tag &)
{
	doBuildIteratorCommon<T>(config, define, policy);
}

template <typename T, typename MetaDefine, typename Policy>
void doBuildIterator(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy, const std::forward_iterator_tag &)
{
	doBuildIteratorCommon<T>(config, define, policy);
}

template <typename T, typename MetaDefine, typename Policy>
void doBuildIterator(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy, const std::bidirectional_iterator_tag &)
{
	doBuildIterator<T>(config, define, policy, std::forward_iterator_tag());

	define
		.CPGF_MD_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(--mopHolder)
		.CPGF_MD_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(mopHolder--)
	;

	if(metaDataConfigIsScriptable(config)) {
		define
			.CPGF_MD_TEMPLATE _method("dec", &scriptableIterator_dec<T>, GMetaPolicyExplicitThis())
		;
	}
}

template <typename T, typename MetaDefine, typename Policy>
void doBuildIterator(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy, const std::random_access_iterator_tag &)
{
	doBuildIterator<T>(config, define, policy, std::bidirectional_iterator_tag());

	define
		.CPGF_MD_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, int)>(mopHolder + mopHolder)
		.CPGF_MD_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, int)>(mopHolder += mopHolder)
		.CPGF_MD_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, int)>(mopHolder - mopHolder)
		.CPGF_MD_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, int)>(mopHolder -= mopHolder)
		.CPGF_MD_TEMPLATE _operator<typename AddReference<typename T::value_type>::Result (GMetaSelf, int)>(mopHolder[mopHolder])
		.CPGF_MD_TEMPLATE _operator<bool (GMetaSelf, const T &)>(mopHolder < mopHolder)
		.CPGF_MD_TEMPLATE _operator<bool (GMetaSelf, const T &)>(mopHolder <= mopHolder)
		.CPGF_MD_TEMPLATE _operator<bool (GMetaSelf, const T &)>(mopHolder > mopHolder)
		.CPGF_MD_TEMPLATE _operator<bool (GMetaSelf, const T &)>(mopHolder >= mopHolder)
	;

	if(metaDataConfigIsScriptable(config)) {
		define
			.CPGF_MD_TEMPLATE _method("skip", &scriptableIterator_skip<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_TEMPLATE _method("less", &scriptableIterator_less<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_TEMPLATE _method("lessOrEqual", &scriptableIterator_lessOrEqual<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_TEMPLATE _method("greater", &scriptableIterator_greater<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_TEMPLATE _method("greaterOrEqual", &scriptableIterator_greaterOrEqual<T>, GMetaPolicyExplicitThis())
		;
	}
}

template <typename T, typename MetaDefine, typename Policy>
void doBuildIteratorAccessor(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & /*policy*/)
{
	define
		.CPGF_MD_TEMPLATE _operator<typename T::pointer (GMetaSelf)>(mopHolder->mopHolder)
		.CPGF_MD_TEMPLATE _operator<typename T::reference (GMetaSelf)>(*mopHolder)
	;

	if(metaDataConfigIsScriptable(config)) {
		define
			.CPGF_MD_TEMPLATE _method("value", &scriptableIterator_value<T>, GMetaPolicyExplicitThis())
		;
	}
}


} // namespace metadata_internal


template <typename MetaDefine, typename Policy>
MetaDefine buildMetaData_constIterator(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy)
{
	metadata_internal::doBuildIteratorAccessor<typename MetaDefine::ClassType>(config, define, policy);
    typename MetaDefine::ClassType::iterator_category category;
	metadata_internal::doBuildIterator<typename MetaDefine::ClassType>(config, define, policy, category);

	return define;
}

template <typename MetaDefine, typename Policy>
MetaDefine buildMetaData_constIterator(MetaDefine define, const Policy & policy)
{
	return buildMetaData_constIterator(mdcScriptable, define, policy);
}


template <typename MetaDefine, typename Policy>
MetaDefine buildMetaData_iterator(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy)
{
	buildMetaData_constIterator(config, define, policy);

	if(metaDataConfigIsScriptable(config)) {
		define
			.CPGF_MD_TEMPLATE _method("set", &metadata_internal::scriptableIterator_set<typename MetaDefine::ClassType>, MergePolicy<GMetaPolicyExplicitThis, Policy>())
		;
	}

	return define;
}

template <typename MetaDefine, typename Policy>
MetaDefine buildMetaData_iterator(MetaDefine define, const Policy & policy)
{
	return buildMetaData_writableIterator(mdcScriptable, define, policy);
}


} // namespace cpgf


#include "cpgf/metadata/private/gmetadata_footer.h"

#endif

