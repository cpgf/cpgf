#ifndef __GMETADATA_ITERATOR_H
#define __GMETADATA_ITERATOR_H

#include "cpgf/gtypetraits.h"
#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/stl/private/gmetadata_stl_header.h"

namespace cpgf {

namespace metadata_internal {

template <typename T, typename MetaDefine>
void doBindIteratorCommon(MetaDefine define)
{
	define
		.TEMPLATE _operator<T & (GMetaSelf, const T &)>(mopHolder == mopHolder)
		.TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, const T &)>(mopHolder != mopHolder)
		.TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(++mopHolder)
		.TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(mopHolder++)
	;
}

template <typename T, typename MetaDefine>
void doBindIterator(MetaDefine define, const std::input_iterator_tag &)
{
	doBindIteratorCommon<T>(define);
}

template <typename T, typename MetaDefine>
void doBindIterator(MetaDefine define, const std::output_iterator_tag &)
{
	doBindIteratorCommon<T>(define);
}

template <typename T, typename MetaDefine>
void doBindIterator(MetaDefine define, const std::forward_iterator_tag &)
{
	doBindIteratorCommon<T>(define);
}

template <typename T, typename MetaDefine>
void doBindIterator(MetaDefine define, const std::bidirectional_iterator_tag &)
{
	doBindIterator<T>(define, std::forward_iterator_tag());

	define
		.TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(--mopHolder)
		.TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf)>(mopHolder--)
	;
}

template <typename T, typename MetaDefine>
void doBindIterator(MetaDefine define, const std::random_access_iterator_tag &)
{
	doBindIterator<T>(define, std::bidirectional_iterator_tag());

	define
		.TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, int)>(mopHolder + mopHolder)
		.TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, int)>(mopHolder += mopHolder)
		.TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, int)>(mopHolder - mopHolder)
		.TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, int)>(mopHolder -= mopHolder)
		.TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, int)>(mopHolder[mopHolder])
		.TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, const T &)>(mopHolder < mopHolder)
		.TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, const T &)>(mopHolder <= mopHolder)
		.TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, const T &)>(mopHolder > mopHolder)
		.TEMPLATE _operator<typename AddReference<T>::Result (GMetaSelf, const T &)>(mopHolder >= mopHolder)
	;
}

} // namespace metadata_internal


template <typename MetaDefine>
void bindMetaData_iterator(MetaDefine define)
{
	define
		.TEMPLATE _operator<typename MetaDefine::ClassType::pointer (GMetaSelf)>(mopHolder->mopHolder)
		.TEMPLATE _operator<typename MetaDefine::ClassType::reference (GMetaSelf)>(*mopHolder)
	;

	metadata_internal::doBindIterator<typename MetaDefine::ClassType>(define, typename MetaDefine::ClassType::iterator_category());
}



} // namespace cpgf


#include "cpgf/metadata/stl/private/gmetadata_stl_footer.h"

#endif

