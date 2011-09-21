//Copyright (c) 2010 Daniel Pfeifer

#ifndef __VMML_MATRIX_TRAITS_HPP__
#define __VMML_MATRIX_TRAITS_HPP__

#include <vmmlib/matrix.hpp>
#include <boost/la/matrix_traits.hpp>

namespace boost
{
namespace la
{

template<size_t M, size_t N, typename T>
struct matrix_traits<vmml::matrix<M, N, T> >
{
	typedef vmml::matrix<M, N, T> matrix_type;

	static const int rows = M;
	static const int cols = N;

	typedef T scalar_type;

	template<int Row, int Col>
	static scalar_type r(const matrix_type& m)
	{
		BOOST_STATIC_ASSERT(Row >= 0);
		BOOST_STATIC_ASSERT(Row < rows);
		BOOST_STATIC_ASSERT(Col >= 0);
		BOOST_STATIC_ASSERT(Col < cols);
		return m.at(Col, Row);
	}

	template<int Row, int Col>
	static scalar_type& w(matrix_type& m)
	{
		BOOST_STATIC_ASSERT(Row >= 0);
		BOOST_STATIC_ASSERT(Row < rows);
		BOOST_STATIC_ASSERT(Col >= 0);
		BOOST_STATIC_ASSERT(Col < cols);
		return m.at(Col, Row);
	}

	static scalar_type ir(int row, int col, const matrix_type& m)
	{
		BOOST_ASSERT(row >= 0);
		BOOST_ASSERT(row < rows);
		BOOST_ASSERT(col >= 0);
		BOOST_ASSERT(col < cols);
		return m.at(col, row);
	}

	static scalar_type& iw(int row, int col, matrix_type& m)
	{
		BOOST_ASSERT(row >= 0);
		BOOST_ASSERT(row < rows);
		BOOST_ASSERT(col >= 0);
		BOOST_ASSERT(col < cols);
		return m.at(col, row);
	}
};

} // namespace la
} // namespace boost

#endif /* VMML_MATRIX_TRAITS_HPP */
