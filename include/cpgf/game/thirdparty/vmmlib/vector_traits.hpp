// Copyright (c) 2010 Daniel Pfeifer

#ifndef __VMML_VECTOR_TRAITS_HPP__
#define __VMML_VECTOR_TRAITS_HPP__

#include <vmmlib/vector.hpp>
#include <boost/la/vector_traits.hpp>

namespace boost
{
namespace la
{

template<size_t M, typename T>
struct vector_traits<vmml::vector<M, T> >
{
	typedef vmml::vector<M, T> vector_type;

	static const int dim = M;

	typedef T scalar_type;

	template<int I>
	static scalar_type r(const vector_type& v)
	{
		BOOST_STATIC_ASSERT(I >= 0);
		BOOST_STATIC_ASSERT(I < dim);
		return v.array[I];
	}

	template<int I>
	static scalar_type& w(vector_type& v)
	{
		BOOST_STATIC_ASSERT(I >= 0);
		BOOST_STATIC_ASSERT(I < dim);
		return v.array[I];
	}

	static scalar_type ir(int i, const vector_type& v)
	{
		BOOST_ASSERT(i >= 0);
		BOOST_ASSERT(i < dim);
		return v.array[i];
	}

	static scalar_type& iw(int i, vector_type& v)
	{
		BOOST_ASSERT(i >= 0);
		BOOST_ASSERT(i < dim);
		return v.array[i];
	}
};

} // namespace la
} // namespace boost

#endif /* VMML_VECTOR_TRAITS_HPP */
