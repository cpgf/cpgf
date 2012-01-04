#ifndef __GMETADATA_ITERATOR_H
#define __GMETADATA_ITERATOR_H

#include "cpgf/gtypetraits.h"
#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/stl/private/gmetadata_stl_header.h"

namespace cpgf {

namespace metadata_internal {

template <typename T>
typename T::value_type scriptableIterator_value(T * it)
{
	return *(*it);
}

template <typename T>
typename T scriptableIterator_inc(T * it)
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
void doBindIteratorCommon(MetaDefine define, bool scriptable)
{
	define
		.CPGF_MD_STL_TEMPLATE _operator<T & (GMetaSelf, const T &)>(mopHolder == mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, const T &)>(mopHolder != mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(++mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(mopHolder++)
	;

	if(scriptable) {
		define
			.CPGF_MD_STL_TEMPLATE _method("inc", &scriptableIterator_inc<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_STL_TEMPLATE _method("equal", &scriptableIterator_equal<T>, GMetaPolicyExplicitThis())
		;
	}
}

template <typename T, typename MetaDefine>
void doBindIterator(MetaDefine define, bool scriptable, const std::input_iterator_tag &)
{
	doBindIteratorCommon<T>(define, scriptable);
}

template <typename T, typename MetaDefine>
void doBindIterator(MetaDefine define, bool scriptable, const std::output_iterator_tag &)
{
	doBindIteratorCommon<T>(define, scriptable);
}

template <typename T, typename MetaDefine>
void doBindIterator(MetaDefine define, bool scriptable, const std::forward_iterator_tag &)
{
	doBindIteratorCommon<T>(define, scriptable);
}

template <typename T, typename MetaDefine>
void doBindIterator(MetaDefine define, bool scriptable, const std::bidirectional_iterator_tag &)
{
	doBindIterator<T>(define, scriptable, std::forward_iterator_tag());

	define
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(--mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(mopHolder--)
	;

	if(scriptable) {
		define
			.CPGF_MD_STL_TEMPLATE _method("dec", &scriptableIterator_dec<T>, GMetaPolicyExplicitThis())
		;
	}
}

template <typename T, typename MetaDefine>
void doBindIterator(MetaDefine define, bool scriptable, const std::random_access_iterator_tag &)
{
	doBindIterator<T>(define, scriptable, std::bidirectional_iterator_tag());

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
			.CPGF_MD_STL_TEMPLATE _method("skip", &scriptableIterator_skip<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_STL_TEMPLATE _method("less", &scriptableIterator_less<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_STL_TEMPLATE _method("lessOrEqual", &scriptableIterator_lessOrEqual<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_STL_TEMPLATE _method("greater", &scriptableIterator_greater<T>, GMetaPolicyExplicitThis())
			.CPGF_MD_STL_TEMPLATE _method("greaterOrEqual", &scriptableIterator_greaterOrEqual<T>, GMetaPolicyExplicitThis())
		;
	}
}

template <typename T, typename MetaDefine>
void doBindIteratorAccessor(MetaDefine define, bool scriptable)
{
	define
		.CPGF_MD_STL_TEMPLATE _operator<typename T::pointer (GMetaSelf)>(mopHolder->mopHolder)
		.CPGF_MD_STL_TEMPLATE _operator<typename T::reference (GMetaSelf)>(*mopHolder)
	;

	if(scriptable) {
		define
			.CPGF_MD_STL_TEMPLATE _method("value", &scriptableIterator_value<T>, GMetaPolicyExplicitThis())
		;
	}
}


} // namespace metadata_internal


template <typename MetaDefine>
MetaDefine bindMetaData_iterator(MetaDefine define, bool scriptable)
{
	metadata_internal::doBindIteratorAccessor<typename MetaDefine::ClassType>(define, scriptable);
	metadata_internal::doBindIterator<typename MetaDefine::ClassType>(define, scriptable, typename MetaDefine::ClassType::iterator_category());

	return define;
}

template <typename MetaDefine>
MetaDefine bindMetaData_iterator(MetaDefine define)
{
	return bindMetaData_iterator(define, false);
}


} // namespace cpgf


#include "cpgf/metadata/stl/private/gmetadata_stl_footer.h"

#endif

