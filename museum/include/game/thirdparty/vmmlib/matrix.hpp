#ifndef __VMML__MATRIX__HPP__
#define __VMML__MATRIX__HPP__

#include <vmmlib/vmmlib_config.hpp>

#include <vmmlib/matrix_functors.hpp>
#include <vmmlib/vector.hpp>
#include <vmmlib/math.hpp>
#include <vmmlib/exception.hpp>
#include <vmmlib/enable_if.hpp>

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstddef>
#include <limits>
#include <algorithm>
#include <string>
#include <cstring>

namespace vmml
{

// matrix of type T with m rows and n columns
template< size_t M, size_t N, typename T = float >
class matrix
{
public:
    typedef T                                       value_type;
	typedef T*                                      pointer;
	typedef T&                                      reference;
    typedef T*                                      iterator;
    typedef const T*                                const_iterator;
    typedef std::reverse_iterator< iterator >       reverse_iterator;
    typedef std::reverse_iterator< const_iterator > const_reverse_iterator;
    
    static const size_t         ROWS = M;
    static const size_t         COLS = N;
    
    // accessors
    inline T& operator()( size_t row_index, size_t col_index );
    inline const T& operator()( size_t row_index, size_t col_index ) const;

    inline T& at( size_t row_index, size_t col_index );
    inline const T& at( size_t row_index, size_t col_index ) const;

    // element iterators - NOTE: column-major order
    iterator                begin();
    iterator                end();
    const_iterator          begin() const;
    const_iterator          end() const;

    reverse_iterator        rbegin();
    reverse_iterator        rend();
    const_reverse_iterator  rbegin() const;
    const_reverse_iterator  rend() const;
    
    // ctors
    // note: this ctor does not initialize anything because of performance reasons.
    matrix();
    
    template< typename U >
    matrix( const matrix< M, N, U >& source_ );

    template< size_t P, size_t Q >
    matrix( const matrix< P, Q, T >& source_ );
    
    #ifndef VMMLIB_NO_CONVERSION_OPERATORS
    // auto conversion operator
    operator T*();
    operator const T*() const;
    #endif
    
    bool operator==( const matrix& other ) const;
    bool operator!=( const matrix& other ) const;

    // due to limited precision, two 'idential' matrices might seem different.
    // this function allows to specify a tolerance when comparing matrices.
    bool equals( const matrix& other, T tolerance ) const;
    // this version takes a comparison functor to compare the components of
    // the two matrices
    template< typename compare_t >
    bool equals( const matrix& other, compare_t& cmp ) const;

    // (this) matrix = left matrix_mxp * right matrix_pxn
    template< size_t P >
    void multiply(
        const matrix< M, P, T >& left,
        const matrix< P, N, T >& right 
        );

    // returned matrix_mxp = (this) matrix * other matrix_nxp;
    // note: using multiply(...) it avoids a copy of the resulting matrix
    template< size_t P >
    matrix< M, P, T > operator*( const matrix< N, P, T >& other ) const; 

    // operator *= is only enabled for square matrices
    template< size_t O, size_t P, typename TT >
    typename enable_if< M == N && O == P && M == O, TT >::type* 
    operator*=( const matrix< O, P, TT >& right );
    
	inline matrix operator+( const matrix& other ) const;
	inline matrix operator-( const matrix& other ) const;

	void operator+=( const matrix& other );
	void operator-=( const matrix& other );
    
	template< size_t O, size_t P, size_t Q, size_t R >
    typename enable_if< M == O + Q && N == P + R >::type* 
    direct_sum( const matrix< O, P, T >& m0, const matrix< Q, R, T >& m1 );

	// 
	// matrix-scalar operations / scaling 
	// 
    matrix operator*( T scalar );
    void operator*=( T scalar );

	//
	// matrix-vector operations
    //
	// transform column vector by matrix ( vec = matrix * vec )
    vector< M, T > operator*( const vector< N, T >& other ) const;

	// transform column vector by matrix ( vec = matrix * vec )
	// assume homogenous coords, e.g. vec3 = mat4x4 * vec3, with w = 1.0
    template< size_t O >
    vector< O, T > operator*( const vector< O, T >& vector_ ) const;
    
    inline matrix< M, N, T > operator-() const;
    matrix< M, N, T > negate() const;
    
    // compute tensor product: (this) = vector (X) vector
	void tensor( const vector< M, T >& u, const vector< N, T >& v );
    // tensor, for api compatibility with old vmmlib version. 
    // WARNING: for M = N = 4 only.
    template< size_t uM, size_t vM >
    typename enable_if< uM == 3 && vM == 3 && M == N && M == 4 >::type* 
    tensor( const vector< uM, T >& u, const vector< vM, T >& v );
	
	template< size_t O, size_t P >
	matrix< O, P, T >
    get_sub_matrix( size_t row_offset, size_t col_offset, 
        typename enable_if< O <= M && P <= N >::type* = 0 ) const;

	template< size_t O, size_t P >
	typename enable_if< O <= M && P <= N >::type*
    get_sub_matrix( matrix< O, P, T >& result, 
        size_t row_offset = 0, size_t col_offset = 0 ) const;

	template< size_t O, size_t P >
	typename enable_if< O <= M && P <= N >::type*
    set_sub_matrix( const matrix< O, P, T >& sub_matrix, 
		size_t row_offset = 0, size_t col_offset = 0 );

    // copies a transposed version of *this into transposedMatrix
    void transpose_to( matrix< N, M, T >& transpose_ ) const;	

    const matrix& operator=( const matrix& source_ );
    
    // these assignment operators return nothing to avoid silent loss of
    // precision
    template< typename U >
    void operator=( const matrix< M, N, U >& source_ );
    void operator=( const T old_fashioned_matrix[ M ][ N ] );

    // WARNING: data_array[] must be at least of size M * N - otherwise CRASH!
    // WARNING: assumes row_by_row layout - if this is not the case, 
    // use matrix::set( data_array, false )
    void operator=( const T* data_array );
    void operator=( const std::vector< T >& data );
	
	// sets all elements to fill_value
    void operator=( T fill_value );
    void fill( T fill_value );
    
    // note: this function copies elements until either the matrix is full or
    // the iterator equals end_.
    template< typename input_iterator_t >
    void set( input_iterator_t begin_, input_iterator_t end_, 
        bool row_major_layout = true );
        
    void zero();
    void identity();
        
    vector< M, T >  get_column( size_t column_index ) const;
    void get_column( size_t column_index, vector< M, T>& column ) const;

    void set_column( size_t index, const vector< M, T >& column );

    void get_column( size_t index, matrix< M, 1, T >& column ) const;
    void set_column( size_t index, const matrix< M, 1, T >& column );

    vector< N, T > get_row( size_t index ) const;
    void get_row( size_t index, vector< N, T >& row ) const;
    void set_row( size_t index,  const vector< N, T >& row );

    void get_row( size_t index, matrix< 1, N, T >& row ) const;
    void set_row( size_t index,  const matrix< 1, N, T >& row );
       
    size_t size() const; // return M * N;   
    
    size_t get_number_of_rows() const;
    size_t get_number_of_columns() const;
    
    T det() const;
    
    // the return value indicates if the matrix is invertible.
    // we need a tolerance term since the computation of the determinant is
    // subject to precision errors.
    template< size_t O, size_t P, typename TT >
    bool inverse( matrix< O, P, TT >& inverse_, 
        T tolerance = std::numeric_limits<T>::epsilon(),
        typename enable_if< M == N && O == P && O == M && M >= 2 && M <= 4, TT >::type* = 0 ) const;

    template< size_t O, size_t P >
    typename enable_if< O == P && M == N && O == M && M >= 2 >::type*
    get_adjugate( matrix< O, P, T >& adjugate ) const;
    
    template< size_t O, size_t P >
    typename enable_if< O == P && M == N && O == M && M >= 2 >::type*
    get_cofactors( matrix< O, P, T >& cofactors ) const;
    

