#ifndef __GMETADATA_VECTOR_H
#define __GMETADATA_VECTOR_H

#include "cpgf/metadata/stl/gmetadata_iterator.h"

#include "cpgf/gmetadefine.h"

#include <vector>


namespace cpgf {

template <typename T, typename MetaDefine>
void bindMetaData_vector(MetaDefine define)
{
	GDefineMetaClass<typename T::iterator> iteratorDefine = GDefineMetaClass<typename T::iterator>::create("iterator");
	bindMetaData_iterator<typename T::iterator>(iteratorDefine);

	define
		._constructor<void * ()>()
		._method("assign", (void (T::*) (T::size_type, const T::value_type&) ) &T::assign)
		//._method("assign", &impl::STL_Vector_AssignWrapper<impl::STLIteratorHolder<T::iterator, typename T::value_type>, T>)
		//._method("assign", &impl::STL_Vector_AssignWrapper<impl::STLIteratorHolder<T::const_iterator, typename T::value_type>, T>)
		//._method("assign", &impl::STL_Vector_AssignWrapper<impl::STLIteratorHolder<T::reverse_iterator, typename T::value_type>, T>)
		//._method("assign", &impl::STL_Vector_AssignWrapper<impl::STLIteratorHolder<T::const_reverse_iterator, typename T::value_type>, T>)
		//._method("at", &impl::STL_AtMethod<T>)
		//._method("at", &impl::STL_AtMethodConst<T>)
		//._method("back", &impl::STL_BackMethod<T>)
		//._method("back", &impl::STL_BackMethodConst<T>)
		._method("capacity", &T::capacity)
		//._method("front", &impl::STL_FrontMethod<T>)
		//._method("front", &impl::STL_FrontMethodConst<T>)
		//._method("insert", &impl::STL_Vector_Insert1<impl::STLIteratorHolder<T::iterator, typename T::value_type>, T>)
		//._method("insert", &impl::STL_Vector_Insert2<impl::STLIteratorHolder<T::iterator, typename T::value_type>, T>)
		._method("max_size", &T::max_size)
		._method("pop_back", &T::pop_back)
._method("push_back", (void (T::*)(T::value_type &&)) &T::push_back)
		._method("push_back", (void (T::*)(const T::value_type &)) &T::push_back)
		._method("reserve", &T::reserve)
		._method("resize", (void (T::*) (T::size_type, T::value_type)) &T::resize)
	;
}


} // namespace cpgf



#endif


