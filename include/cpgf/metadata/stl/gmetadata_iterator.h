#ifndef __GMETADATA_ITERATOR_H
#define __GMETADATA_ITERATOR_H

#include "cpgf/gtypetraits.h"
#include "cpgf/gmetadefine.h"

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

template <typename T, typename MetaDefine>
void doBuildIteratorCommon(bool scriptable, MetaDefine define, const GMetaDataNameReplacer * replacer)
{
	define
		.CPGF_MD_STL_TEMPLATE _operator<T & (GMetaSelf, const T &)>(mopHolder == mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, const T &)>(mopHolder != mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(++mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(mopHolder++)
	;

	if(scriptable) {
		define
			.CPGF_MD_STL_TEMPLATE _method(replaceName("inc", replacer), &scriptableIterator_inc<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_STL_TEMPLATE _method(replaceName("equals", replacer), &scriptableIterator_equals<T>, GMetaPolicyExplicitThis())
		;
	}
}

template <typename T, typename MetaDefine>
void doBuildIterator(bool scriptable, MetaDefine define, const std::input_iterator_tag &, const GMetaDataNameReplacer * replacer)
{
	doBuildIteratorCommon<T>(scriptable, define, replacer);
}

template <typename T, typename MetaDefine>
void doBuildIterator(bool scriptable, MetaDefine define, const std::output_iterator_tag &, const GMetaDataNameReplacer * replacer)
{
	doBuildIteratorCommon<T>(scriptable, define, replacer);
}

template <typename T, typename MetaDefine>
void doBuildIterator(bool scriptable, MetaDefine define, const std::forward_iterator_tag &, const GMetaDataNameReplacer * replacer)
{
	doBuildIteratorCommon<T>(scriptable, define, replacer);
}

template <typename T, typename MetaDefine>
void doBuildIterator(bool scriptable, MetaDefine define, const std::bidirectional_iterator_tag &, const GMetaDataNameReplacer * replacer)
{
	doBuildIterator<T>(scriptable, define, std::forward_iterator_tag(), replacer);

	define
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(--mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(mopHolder--)
	;

	if(scriptable) {
		define
			.CPGF_MD_STL_TEMPLATE _method(replaceName("dec", replacer), &scriptableIterator_dec<T>, GMetaPolicyExplicitThis())
		;
	}
}

template <typename T, typename MetaDefine>
void doBuildIterator(bool scriptable, MetaDefine define, const std::random_access_iterator_tag &, const GMetaDataNameReplacer * replacer)
{
	doBuildIterator<T>(scriptable, define, std::bidirectional_iterator_tag(), replacer);

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

	if(scriptable) {
		define
			.CPGF_MD_STL_TEMPLATE _method(replaceName("skip", replacer), &scriptableIterator_skip<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_STL_TEMPLATE _method(replaceName("less", replacer), &scriptableIterator_less<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_STL_TEMPLATE _method(replaceName("lessOrEqual", replacer), &scriptableIterator_lessOrEqual<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_STL_TEMPLATE _method(replaceName("greater", replacer), &scriptableIterator_greater<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_STL_TEMPLATE _method(replaceName("greaterOrEqual", replacer), &scriptableIterator_greaterOrEqual<T>, GMetaPolicyExplicitThis())
		;
	}
}

template <typename T, typename MetaDefine>
void doBuildIteratorAccessor(bool scriptable, MetaDefine define, const GMetaDataNameReplacer * replacer)
{
	define
		.CPGF_MD_STL_TEMPLATE _operator<typename T::pointer (GMetaSelf)>(mopHolder->mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename T::reference (GMetaSelf)>(*mopHolder)
	;

	if(scriptable) {
		define
			.CPGF_MD_STL_TEMPLATE _method(replaceName("value", replacer), &scriptableIterator_value<T>, GMetaPolicyExplicitThis())
		;
	}
}


} // namespace metadata_internal


template <typename MetaDefine>
MetaDefine buildMetaData_iterator(bool scriptable, MetaDefine define, const GMetaDataNameReplacer * replacer = NULL)
{
	metadata_internal::doBuildIteratorAccessor<typename MetaDefine::ClassType>(scriptable, define, replacer);
	metadata_internal::doBuildIterator<typename MetaDefine::ClassType>(scriptable, define, typename MetaDefine::ClassType::iterator_category(), replacer);

	return define;
}

template <typename MetaDefine>
MetaDefine buildMetaData_iterator(MetaDefine define, const GMetaDataNameReplacer * replacer = NULL)
{
	return buildMetaData_iterator(true, define, replacer);
}


} // namespace cpgf


#include "cpgf/metadata/stl/private/gmetadata_stl_footer.h"

#endif