    // returns the determinant of a square matrix M-1, N-1
    template< size_t O, size_t P >
    T get_minor( matrix< O, P, T >& minor_, size_t row_to_cut, size_t col_to_cut,
        typename enable_if< O == M-1 && P == N-1 && M == N && M >= 2 >::type* = 0 ) const;
    
    //
    // 4*4 matrices only
    //
    /** create rotation matrix from parameters.
    * @param angle - angle in radians
    * @param rotation axis - must be normalized!
    */
    template< typename TT >
    void rotate( const TT angle, const vector< M-1, T >& axis, 
        typename enable_if< M == N && M == 4, TT >::type* = 0 );

    template< typename TT >
    void rotate_x( const TT angle, 
        typename enable_if< M == N && M == 4, TT >::type* = 0 );

    template< typename TT >
    void rotate_y( const TT angle, 
        typename enable_if< M == N && M == 4, TT >::type* = 0 );
        
    template< typename TT >
    void rotate_z( const TT angle, 
        typename enable_if< M == N && M == 4, TT >::type* = 0 );
    
    template< typename TT >
    void pre_rotate_x( const TT angle, 
        typename enable_if< M == N && M == 4, TT >::type* = 0 );

    template< typename TT >
    void pre_rotate_y( const TT angle, 
        typename enable_if< M == N && M == 4, TT >::type* = 0 );
    
    template< typename TT >
    void pre_rotate_z( const TT angle, 
        typename enable_if< M == N && M == 4, TT >::type* = 0 );
  
    template< typename TT >
    void scale( const TT scale[3], 
        typename enable_if< M == N && M == 4, TT >::type* = 0 );
    
    template< typename TT >
    void scale( const TT x_, const T y, const T z, 
        typename enable_if< M == N && M == 4, TT >::type* = 0 );
    
    template< typename TT >
    inline void
    scale( const vector< 3, TT >& scale_, 
        typename enable_if< M == N && M == 4, TT >::type* = 0 );
    
    template< typename TT >
    inline void
    scale_translation( const TT scale_[3], 
        typename enable_if< M == N && M == 4, TT >::type* = 0 );
    
    template< typename TT >
    inline void scale_translation( const vector< 3, TT >& scale_, 
        typename enable_if< M == N && M == 4, TT >::type* = 0 );
    
    template< typename TT >
    inline void set_translation( const TT x_, const TT y_, const TT z_, 
        typename enable_if< M == N && M == 4, TT >::type* = 0 );
    
    template< typename TT >
    inline void set_translation( const TT trans[3],
        typename enable_if< M == N && M == 4, TT >::type* = 0 );
    
    template< typename TT >
    inline void set_translation( const vector< 3, TT >& translation_,
        typename enable_if< M == N && M == 4, TT >::type* = 0 );

    template< typename TT >
    inline void get_translation( vector< 3, TT >& translation_, 
        typename enable_if< M == N && M == 4, TT >::type* = 0 ) const;

	// hack for static-member-init
	template< typename init_functor_t >
	static const matrix get_initialized_matrix();

    inline T& x();
    inline T& y();
    inline T& z();
    
	// legacy/compatibility accessor
	struct row_accessor
	{
		row_accessor( T* array_ ) : array( array_ ) {}
		T&
		operator[]( size_t col_index )
		{ 
			#ifdef VMMLIB_SAFE_ACCESSORS
			if ( col_index >= N ) 
                VMMLIB_ERROR( "column index out of bounds", VMMLIB_HERE );
			#endif
			return array[ col_index * M ]; 
		}

		const T&
		operator[]( size_t col_index ) const
		{ 
			#ifdef VMMLIB_SAFE_ACCESSORS
			if ( col_index >= N ) 
                VMMLIB_ERROR( "column index out of bounds", VMMLIB_HERE );
			#endif
			return array[ col_index * M ]; 
		}
		
		T* array;
		private: row_accessor() {} // disallow std ctor
	};
	// this is a hack to allow array-style access to matrix elements
	// usage: matrix< 2, 2, float > m; m[ 1 ][ 0 ] = 37.0f;
	inline row_accessor operator[]( size_t row_index )
	{ 
		#ifdef VMMLIB_SAFE_ACCESSORS
		if ( row_index > M ) 
            VMMLIB_ERROR( "row index out of bounds", VMMLIB_HERE );
		#endif
		return row_accessor( array + row_index );
	}

    friend std::ostream& operator << ( std::ostream& os, 
        const matrix< M, N, T >& matrix )
    {
#ifdef EQ_EXPORT
        const std::ios::fmtflags flags = os.flags();
        const int                prec  = os.precision();
        
        os.setf( std::ios::right, std::ios::adjustfield );
        os.precision( 5 );

        for( size_t row_index = 0; row_index < M; ++row_index )
        {
            os << "|";
            for( size_t col_index = 0; col_index < N; ++col_index )
            {
                os << std::setw(10) << matrix.at( row_index, col_index ) << " ";
            }
            os << "|" << std::endl;
        }
        os.precision( prec );
        os.setf( flags );
#else
        for( size_t row_index = 0; row_index < M; ++row_index )
        {
            os << "(";
            for( size_t col_index = 0; col_index < N; ++col_index )
            {
                os << matrix.at( row_index, col_index );
				if (col_index < (N-1) )
					os << ", ";
            }
            os << ")" << std::endl;
        }
#endif
        return os;
    };  

    // protected:
    // column_by_column
    VMMLIB_ALIGN( T array[ M * N ] );
    
