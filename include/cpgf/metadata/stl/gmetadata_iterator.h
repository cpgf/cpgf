#ifndef __GMETADATA_ITERATOR_H
#define __GMETADATA_ITERATOR_H

#include "cpgf/gtypetraits.h"
#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/stl/private/gmetadata_stl_header.h"

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
void doBuildIteratorCommon(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy, const GMetaDataNameReplacer * replacer)
{
	(void)policy;

	define
		.CPGF_MD_STL_TEMPLATE _operator<T & (GMetaSelf, const T &)>(mopHolder == mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, const T &)>(mopHolder != mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(++mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(mopHolder++)
	;

	if(metaDataConfigIsScriptable(config)) {
		define
			.CPGF_MD_STL_TEMPLATE _method(replaceName("inc", replacer), &scriptableIterator_inc<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_STL_TEMPLATE _method(replaceName("equals", replacer), &scriptableIterator_equals<T>, GMetaPolicyExplicitThis())
		;
	}
}

template <typename T, typename MetaDefine, typename Policy>
void doBuildIterator(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy, const std::input_iterator_tag &, const GMetaDataNameReplacer * replacer)
{
	doBuildIteratorCommon<T>(config, define, policy, replacer);
}

template <typename T, typename MetaDefine, typename Policy>
void doBuildIterator(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy, const std::output_iterator_tag &, const GMetaDataNameReplacer * replacer)
{
	doBuildIteratorCommon<T>(config, define, policy, replacer);
}

template <typename T, typename MetaDefine, typename Policy>
void doBuildIterator(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy, const std::forward_iterator_tag &, const GMetaDataNameReplacer * replacer)
{
	doBuildIteratorCommon<T>(config, define, policy, replacer);
}

template <typename T, typename MetaDefine, typename Policy>
void doBuildIterator(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy, const std::bidirectional_iterator_tag &, const GMetaDataNameReplacer * replacer)
{
	doBuildIterator<T>(config, define, policy, std::forward_iterator_tag(), replacer);

	define
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(--mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(mopHolder--)
	;

	if(metaDataConfigIsScriptable(config)) {
		define
			.CPGF_MD_STL_TEMPLATE _method(replaceName("dec", replacer), &scriptableIterator_dec<T>, GMetaPolicyExplicitThis())
		;
	}
}

template <typename T, typename MetaDefine, typename Policy>
void doBuildIterator(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy, const std::random_access_iterator_tag &, const GMetaDataNameReplacer * replacer)
{
	doBuildIterator<T>(config, define, policy, std::bidirectional_iterator_tag(), replacer);

	define
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, int)>(mopHolder + mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, int)>(mopHolder += mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, int)>(mopHolder - mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, int)>(mopHolder -= mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, int)>(mopHolder[mopHolder])
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, const T &)>(mopHolder < mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, const T &)>(mopHolder <= mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, const T &)>(mopHolder > mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, const T &)>(mopHolder >= mopHolder)
	;

	if(metaDataConfigIsScriptable(config)) {
		define
			.CPGF_MD_STL_TEMPLATE _method(replaceName("skip", replacer), &scriptableIterator_skip<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_STL_TEMPLATE _method(replaceName("less", replacer), &scriptableIterator_less<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_STL_TEMPLATE _method(replaceName("lessOrEqual", replacer), &scriptableIterator_lessOrEqual<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_STL_TEMPLATE _method(replaceName("greater", replacer), &scriptableIterator_greater<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_STL_TEMPLATE _method(replaceName("greaterOrEqual", replacer), &scriptableIterator_greaterOrEqual<T>, GMetaPolicyExplicitThis())
		;
	}
}

template <typename T, typename MetaDefine, typename Policy>
void doBuildIteratorAccessor(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy, const GMetaDataNameReplacer * replacer)
{
	(void)policy;

	define
		.CPGF_MD_STL_TEMPLATE _operator<typename T::pointer (GMetaSelf)>(mopHolder->mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename T::reference (GMetaSelf)>(*mopHolder)
	;

	if(metaDataConfigIsScriptable(config)) {
		define
			.CPGF_MD_STL_TEMPLATE _method(replaceName("value", replacer), &scriptableIterator_value<T>, GMetaPolicyExplicitThis())
		;
	}
}


} // namespace metadata_internal


template <typename MetaDefine, typename Policy>
MetaDefine buildMetaData_constIterator(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy, const GMetaDataNameReplacer * replacer = NULL)
{
	metadata_internal::doBuildIteratorAccessor<typename MetaDefine::ClassType>(config, define, policy, replacer);
	metadata_internal::doBuildIterator<typename MetaDefine::ClassType>(config, define, policy, typename MetaDefine::ClassType::iterator_category(), replacer);

	return define;
}

template <typename MetaDefine, typename Policy>
MetaDefine buildMetaData_constIterator(MetaDefine define, const Policy & policy, const GMetaDataNameReplacer * replacer = NULL)
{
	return buildMetaData_constIterator(mdcScriptable, define, policy, replacer);
}


template <typename MetaDefine, typename Policy>
MetaDefine buildMetaData_iterator(const GMetaDataConfigFlags & config, MetaDefine define, const Policy & policy, const GMetaDataNameReplacer * replacer = NULL)
{
	buildMetaData_constIterator(config, define, policy, replacer);

	if(metaDataConfigIsScriptable(config)) {
		define
			.CPGF_MD_STL_TEMPLATE _method(replaceName("set", replacer), &metadata_internal::scriptableIterator_set<typename MetaDefine::ClassType>, MergePolicy<GMetaPolicyExplicitThis, Policy>())
		;
	}

	return define;
}

template <typename MetaDefine, typename Policy>
MetaDefine buildMetaData_iterator(MetaDefine define, const Policy & policy, const GMetaDataNameReplacer * replacer = NULL)
{
	return buildMetaData_writableIterator(mdcScriptable, define, policy, replacer);
}


} // namespace cpgf


#include "cpgf/metadata/stl/private/gmetadata_stl_footer.h"

#endif

