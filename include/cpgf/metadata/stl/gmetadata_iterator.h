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
	return (*it) + n;
}

template <typename T>
bool scriptableIterator_equal(T * it, T * other)
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
void doBuildIteratorCommon(MetaDefine define, bool scriptable, const GMetaDataNameReplacer * replacer)
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
			.CPGF_MD_STL_TEMPLATE _method(replaceName("equal", replacer), &scriptableIterator_equal<T>, GMetaPolicyExplicitThis())
		;
	}
}

template <typename T, typename MetaDefine>
void doBuildIterator(MetaDefine define, bool scriptable, const std::input_iterator_tag &, const GMetaDataNameReplacer * replacer)
{
	doBuildIteratorCommon<T>(define, scriptable, replacer);
}

template <typename T, typename MetaDefine>
void doBuildIterator(MetaDefine define, bool scriptable, const std::output_iterator_tag &, const GMetaDataNameReplacer * replacer)
{
	doBuildIteratorCommon<T>(define, scriptable, replacer);
}

template <typename T, typename MetaDefine>
void doBuildIterator(MetaDefine define, bool scriptable, const std::forward_iterator_tag &, const GMetaDataNameReplacer * replacer)
{
	doBuildIteratorCommon<T>(define, scriptable, replacer);
}

template <typename T, typename MetaDefine>
void doBuildIterator(MetaDefine define, bool scriptable, const std::bidirectional_iterator_tag &, const GMetaDataNameReplacer * replacer)
{
	doBuildIterator<T>(define, scriptable, std::forward_iterator_tag(), replacer);

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
void doBuildIterator(MetaDefine define, bool scriptable, const std::random_access_iterator_tag &, const GMetaDataNameReplacer * replacer)
{
	doBuildIterator<T>(define, scriptable, std::bidirectional_iterator_tag(), replacer);

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
void doBuildIteratorAccessor(MetaDefine define, bool scriptable, const GMetaDataNameReplacer * replacer)
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
MetaDefine buildMetaData_iterator(MetaDefine define, bool scriptable, const GMetaDataNameReplacer * replacer = NULL)
{
	metadata_internal::doBuildIteratorAccessor<typename MetaDefine::ClassType>(define, scriptable, replacer);
	metadata_internal::doBuildIterator<typename MetaDefine::ClassType>(define, scriptable, typename MetaDefine::ClassType::iterator_category(), replacer);

	return define;
}

template <typename MetaDefine>
MetaDefine buildMetaData_iterator(MetaDefine define, const GMetaDataNameReplacer * replacer = NULL)
{
	return buildMetaData_iterator(define, false, replacer);
}


} // namespace cpgf


#include "cpgf/metadata/stl/private/gmetadata_stl_footer.h"

#endif