    // static members
    static const matrix< M, N, T > IDENTITY;
    static const matrix< M, N, T > ZERO;

}; // class matrix


#ifndef VMMLIB_NO_TYPEDEFS
typedef matrix< 3, 3, float >   mat3f;
typedef matrix< 3, 3, double  > mat3d;
typedef matrix< 4, 4, float >   mat4f;
typedef matrix< 4, 4, double >  mat4d;
#endif

/*
*   free functions
*/ 

template< size_t M, size_t N, typename T >
bool equals( const matrix< M, N, T >& m0, const matrix< M, N, T >& m1, T tolerance )
{
    return m0.equals( m1, tolerance );
}


template< size_t M, size_t N, typename T >
inline void 
multiply( const matrix< M, N, T >& left, const matrix< M, N, T >& right,
    matrix< M, N, T >& result )
{
    result.multiply( left, right );
}


template< size_t M, size_t N, size_t P, typename T >
inline void 
multiply( const matrix< M, P, T >& left, const matrix< P, N, T >& right, 
    matrix< M, N, T >& result )
{
    result.multiply( left, right );
}


template< size_t M, size_t N, typename T >
inline typename enable_if< M == N >::type*
identity( matrix< M, N, T >& matrix_ )
{
    matrix_ = static_cast< T >( 0.0 );
    for( size_t index = 0; index < M; ++index )
    {
        matrix_( index, index ) = static_cast< T >( 1.0 );
    }
    return 0; // for sfinae
}



template< typename T >
inline T compute_determinant( const matrix< 1, 1, T >& matrix_ )
{
    return matrix_.array[ 0 ];
}



template< typename T >
inline T compute_determinant( const matrix< 2, 2, T >& matrix_ )
{
    const T& a = matrix_( 0, 0 );
    const T& b = matrix_( 0, 1 );
    const T& c = matrix_( 1, 0 );
    const T& d = matrix_( 1, 1 );
    return a * d - b * c;
}



template< typename T >
inline T compute_determinant( const matrix< 3, 3, T >& m_ )
{
    return
          m_( 0,0 ) * ( m_( 1,1 ) * m_( 2,2 ) - m_( 1,2 ) * m_( 2,1 ) )
        + m_( 0,1 ) * ( m_( 1,2 ) * m_( 2,0 ) - m_( 1,0 ) * m_( 2,2 ) )
        + m_( 0,2 ) * ( m_( 1,0 ) * m_( 2,1 ) - m_( 1,1 ) * m_( 2,0 ) );
}


template< typename T >
inline T compute_determinant( const matrix< 4, 4, T >& m )
{
    T m00   = m( 0, 0 );
    T m10   = m( 1, 0 );
    T m20   = m( 2, 0 );
    T m30   = m( 3, 0 );
    T m01   = m( 0, 1 );
    T m11   = m( 1, 1 );
    T m21   = m( 2, 1 );
    T m31   = m( 3, 1 );
    T m02   = m( 0, 2 );
    T m12   = m( 1, 2 );
    T m22   = m( 2, 2 );
    T m32   = m( 3, 2 );
    T m03   = m( 0, 3 );
    T m13   = m( 1, 3 );
    T m23   = m( 2, 3 );
    T m33   = m( 3, 3 );

    return
        m03 * m12 * m21 * m30
            - m02 * m13 * m21 * m30
            - m03 * m11 * m22 * m30
            + m01 * m13 * m22 * m30
            + m02 * m11 * m23 * m30
            - m01 * m12 * m23 * m30
            - m03 * m12 * m20 * m31
            + m02 * m13 * m20 * m31
            + m03 * m10 * m22 * m31
            - m00 * m13 * m22 * m31
            - m02 * m10 * m23 * m31
            + m00 * m12 * m23 * m31
            + m03 * m11 * m20 * m32
            - m01 * m13 * m20 * m32
            - m03 * m10 * m21 * m32
            + m00 * m13 * m21 * m32
            + m01 * m10 * m23 * m32
            - m00 * m11 * m23 * m32
            - m02 * m11 * m20 * m33
            + m01 * m12 * m20 * m33
            + m02 * m10 * m21 * m33
            - m00 * m12 * m21 * m33
            - m01 * m10 * m22 * m33
            + m00 * m11 * m22 * m33;
}



template< typename T >
bool compute_inverse( const matrix< 2, 2, T >& m_, 
    matrix< 2, 2, T >& inverse_, 
    T tolerance_ = std::numeric_limits<T>::epsilon())
{
    T det_ = compute_determinant( m_ );
    if ( fabs( det_ ) < tolerance_ )
    {
        return false;
    }
    T detinv = static_cast< T >( 1.0 ) / det_;
    
    // set inverse_ to the adjugate of M
    m_.get_adjugate( inverse_ );
    
    inverse_ *= detinv;
    
    return true;
}



template< typename T >
bool compute_inverse( const matrix< 3, 3, T >& m_, 
    matrix< 3, 3, T >& inverse_, 
    T tolerance_ = std::numeric_limits<T>::epsilon() )
{
    // Invert a 3x3 using cofactors.  This is about 8 times faster than
    // the Numerical Recipes code which uses Gaussian elimination.

    inverse_.at( 0, 0 ) = m_.at( 1, 1 ) * m_.at( 2, 2 ) - m_.at( 1, 2 ) * m_.at( 2, 1 );
    inverse_.at( 0, 1 ) = m_.at( 0, 2 ) * m_.at( 2, 1 ) - m_.at( 0, 1 ) * m_.at( 2, 2 );
    inverse_.at( 0, 2 ) = m_.at( 0, 1 ) * m_.at( 1, 2 ) - m_.at( 0, 2 ) * m_.at( 1, 1 );
    inverse_.at( 1, 0 ) = m_.at( 1, 2 ) * m_.at( 2, 0 ) - m_.at( 1, 0 ) * m_.at( 2, 2 );
    inverse_.at( 1, 1 ) = m_.at( 0, 0 ) * m_.at( 2, 2 ) - m_.at( 0, 2 ) * m_.at( 2, 0 );
    inverse_.at( 1, 2 ) = m_.at( 0, 2 ) * m_.at( 1, 0 ) - m_.at( 0, 0 ) * m_.at( 1, 2 );
    inverse_.at( 2, 0 ) = m_.at( 1, 0 ) * m_.at( 2, 1 ) - m_.at( 1, 1 ) * m_.at( 2, 0 );
    inverse_.at( 2, 1 ) = m_.at( 0, 1 ) * m_.at( 2, 0 ) - m_.at( 0, 0 ) * m_.at( 2, 1 );
    inverse_.at( 2, 2 ) = m_.at( 0, 0 ) * m_.at( 1, 1 ) - m_.at( 0, 1 ) * m_.at( 1, 0 );
    
    const T determinant =
          m_.at( 0, 0 ) * inverse_.at( 0, 0 ) 
        + m_.at( 0, 1 ) * inverse_.at( 1, 0 )
        + m_.at( 0, 2 ) * inverse_.at( 2, 0 );
    
    if ( fabs( determinant ) <= tolerance_ )
        return false; // matrix is not invertible

    const T detinv = static_cast< T >( 1.0 ) / determinant;
    
    inverse_.at( 0, 0 ) *= detinv;
    inverse_.at( 0, 1 ) *= detinv;
    inverse_.at( 0, 2 ) *= detinv;
    inverse_.at( 1, 0 ) *= detinv;
    inverse_.at( 1, 1 ) *= detinv;
    inverse_.at( 1, 2 ) *= detinv;
    inverse_.at( 2, 0 ) *= detinv;
    inverse_.at( 2, 1 ) *= detinv;
    inverse_.at( 2, 2 ) *= detinv;

    return true;
}


template< typename T >
bool compute_inverse( const matrix< 4, 4, T >& m_, 
    matrix< 4, 4, T >& inv_, 
    T tolerance_ = std::numeric_limits<T>::epsilon() )
{
    const T* array = m_.array;
    
    // tuned version from Claude Knaus
    /* first set of 2x2 determinants: 12 multiplications, 6 additions */
    const T t1[6] = { array[ 2] * array[ 7] - array[ 6] * array[ 3],
                      array[ 2] * array[11] - array[10] * array[ 3],
                      array[ 2] * array[15] - array[14] * array[ 3],
                      array[ 6] * array[11] - array[10] * array[ 7],
                      array[ 6] * array[15] - array[14] * array[ 7],
                      array[10] * array[15] - array[14] * array[11] };

    /* first half of comatrix: 24 multiplications, 16 additions */
    inv_.array[0] = array[ 5] * t1[5] - array[ 9] * t1[4] + array[13] * t1[3];
    inv_.array[1] = array[ 9] * t1[2] - array[13] * t1[1] - array[ 1] * t1[5];
    inv_.array[2] = array[13] * t1[0] - array[ 5] * t1[2] + array[ 1] * t1[4];
    inv_.array[3] = array[ 5] * t1[1] - array[ 1] * t1[3] - array[ 9] * t1[0];
    inv_.array[4] = array[ 8] * t1[4] - array[ 4] * t1[5] - array[12] * t1[3];
    inv_.array[5] = array[ 0] * t1[5] - array[ 8] * t1[2] + array[12] * t1[1];
    inv_.array[6] = array[ 4] * t1[2] - array[12] * t1[0] - array[ 0] * t1[4];
    inv_.array[7] = array[ 0] * t1[3] - array[ 4] * t1[1] + array[ 8] * t1[0];

   /* second set of 2x2 determinants: 12 multiplications, 6 additions */
    const T t2[6] = { array[ 0] * array[ 5] - array[ 4] * array[ 1],
                      array[ 0] * array[ 9] - array[ 8] * array[ 1],
                      array[ 0] * array[13] - array[12] * array[ 1],
                      array[ 4] * array[ 9] - array[ 8] * array[ 5],
                      array[ 4] * array[13] - array[12] * array[ 5],
                      array[ 8] * array[13] - array[12] * array[ 9] };

    /* second half of comatrix: 24 multiplications, 16 additions */
    inv_.array[8]  = array[ 7] * t2[5] - array[11] * t2[4] + array[15] * t2[3];
    inv_.array[9]  = array[11] * t2[2] - array[15] * t2[1] - array[ 3] * t2[5];
    inv_.array[10] = array[15] * t2[0] - array[ 7] * t2[2] + array[ 3] * t2[4];
    inv_.array[11] = array[ 7] * t2[1] - array[ 3] * t2[3] - array[11] * t2[0];
    inv_.array[12] = array[10] * t2[4] - array[ 6] * t2[5] - array[14] * t2[3];
    inv_.array[13] = array[ 2] * t2[5] - array[10] * t2[2] + array[14] * t2[1];
    inv_.array[14] = array[ 6] * t2[2] - array[14] * t2[0] - array[ 2] * t2[4];
    inv_.array[15] = array[ 2] * t2[3] - array[ 6] * t2[1] + array[10] * t2[0];

   /* determinant: 4 multiplications, 3 additions */
   const T determinant = array[0] * inv_.array[0] + array[4] * inv_.array[1] +
                         array[8] * inv_.array[2] + array[12] * inv_.array[3];

   if( fabs( determinant ) <= tolerance_ )
        return false; // matrix is not invertible

   /* division: 16 multiplications, 1 division */
   const T detinv = static_cast< T >( 1.0 ) / determinant;

   for( size_t i = 0; i != 16; ++i )
       inv_.array[i] *= detinv;
       
    return true;
}


// this function returns the transpose of a matrix
// however, using matrix::transpose_to( .. ) avoids the copy.
template< size_t M, size_t N, typename T >
inline matrix< N, M, T >
transpose( const matrix< M, N, T >& matrix_ )
{
    matrix< N, M, T > transpose_;
    matrix_.transpose_to( transpose_ );
    return transpose_;
}


template< size_t M, size_t N, typename T >
bool is_positive_definite( const matrix< M, N, T >& matrix_, 
    const T limit = -1e12, 
    typename enable_if< M == N && M <= 3 >::type* = 0 )
{
    if ( matrix_.at( 0, 0 ) < limit )
        return false;
    
    // sylvester criterion
    if ( M > 1 )
    {
        matrix< 2, 2, T > m;
        matrix_.get_sub_matrix( m, 0, 0 );
        if ( compute_determinant( m ) < limit )
            return false;
    }
    
    if ( M > 2 )
    {
        matrix< 3, 3, T > m;
        matrix_.get_sub_matrix( m, 0, 0 );
        if ( compute_determinant( m ) < limit )
            return false;
    }

    return true;
}


template< size_t M, size_t N, typename T >
matrix< M, N, T >::matrix()
{
    // no initialization for performance reasons.
}



template< size_t M, size_t N, typename T >
template< typename U >
matrix< M, N, T >::
matrix( const matrix< M, N, U >& source_ )
{
    (*this) = source_;
}

template< size_t M, size_t N, typename T >
template< size_t P, size_t Q >
matrix< M, N, T >::
matrix( const matrix< P, Q, T >& source_ )
{
   const size_t minL =  P < M ? P : M;
   const size_t minC =  Q < N ? Q : N;

   (*this) = ZERO;
   
   for ( size_t i = 0 ; i < minL ; i++ )
       for ( size_t j = 0 ; j < minC ; j++ )
       {
           at( i,j ) = source_( i, j ); 
       }
}

template< size_t M, size_t N, typename T >
inline T&
matrix< M, N, T >::at( size_t row_index, size_t col_index )
{
    #ifdef VMMLIB_SAFE_ACCESSORS
    if ( row_index >= M || col_index >= N )
        VMMLIB_ERROR( "at( row, col ) - index out of bounds", VMMLIB_HERE );
    #endif
    return array[ col_index * M + row_index ];
}



template< size_t M, size_t N, typename T >
const inline T&
matrix< M, N, T >::at( size_t row_index, size_t col_index ) const
{
    #ifdef VMMLIB_SAFE_ACCESSORS
    if ( row_index >= M || col_index >= N )
        VMMLIB_ERROR( "at( row, col ) - index out of bounds", VMMLIB_HERE );
    #endif
    return array[ col_index * M + row_index ];
}


template< size_t M, size_t N, typename T >
inline T&
matrix< M, N, T >::operator()( size_t row_index, size_t col_index )
{
    return at( row_index, col_index );
}



template< size_t M, size_t N, typename T >
const inline T&
matrix< M, N, T >::operator()( size_t row_index, size_t col_index ) const
{
    return at( row_index, col_index );
}


#if 0
template< size_t M, size_t N, typename T >
matrix< M, N, T >::matrix()
{
}



template< size_t M, size_t N, typename T >
matrix< M, N, T >::matrix( const matrix< M, N >& original )
{

}
#endif


#ifndef VMMLIB_NO_CONVERSION_OPERATORS

template< size_t M, size_t N, typename T >
matrix< M, N, T >::
operator T*()
{
    return array;
}



template< size_t M, size_t N, typename T >
matrix< M, N, T >::
operator const T*() const
{
    return array;
}

#endif


template< size_t M, size_t N, typename T >
bool
matrix< M, N, T >::
operator==( const matrix< M, N, T >& other ) const
{
    bool ok = true;
    for( size_t i = 0; ok && i < M * N; ++i )
    {
        ok = array[ i ] == other.array[ i ];
    }
    return ok;
}




template< size_t M, size_t N, typename T >
bool
matrix< M, N, T >::
operator!=( const matrix< M, N, T >& other ) const
{
    return ! operator==( other );
}



template< size_t M, size_t N, typename T >
bool
matrix< M, N, T >::
equals( const matrix< M, N, T >& other, T tolerance ) const
{
    bool ok = true;
    for( size_t row_index = 0; ok && row_index < M; row_index++)
    {
        for( size_t col_index = 0; ok && col_index < N; col_index++) 
        {
            ok = fabs( at( row_index, col_index ) - other( row_index, col_index ) ) < tolerance;
        }
    }
    return ok;
}



template< size_t M, size_t N, typename T >
template< typename compare_t >
bool
matrix< M, N, T >::
equals( const matrix< M, N, T >& other_matrix, compare_t& cmp ) const
{
    bool ok = true;
    for( size_t row = 0; ok && row < M; ++row )
    {
        for( size_t col = 0; ok && col < N; ++col) 
        {
            ok = cmp( at( row, col ), other_matrix.at( row, col ) );
        }
    }
    return ok;
}



template< size_t M, size_t N, typename T >
const matrix< M, N, T >& 
matrix< M, N, T >::operator=( const matrix& source_ )
{
    memcpy( array, source_.array, M * N * sizeof( T ) );
    return *this;
}



template< size_t M, size_t N, typename T >
template< typename U >
void
matrix< M, N, T >::operator=( const matrix< M, N, U >& source_ )
{
    const U* it = source_.begin();
    const U* it_end = source_.end();
    
    iterator my_it = begin();
    for( ; it != it_end; ++it, ++my_it )
    {
        *my_it = static_cast< T >( *it );
    }
}


template< size_t M, size_t N, typename T >
void
matrix< M, N, T >::operator=( const T old_fashioned_matrix[ M ][ N ] )
{
    for( size_t row = 0; row < M; row++)
    {
        for( size_t col = 0; col < N; col++) 
        {
            at( row, col ) = old_fashioned_matrix[ row ][ col ];
        }
    }

}


// WARNING: data_array[] must be at least of size M * N - otherwise CRASH!
// WARNING: assumes row_by_row layout - if this is not the case, 
// use matrix::set( data_array, false )
template< size_t M, size_t N, typename T >
void
matrix< M, N, T >::operator=( const T* data_array )
{
    set( data_array, data_array + M * N, true );
}



template< size_t M, size_t N, typename T >
void
matrix< M, N, T >::operator=( const std::vector< T >& data )
{
    if ( data.size() < M * N )
        VMMLIB_ERROR( "index out of bounds.", VMMLIB_HERE );

    set( data.begin(), data.end(), true );
}



template< size_t M, size_t N, typename T >
template< size_t P >
void
matrix< M, N, T >::multiply( 
    const matrix< M, P, T >& left,
    const matrix< P, N, T >& right 
    )
{
    for( size_t row_index = 0; row_index < M; row_index++)
    {
        for( size_t col_index = 0; col_index < N; col_index++) 
        {
            T& component = at( row_index, col_index );
            component = static_cast< T >( 0.0 );
            for( size_t p = 0; p < P; p++)
            {
                component += left.at( row_index, p ) * right.at( p, col_index );
            }
        }
    }
}



template< size_t M, size_t N, typename T >
template< size_t P >
matrix< M, P, T >
matrix< M, N, T >::operator*( const matrix< N, P, T >& other ) const
{
    matrix< M, P, T > result;
    result.multiply( *this, other );
    return result;
}



template< size_t M, size_t N, typename T >
template< size_t O, size_t P, typename TT >
typename enable_if< M == N && O == P && M == O, TT >::type* 
matrix< M, N, T >::operator*=( const matrix< O, P, TT >& right )
{
    matrix< M, N, T > copy( *this );
    multiply( copy, right );
    return 0;
}



template< size_t M, size_t N, typename T >
matrix< M, N, T >
matrix< M, N, T >::operator*( T scalar )
{
    matrix< M, N, T > result;
    
    for( size_t row_index = 0; row_index < M; ++row_index )
    {
        for( size_t col_index = 0; col_index < N; ++col_index )
        {
            result.at( row_index, col_index ) = at( row_index, col_index ) * scalar;
        }
    }
    return result;
}



template< size_t M, size_t N, typename T >
void
matrix< M, N, T >::operator*=( T scalar )
{
    for( size_t row_index = 0; row_index < M; ++row_index )
    {
        for( size_t col_index = 0; col_index < N; ++col_index )
        {
            at( row_index, col_index ) *= scalar;
        }
    }
}



template< size_t M, size_t N, typename T >
vector< M, T >
matrix< M, N, T >::
operator*( const vector< N, T >& other ) const
{
    vector< M, T > result;

    // this < M, 1 > = < M, P > * < P, 1 >
    T tmp;
    for( size_t row_index = 0; row_index < M; row_index++)
    {
        tmp = static_cast< T >( 0.0 );
        for( size_t p = 0; p < N; p++)
        {
            tmp += at( row_index, p ) * other.at( p );
        }
        result.at( row_index ) = tmp;
    }
    return result;
}



// transform vector by matrix ( vec = matrix * vec )
// assume homogenous coords( for vec3 = mat4 * vec3 ), e.g. vec[3] = 1.0
template< size_t M, size_t N, typename T >
template< size_t O >
vector< O, T >
matrix< M, N, T >::
operator*( const vector< O, T >& vector_ ) const
{
    vector< O, T > result;
    T tmp;
    for( size_t row_index = 0; row_index < M; ++row_index )
    {
        tmp = 0.0;
        for( size_t col_index = 0; col_index < N-1; ++col_index )
        {
            tmp += vector_( col_index ) * at( row_index, col_index );
        }
        if ( row_index < N - 1 )
            result( row_index ) = tmp + at( row_index, N-1 ); // * 1.0 -> homogeneous vec4
        else
        {
            tmp += at( row_index, N - 1  );
            for( size_t col_index = 0; col_index < N - 1; ++col_index )
            {
                result( col_index ) /= tmp;
            }
        }
    }
    return result;
}



template< size_t M, size_t N, typename T >
inline matrix< M, N, T >
matrix< M, N, T >::operator-() const
{
    return negate();
}



template< size_t M, size_t N, typename T >
matrix< M, N, T >
matrix< M, N, T >::negate() const
{
    matrix< M, N, T > result;
    result *= -1.0;
    return result;
}



template< size_t M, size_t N, typename T >
void 
matrix< M, N, T >::tensor( 
	const vector< M, T >& u,
	const vector< N, T >& v 
	)
{
	for ( size_t col_index = 0; col_index < N; ++col_index )
		for ( size_t row_index = 0; row_index < M; ++row_index )
            at( row_index, col_index ) = u.array[ col_index ] * v.array[ row_index ];

}



template< size_t M, size_t N, typename T >
template< size_t uM, size_t vM >
typename enable_if< uM == 3 && vM == 3 && M == N && M == 4 >::type* 
matrix< M, N, T >::tensor( const vector< uM, T >& u, const vector< vM, T >& v )
{
    int i, j;
    for ( size_t col_index = 0; col_index < 3; ++col_index ) 
    {
        for ( size_t row_index = 0; row_index < 3; ++row_index )
            at( row_index, col_index ) = u.array[ col_index ] * v.array[ row_index ];

        at( 3, col_index ) = u.array[ col_index ];
    }

    for ( size_t row_index = 0; row_index < 3; ++row_index )
        at( row_index, 3 ) = v.array[ row_index ];

    at( 3, 3 ) = 1.0;

    return 0;
}



template< size_t M, size_t N, typename T >
void
matrix< M, N, T >::
transpose_to( matrix< N, M, T >& tM ) const
{
    for( size_t row = 0; row < M; ++row )
    {
        for( size_t col = 0; col < N; ++col )
        {
            tM.at( col, row ) = at( row, col );
        }
    }
}



template< size_t M, size_t N, typename T >
vector< M, T > 
matrix< M, N, T >::
get_column( size_t index ) const
{
	vector< M, T > column;
	get_column( index, column );
	return column;
}



template< size_t M, size_t N, typename T >
void
matrix< M, N, T >::
get_column( size_t index, vector< M, T >& column ) const
{
    #ifdef VMMLIB_SAFE_ACCESSORS

    if ( index >= N )
        VMMLIB_ERROR( "get_column() - index out of bounds.", VMMLIB_HERE );

    #endif
    
    memcpy( &column.array[0], &array[ M * index ], M * sizeof( T ) );
}



template< size_t M, size_t N, typename T >
void
matrix< M, N, T >::
set_column( size_t index, const vector< M, T >& column )
{
    #ifdef VMMLIB_SAFE_ACCESSORS
    
    if ( index >= N )
        VMMLIB_ERROR( "set_column() - index out of bounds.", VMMLIB_HERE );
    
    #endif

    memcpy( array + M * index, column.array, M * sizeof( T ) );
}



template< size_t M, size_t N, typename T >
void
matrix< M, N, T >::
get_column( size_t index, matrix< M, 1, T >& column ) const
{
    #ifdef VMMLIB_SAFE_ACCESSORS
    
    if ( index >= N )
        VMMLIB_ERROR( "get_column() - index out of bounds.", VMMLIB_HERE );
    
    #endif

    memcpy( column.array, array + M * index, M * sizeof( T ) );
}


template< size_t M, size_t N, typename T >
void
matrix< M, N, T >::
set_column( size_t index, const matrix< M, 1, T >& column )
{
    #ifdef VMMLIB_SAFE_ACCESSORS
    
    if ( index >= N )
        VMMLIB_ERROR( "set_column() - index out of bounds.", VMMLIB_HERE );
        
    #endif

    memcpy( &array[ M * index ], column.array, M * sizeof( T ) );
}



template< size_t M, size_t N, typename T >
vector< N, T > 
matrix< M, N, T >::
get_row( size_t index ) const
{
	vector< N, T > row;
	get_row( index, row );
	return row;
}



template< size_t M, size_t N, typename T >
void
matrix< M, N, T >::
get_row( size_t row_index, vector< N, T >& row ) const
{
    #ifdef VMMLIB_SAFE_ACCESSORS
    if ( row_index >= M )
        VMMLIB_ERROR( "get_row() - index out of bounds.", VMMLIB_HERE );
    #endif

    for( size_t col_index = 0; col_index < N; ++col_index )
    {
        row.at( col_index ) = at( row_index, col_index );
    }
}



template< size_t M, size_t N, typename T >
void
matrix< M, N, T >::
set_row( size_t row_index, const vector< N, T >& row )
{
    #ifdef VMMLIB_SAFE_ACCESSORS
    if ( row_index >= M )
        VMMLIB_ERROR( "set_row() - index out of bounds.", VMMLIB_HERE );
    #endif

    for( size_t col_index = 0; col_index < N; ++col_index )
    {
        at( row_index, col_index ) = row.at( col_index );
    }
}



template< size_t M, size_t N, typename T >
void
matrix< M, N, T >::
get_row( size_t row_index, matrix< 1, N, T >& row ) const
{
    #ifdef VMMLIB_SAFE_ACCESSORS
    if ( row_index >= M )
        VMMLIB_ERROR( "get_row() - index out of bounds.", VMMLIB_HERE );
    #endif

    for( size_t col_index = 0; col_index < N; ++col_index )
    {
        row.at( 0, col_index ) = at( row_index, col_index );
    }
}



template< size_t M, size_t N, typename T >
void
matrix< M, N, T >::
set_row( size_t row_index,  const matrix< 1, N, T >& row )
{
    #ifdef VMMLIB_SAFE_ACCESSORS
    if ( row_index >= M )
        VMMLIB_ERROR( "set_row() - index out of bounds.", VMMLIB_HERE );
    #endif

    for( size_t col_index = 0; col_index < N; ++col_index )
    {
        at( row_index, col_index ) = row.at( 0, col_index );
    }
}



/*
template< size_t M, size_t N, typename T >
matrix< 1, N, T >&
matrix< M, N, T >::
operator[]( size_t row_index )
{
    if ( row_index >= M ) throw "FIXME.";
    assert( row_index < M );
    return _rows[ row_index ];
}



template< size_t M, size_t N, typename T >
const matrix< 1, N, T >&
matrix< M, N, T >::
operator[]( size_t row_index ) const
{
    if ( row_index >= M ) throw "FIXME.";
    assert( row_index < M );
    return _rows[ row_index ];
}


*/
template< size_t M, size_t N, typename T >
size_t
matrix< M, N, T >::
get_number_of_rows() const
{
    return M;
}



template< size_t M, size_t N, typename T >
size_t
matrix< M, N, T >::
get_number_of_columns() const
{ 
    return N; 
}



template< size_t M, size_t N, typename T >
void
matrix< M, N, T >::
fill( T fillValue )
{
    for( size_t row_index = 0; row_index < M; ++row_index )
    {
        for( size_t col_index = 0; col_index < N; ++col_index )
        {
			at( row_index, col_index ) = fillValue;
		}
    }
}


template< size_t M, size_t N, typename T >
inline T&
matrix< M, N, T >::x()
{
    return array[ 12 ];
}



template< size_t M, size_t N, typename T >
inline T&
matrix< M, N, T >::y()
{
    return array[ 13 ];
}



template< size_t M, size_t N, typename T >
inline T&
matrix< M, N, T >::z()
{
    return array[ 14 ];
}

template< size_t M, size_t N, typename T >
template< typename input_iterator_t >
void
matrix< M, N, T >::
set( input_iterator_t begin_, input_iterator_t end_, bool row_major_layout )
{
    input_iterator_t it( begin_ );
    if( row_major_layout )
    {
        for( size_t row = 0; row < M; ++row )
        {
            for( size_t col = 0; col < N; ++col, ++it )
            {
                if ( it == end_ )
                    return;
                at( row, col ) = static_cast< T >( *it );
            }
        }
    }
    else
    {
        std::copy( it, it + ( M * N ), begin() );
    }
}



template< size_t M, size_t N, typename T >
void
matrix< M, N, T >::zero()
{
    fill( static_cast< T >( 0.0 ) );
}


template< size_t M, size_t N, typename T >
void
matrix< M, N, T >::
operator=( T value_ )
{
    std::fill( begin(), end(), value_ );
}



template< size_t M, size_t N, typename T >
inline matrix< M, N, T > 
matrix< M, N, T >::
operator+( const matrix< M, N, T >& other ) const
{
	matrix< M, N, T > result( *this );
	result += other;
	return result;
}



template< size_t M, size_t N, typename T >
void
matrix< M, N, T >::
operator+=( const matrix< M, N, T >& other )
{
    iterator it = begin(), it_end = end(); 
    const_iterator other_it = other.begin();
    for( ; it != it_end; ++it, ++other_it )
    {
        *it += *other_it;
    }
}



template< size_t M, size_t N, typename T >
inline matrix< M, N, T > 
matrix< M, N, T >::
operator-( const matrix< M, N, T >& other ) const
{
	matrix< M, N, T > result( *this );
	result -= other;
	return result;
}



template< size_t M, size_t N, typename T >
void
matrix< M, N, T >::
operator-=( const matrix< M, N, T >& other )
{
    iterator it = begin(), it_end = end(); 
    const_iterator other_it = other.begin();
    for( ; it != it_end; ++it, ++other_it )
    {
        *it -= *other_it;
    }
}



template< size_t M, size_t N, typename T >
template< size_t O, size_t P, size_t Q, size_t R >
typename enable_if< M == O + Q && N == P + R >::type* 
matrix< M, N, T >::
direct_sum( const matrix< O, P, T >& upper_left,
    const matrix< Q, R, T >& lower_right )
{
    (*this) = static_cast< T >( 0.0 );

    for( size_t row = 0; row < O; ++row )
    {
        for( size_t col = 0; col < P; ++col )
        {
            at( row, col ) = upper_left( row, col );
        }
    }

    for( size_t row = 0; row < Q; ++row )
    {
        for( size_t col = 0; col < R; ++col )
        {
            at( O + row, P + col ) = lower_right( row, col );
        }
    }

    return 0;
}



template< size_t M, size_t N, typename T >
template< size_t O, size_t P >
matrix< O, P, T >
matrix< M, N, T >::
get_sub_matrix( size_t row_offset, size_t col_offset, 
typename enable_if< O <= M && P <= N >::type* ) const
{
	matrix< O, P, T > result;
	get_sub_matrix( result, row_offset, col_offset );
	return result;
}



template< size_t M, size_t N, typename T >
template< size_t O, size_t P >
typename enable_if< O <= M && P <= N >::type*
matrix< M, N, T >::
get_sub_matrix( matrix< O, P, T >& result, 
	size_t row_offset, size_t col_offset ) const
{
    #ifdef VMMLIB_SAFE_ACCESSORS
    if ( O + row_offset > M || P + col_offset > N )
        VMMLIB_ERROR( "index out of bounds.", VMMLIB_HERE );
    #endif

	for( size_t row = 0; row < O; ++row )
	{
		for( size_t col = 0; col < P; ++col )
		{
			result.at( row, col ) 
				= at( row_offset + row, col_offset + col );
		}
	}
    return 0;
}



template< size_t M, size_t N, typename T >
template< size_t O, size_t P >
typename enable_if< O <= M && P <= N >::type*
matrix< M, N, T >::
set_sub_matrix( const matrix< O, P, T >& sub_matrix, 
    size_t row_offset, size_t col_offset )
{
	for( size_t row = 0; row < O; ++row )
	{
		for( size_t col = 0; col < P; ++col )
		{
            at( row_offset + row, col_offset + col ) 
                = sub_matrix.at( row, col );
		}
	}
    return 0; // for sfinae
}



template< size_t M, size_t N, typename T >
inline T
matrix< M, N, T >::det() const
{
    return compute_determinant( *this );
}



template< size_t M, size_t N, typename T >
template< size_t O, size_t P, typename TT >
inline bool
matrix< M, N, T >::inverse( matrix< O, P, TT >& inverse_, T tolerance, 
    typename enable_if< M == N && O == P && O == M && M >= 2 && M <= 4, TT >::type* ) const
{
    return compute_inverse( *this, inverse_, tolerance );
}



template< size_t M, size_t N, typename T >
template< size_t O, size_t P >
typename enable_if< O == P && M == N && O == M && M >= 2 >::type*
matrix< M, N, T >::
get_adjugate( matrix< O, P, T >& adjugate ) const
{
	get_cofactors( adjugate );
	adjugate = transpose( adjugate );
    return 0;
}



template< size_t M, size_t N, typename T >
template< size_t O, size_t P >
typename enable_if< O == P && M == N && O == M && M >= 2 >::type*
matrix< M, N, T >::
get_cofactors( matrix< O, P, T >& cofactors ) const
{
    matrix< M-1, N-1, T >   minor_;

	const size_t _negate = 1u;
	for( size_t row_index = 0; row_index < M; ++row_index )
	{
		for( size_t col_index = 0; col_index < N; ++col_index )
		{
			if ( ( row_index + col_index ) & _negate )
				cofactors( row_index, col_index ) = -get_minor( minor_, row_index, col_index );
			else
				cofactors( row_index, col_index ) = get_minor( minor_, row_index, col_index );
		}
	}
    return 0;
}



template< size_t M, size_t N, typename T >
template< size_t O, size_t P >
T
matrix< M, N, T >::
get_minor( matrix< O, P, T >& minor_, size_t row_to_cut, size_t col_to_cut, 
typename enable_if< O == M-1 && P == N-1 && M == N && M >= 2 >::type* ) const
{
	ssize_t row_offset = 0;
	ssize_t col_offset = 0;
	for( ssize_t row_index = 0; row_index < M; ++row_index )
	{
		if ( row_index == row_to_cut )
			row_offset = -1;
		else
		{
			for( ssize_t col_index = 0; col_index < M; ++col_index )
			{
				if ( col_index == col_to_cut )
					col_offset = -1;
				else
					minor_.at( row_index + row_offset, col_index + col_offset ) 
						= at( row_index, col_index );
			}
			col_offset = 0;
		}
	}
    return compute_determinant( minor_ );
}



template< size_t M, size_t N, typename T >
template< typename TT >
void
matrix< M, N, T >::
rotate( const TT angle_, const vector< M-1, T >& axis, 
typename enable_if< M == N && M == 4, TT >::type* )
{
    const T angle = static_cast< T >( angle_ );

    const T sine      = sin( angle );
    const T cosine    = cos( angle );
    
    // this is necessary since Visual Studio cannot resolve the
    // pow()-call correctly if we just use 2.0 directly.
    // this way, the '2.0' is converted to the same format 
    // as the axis components

	const T _zero = 0.0;
	const T one = 1.0;
    const T two = 2.0;

    array[0]  = cosine + ( one - cosine ) * pow( axis.array[0], two );
    array[1]  = ( one - cosine ) * axis.array[0] * axis.array[1] + sine * axis.array[2];
    array[2]  = ( one - cosine ) * axis.array[0] * axis.array[2] - sine * axis.array[1];
    array[3]  = _zero;
    
    array[4]  = ( one - cosine ) * axis.array[0] * axis.array[1] - sine * axis.array[2];
    array[5]  = cosine + ( one - cosine ) * pow( axis.array[1], two );
    array[6]  = ( one - cosine ) * axis.array[1] *  axis.array[2] + sine * axis.array[0];
    array[7]  = _zero;
    
    array[8]  = ( one - cosine ) * axis.array[0] * axis.array[2] + sine * axis.array[1];
    array[9]  = ( one - cosine ) * axis.array[1] * axis.array[2] - sine * axis.array[0];
    array[10] = cosine + ( one - cosine ) * pow( axis.array[2], two );
    array[11] = _zero;

    array[12] = _zero;
    array[13] = _zero;
    array[14] = _zero;
    array[15] = one;

}



template< size_t M, size_t N, typename T >
template< typename TT >
void
matrix< M, N, T >::
rotate_x( const TT angle_, typename enable_if< M == N && M == 4, TT >::type* )
{
    const T angle       = static_cast< T >( angle_ );

    const T sine        = sin( angle );
    const T cosine      = cos( angle );

    T tmp;

    tmp         = array[ 4 ] * cosine + array[ 8 ] * sine;
    array[ 8 ]  = - array[ 4 ] * sine + array[ 8 ] * cosine;
    array[ 4 ]  = tmp;

    tmp         = array[ 5 ] * cosine + array[ 9 ] * sine;
    array[ 9 ]  = - array[ 5 ] * sine + array[ 9 ] * cosine;
    array[ 5 ]  = tmp;

    tmp         = array[ 6 ] * cosine + array[ 10 ] * sine;
    array[ 10 ] = - array[ 6 ] * sine + array[ 10 ] * cosine;
    array[ 6 ]  = tmp;

    tmp         = array[ 7 ] * cosine + array[ 11 ] * sine;
    array[ 11 ] = - array[ 7 ] * sine + array[ 11 ] * cosine;
    array[ 7 ]  = tmp;
}



template< size_t M, size_t N, typename T >
template< typename TT >
void
matrix< M, N, T >::
rotate_y( const TT angle_, typename enable_if< M == N && M == 4, TT >::type* )
{
    const T angle = static_cast< T >( angle_ );

    const T sine      = sin( angle );
    const T cosine    = cos( angle );

    T tmp;

    tmp         = array[ 0 ] * cosine   - array[ 8 ] * sine;
    array[ 8 ]  = array[ 0 ] * sine     + array[ 8 ] * cosine;
    array[ 0 ]  = tmp;

    tmp         = array[ 1 ] * cosine   - array[ 9 ] * sine;
    array[ 9 ]  = array[ 1 ] * sine     + array[ 9 ] * cosine;
    array[ 1 ]  = tmp;

    tmp         = array[ 2 ] * cosine   - array[ 10 ] * sine;
    array[ 10 ] = array[ 2 ] * sine     + array[ 10 ] * cosine;
    array[ 2 ]  = tmp;

    tmp         = array[ 3 ] * cosine   - array[ 11 ] * sine;
    array[ 11 ] = array[ 3 ] * sine     + array[ 11 ] * cosine;
    array[ 3 ]  = tmp;
}



template< size_t M, size_t N, typename T >
template< typename TT >
void
matrix< M, N, T >::
rotate_z( const TT angle_, typename enable_if< M == N && M == 4, TT >::type* )
{
    const T angle = static_cast< T >( angle_ );
    
    const T sine      = sin( angle );
    const T cosine    = cos( angle );

    T tmp;

    tmp         = array[ 0 ] * cosine + array[ 4 ] * sine;
    array[ 4 ]  = - array[ 0 ] * sine + array[ 4 ] * cosine;
    array[ 0 ]  = tmp;
    
    tmp         = array[ 1 ] * cosine + array[ 5 ] * sine;
    array[ 5 ]  = - array[ 1 ] * sine + array[ 5 ] * cosine;
    array[ 1 ]  = tmp;

    tmp         = array[ 2 ] * cosine + array[ 6 ] * sine;
    array[ 6 ]  = - array[ 2 ] * sine + array[ 6 ] * cosine;
    array[ 2 ]  = tmp;

    tmp         = array[ 3 ] * cosine + array[ 7 ] * sine;
    array[ 7 ]  = - array[ 3 ] * sine + array[ 7 ] * cosine;
    array[ 3 ]  = tmp;
}



template< size_t M, size_t N, typename T >
template< typename TT >
void
matrix< M, N, T >::
pre_rotate_x( const TT angle_, 
              typename enable_if< M == N && M == 4, TT >::type* )
{
    const T angle = static_cast< T >( angle_ );

    const T sine      = sin( angle );
    const T cosine    = cos( angle );

    T tmp;
    
    tmp         = array[ 1 ];
    array[ 1 ]  = array[ 1 ] * cosine + array[ 2 ] * sine;
    array[ 2 ]  = tmp * -sine + array[ 2 ] * cosine;

    tmp         = array[ 5 ];
    array[ 5 ]  = array[ 5 ] * cosine + array[ 6 ] * sine;
    array[ 6 ]  = tmp * -sine + array[ 6 ] * cosine;

    tmp         = array[ 9 ];
    array[ 9 ]  = array[ 9 ] * cosine + array[ 10 ] * sine;
    array[ 10 ] = tmp * -sine + array[ 10 ] * cosine;

    tmp         = array[ 13 ];
    array[ 13 ] = array[ 13 ] * cosine + array[ 14 ] * sine;
    array[ 14 ] = tmp * -sine + array[ 14 ] * cosine;
}



template< size_t M, size_t N, typename T >
template< typename TT >
void
matrix< M, N, T >::
pre_rotate_y( const TT angle_, typename enable_if< M == N && M == 4, TT >::type* )
{
    const T angle = static_cast< T >( angle_ );

    const T sine      = sin( angle );
    const T cosine    = cos( angle );

    T tmp;

    tmp         = array[ 0 ];
    array[ 0 ]  = array[ 0 ] * cosine - array[ 2 ] * sine;
    array[ 2 ]  = tmp * sine + array[ 2 ] * cosine;

    tmp         = array[ 4 ];
    array[ 4 ]  = array[ 4 ] * cosine - array[ 6 ] * sine;
    array[ 6 ]  = tmp * sine + array[ 6 ] * cosine;

    tmp         = array[ 8 ];
    array[ 8 ]  = array[ 8 ] * cosine - array[ 10 ] * sine;
    array[ 10 ] = tmp * sine + array[ 10 ] * cosine;

    tmp         = array[ 12 ];
    array[ 12 ] = array[ 12 ] * cosine - array[ 14 ] * sine;
    array[ 14 ] = tmp * sine + array[ 14 ] * cosine;
}



template< size_t M, size_t N, typename T >
template< typename TT >
void
matrix< M, N, T >::
pre_rotate_z( const TT angle_, typename enable_if< M == N && M == 4, TT >::type* )
{
    const T angle = static_cast< T >( angle_ );

    const T sine      = sin( angle );
    const T cosine    = cos( angle );

    T tmp;
    
    tmp         = array[ 0 ];
    array[ 0 ]  = array[ 0 ] * cosine + array[ 1 ] * sine;
    array[ 1 ]  = tmp * -sine + array[ 1 ] * cosine;

    tmp         = array[ 4 ];
    array[ 4 ]  = array[ 4 ] * cosine + array[ 5 ] * sine;
    array[ 5 ]  = tmp * -sine + array[ 5 ] * cosine;

    tmp         = array[ 8 ];
    array[ 8 ]  = array[ 8 ] * cosine + array[ 9 ] * sine;
    array[ 9 ]  = tmp * -sine + array[ 9 ] * cosine;

    tmp         = array[ 12 ];
    array[ 12 ] = array[ 12 ] * cosine + array[ 13 ] * sine;
    array[ 13 ] = tmp * -sine + array[ 13 ] * cosine;

}



template< size_t M, size_t N, typename T >
template< typename TT >
void
matrix< M, N, T >::
scale( const TT _scale[3], 
    typename enable_if< M == N && M == 4, TT >::type* )
{
    const T scale0 = static_cast< T >( _scale[ 0 ] );
    const T scale1 = static_cast< T >( _scale[ 1 ] );
    const T scale2 = static_cast< T >( _scale[ 2 ] );

    array[0]  *= scale0;
    array[1]  *= scale0;
    array[2]  *= scale0;
    array[3]  *= scale0;
    array[4]  *= scale1;
    array[5]  *= scale1;
    array[6]  *= scale1;
    array[7]  *= scale1;
    array[8]  *= scale2;
    array[9]  *= scale2;
    array[10] *= scale2;
    array[11] *= scale2;

}



template< size_t M, size_t N, typename T >
template< typename TT >
void
matrix< M, N, T >::
scale( const TT x_, const T y_, const T z_, 
    typename enable_if< M == N && M == 4, TT >::type* )
{
    const T _x = static_cast< T >( x_ );

    array[0]  *= _x;
    array[1]  *= _x;
    array[2]  *= _x;
    array[3]  *= _x;
    array[4]  *= y_;
    array[5]  *= y_;
    array[6]  *= y_;
    array[7]  *= y_;
    array[8]  *= z_;
    array[9]  *= z_;
    array[10] *= z_;
    array[11] *= z_;

}



template< size_t M, size_t N, typename T >
template< typename TT >
inline void
matrix< M, N, T >::
scale( const vector< 3, TT >& scale_, 
    typename enable_if< M == N && M == 4, TT >::type* )
{
    scale( scale_.array );
}



template< size_t M, size_t N, typename T >
template< typename TT >
void
matrix< M, N, T >::
scale_translation( const TT scale_[3], 
    typename enable_if< M == N && M == 4, TT >::type* )
{
    array[12] *= static_cast< T >( scale_[0] );
    array[13] *= static_cast< T >( scale_[1] );
    array[14] *= static_cast< T >( scale_[2] );
}



template< size_t M, size_t N, typename T >
template< typename TT >
inline void
matrix< M, N, T >::
scale_translation( const vector< 3, TT >& scale_, 
    typename enable_if< M == N && M == 4, TT >::type* )
{
    scale_translation( scale_.array );
}



template< size_t M, size_t N, typename T >
template< typename TT >
inline void 
matrix< M, N, T >::
set_translation( const TT x_, const TT y_, const TT z_, 
  typename enable_if< M == N && M == 4, TT >::type* )
{
    array[12] = static_cast< T >( x_ );
    array[13] = static_cast< T >( y_ );
    array[14] = static_cast< T >( z_ );
}



template< size_t M, size_t N, typename T >
template< typename TT >
inline void
matrix< M, N, T >::
set_translation( const TT trans[3], 
    typename enable_if< M == N && M == 4, TT >::type* )
{
    array[12] = static_cast< T >( trans[ 0 ] );
    array[13] = static_cast< T >( trans[ 1 ] );
    array[14] = static_cast< T >( trans[ 2 ] );
}



template< size_t M, size_t N, typename T >
template< typename TT >
inline void
matrix< M, N, T >::
set_translation( const vector< 3, TT >& translation_, 
    typename enable_if< M == N && M == 4, TT >::type* )
{
    set_translation( translation_.array );
}



template< size_t M, size_t N, typename T >
template< typename TT >
inline void
matrix< M, N, T >::
get_translation( vector< 3, TT >& translation_, 
    typename enable_if< M == N && M == 4, TT >::type* ) const
{
    translation_.array[ 0 ] = array[ 12 ];
    translation_.array[ 1 ] = array[ 13 ];
    translation_.array[ 2 ] = array[ 14 ];
}



template< size_t M, size_t N, typename T >
size_t
matrix< M, N, T >::
size() const
{
    return M * N;
}



template< size_t M, size_t N, typename T >
typename matrix< M, N, T >::iterator
matrix< M, N, T >::
begin()
{
    return array;
}



template< size_t M, size_t N, typename T >
typename matrix< M, N, T >::iterator
matrix< M, N, T >::
end()
{
    return array + size();
}



template< size_t M, size_t N, typename T >
typename matrix< M, N, T >::const_iterator
matrix< M, N, T >::
begin() const
{
    return array;
}



template< size_t M, size_t N, typename T >
typename matrix< M, N, T >::const_iterator
matrix< M, N, T >::
end() const
{
    return array + size();
}



template< size_t M, size_t N, typename T >
typename matrix< M, N, T >::reverse_iterator
matrix< M, N, T >::
rbegin()
{
    return array + size() - 1;
}



template< size_t M, size_t N, typename T >
typename matrix< M, N, T >::reverse_iterator
matrix< M, N, T >::
rend()
{
    return array - 1;
}



template< size_t M, size_t N, typename T >
typename matrix< M, N, T >::const_reverse_iterator
matrix< M, N, T >::
rbegin() const
{
    return array + size() - 1;
}



template< size_t M, size_t N, typename T >
typename matrix< M, N, T >::const_reverse_iterator
matrix< M, N, T >::
rend() const
{
    return array - 1;
}



template< size_t M, size_t N, typename T >
template< typename init_functor_t >
const matrix< M, N, T >
matrix< M, N, T >::get_initialized_matrix()
{
	matrix< M, N, T > matrix_;
	init_functor_t()( matrix_ );
	return matrix_;
}


// it's ugly, but it allows having properly initialized static members
// without having to worry about static initialization order.
template< size_t M, size_t N, typename T >
const matrix< M, N, T > 
matrix< M, N, T >::IDENTITY(
    matrix< M, N, T >::
        get_initialized_matrix< set_to_identity_functor< matrix< M, N, T > > >()
    );


template< size_t M, size_t N, typename T >
const matrix< M, N, T > 
matrix< M, N, T >::ZERO(
    matrix< M, N, T >::
        get_initialized_matrix< set_to_zero_functor< matrix< M, N, T > > >() 
    );



} // namespace vmml

#endif

