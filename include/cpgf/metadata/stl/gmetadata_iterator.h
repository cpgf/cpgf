#ifndef __GMETADATA_ITERATOR_H
#define __GMETADATA_ITERATOR_H

#include "cpgf/gtypetraits.h"

#include <iterator>


namespace cpgf {

namespace metadata_internal {

template <typename T, typename MetaDefine>
void doBindIterator(MetaDefine define, const std::input_iterator_tag &)
{
	define
		._operator<typename AddReference<T>::Result (GMetaSelf)>(++mopHolder)
		._operator<typename AddReference<T>::Result (GMetaSelf)>(mopHolder++)
	;
}

template <typename T, typename MetaDefine>
void doBindIterator(MetaDefine define, const std::output_iterator_tag &)
{
}

template <typename T, typename MetaDefine>
void doBindIterator(MetaDefine define, const std::forward_iterator_tag &)
{
}

template <typename T, typename MetaDefine>
void doBindIterator(MetaDefine define, const std::bidirectional_iterator_tag &)
{
}

template <typename T, typename MetaDefine>
void doBindIterator(MetaDefine define, const std::random_access_iterator_tag &)
{
	doBindIterator<T>(define, std::input_iterator_tag());
}

} // namespace metadata_internal


template <typename T, typename MetaDefine>
void bindMetaData_iterator(MetaDefine define)
{
	define
		._operator<typename T::pointer (GMetaSelf)>(mopHolder->mopHolder)
		._operator<typename T::reference (GMetaSelf)>(*mopHolder)
	;

	metadata_internal::doBindIterator<T>(define, typename T::iterator_category ());
}



} // namespace cpgf



#endif
