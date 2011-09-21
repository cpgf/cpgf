/* 
 * VMMLib - Tensor Classes
 *  
 * @author Susanne Suter
 * @author Jonas Boesch
 * 
 * a tensor is a generalization of a multidimensional array
 * a tensor3 is a tensor data structure with three modes I1, I2 and I3
 */
 
#ifndef __VMML__TENSOR3__HPP__
#define __VMML__TENSOR3__HPP__

#include <vmmlib/matrix.hpp>

namespace vmml
{
	
// tensor with three modes, containing a series I3 of I1 x I2 vmml matrices
//I1 is number of rows, I2 is number of columns and I3 is number of tubes
template< size_t I1, size_t I2, size_t I3, typename T = float >
class tensor3
{
public:
    typedef T                                       value_type;
	typedef T*                                      pointer;
	typedef T&                                      reference;
    typedef T*                                      iterator;
    typedef const T*                                const_iterator;
    typedef std::reverse_iterator< iterator >       reverse_iterator;
    typedef std::reverse_iterator< const_iterator > const_reverse_iterator;
    
    typedef typename vmml::matrix< I1, I2, T >        slice_type_frontal;
    
 
    // accessors
    inline T& operator()( size_t i1, size_t i2, size_t i3 );
    inline const T& operator()( size_t i1, size_t i2, size_t i3 ) const;

    inline T& at( size_t i1, size_t i2, size_t i3 );
    inline const T& at( size_t i1, size_t i2, size_t i3 ) const;
    
    // element iterators - NOTE: column-major order
    iterator                begin();
    iterator                end();
    const_iterator          begin() const;
    const_iterator          end() const;
	
    reverse_iterator        rbegin();
    reverse_iterator        rend();
    const_reverse_iterator  rbegin() const;
    const_reverse_iterator  rend() const;
	
	// hack for static-member-init
	template< typename init_functor_t >
	static const tensor3 get_initialized_tensor3();
	
    // ctors
    // note: this ctor does not initialize anything because of performance reasons.
    tensor3();
    
    template< typename U >
    tensor3( const tensor3< I1, I2, I3, U >& source_ );
	
    template< size_t J1, size_t J2, size_t J3>
    tensor3( const tensor3< J1, J2, J3, T >& source_ );

    size_t size() const; // return I1 * I2 * I3;   
	
    inline void get_I1_vector( size_t i2, size_t i3, vmml::vector< I1, T >& data ) const; // I1_vector is a column vector with all values i1 at i2 and i3
	inline void get_I2_vector( size_t i1, size_t i3, vmml::vector< I2, T >& data ) const; // I2_vector is a row vector with all values i2 at i1 and i3
    inline void get_I3_vector( size_t i1, size_t i2, vmml::vector< I3, T >& data ) const; // I3_vector is a vector with all values i3 at a given i1 and i2

	inline void get_row( size_t i1, size_t i3, vmml::vector< I2, T >& data ) const; // same as get_I2_vector
    inline void get_column( size_t i2, size_t i3, vmml::vector< I1, T >& data ) const; // same as get_I1_vector
    inline void get_tube( size_t i1, size_t i2, vmml::vector< I3, T >& data ) const; // same as get_I3_vector
	
    inline void set_I1_vector( size_t i2, size_t i3, const vmml::vector< I1, T >& data ); // I1_vector is a column vector with all values i1 at i2 and i3
	inline void set_I2_vector( size_t i1, size_t i3, const vmml::vector< I2, T >& data ); // I2_vector is a row vector with all values i2 at i1 and i3
    inline void set_I3_vector( size_t i1, size_t i2, const vmml::vector< I3, T >& data ); // I3_vector is a vector with all values i3 at a given i1 and i2

	inline void set_row( size_t i1, size_t i3, const vmml::vector< I2, T >& data ); // same as set_I2_vector
    inline void set_column( size_t i2, size_t i3, const vmml::vector< I1, T >& data ); // same as set_I1_vector
    inline void set_tube( size_t i1, size_t i2, const vmml::vector< I3, T >& data ); // same as set_I3_vector

	inline void get_frontal_slice( size_t i3, vmml::matrix< I1, I2, T >& data ) const;  
	inline void get_lateral_slice( size_t i2, vmml::matrix< I1, I3, T >& data ) const; 
	inline void get_horizontal_slice( size_t i1, vmml::matrix< I2, I3, T >& data ) const;
	
	inline void set_frontal_slice( size_t i3, const vmml::matrix< I1, I2, T >& data ); 
	inline void set_lateral_slice( size_t i2, const vmml::matrix< I1, I3, T >& data ); 
	inline void set_horizontal_slice( size_t i1, const vmml::matrix< I2, I3, T >& data );
	
	
	// sets all elements to fill_value
    void operator=( T fill_value ); //@SUS: todo
    void fill( T fill_value ); //special case of set method (all values are set to the same value!)
    void fill_random( );
    void fill_random_signed( );
    void fill_increasing_values( );
	
	void RangeThreshold(tensor3<I1, I2, I3, T>& other_, const T& start_value, const T& end_value) const;
    
    // note: this function copies elements until either the matrix is full or
    // the iterator equals end_.
    template< typename input_iterator_t >
    void set( input_iterator_t begin_, input_iterator_t end_, 
			 bool row_major_layout = true );	
    void zero();

    bool operator==( const tensor3& other ) const;
    bool operator!=( const tensor3& other ) const;
	
	// due to limited precision, two 'idential' tensor3 might seem different.
    // this function allows to specify a tolerance when comparing matrices.
    bool equals( const tensor3& other, T tolerance ) const;
    // this version takes a comparison functor to compare the components of
    // the two tensor3 data structures
    template< typename compare_t >
    bool equals( const tensor3& other, compare_t& cmp ) const;
	 
	
    //tensor times matrix multiplication along different modes
    template< size_t J1, size_t J2, size_t J3 > 
    void multiply_horizontal( const tensor3< J1, J2, J3, T >& core, const matrix< I3, J3, T >& U3 ); //output: tensor3< J1, J2, I3, T >  
	
    template< size_t J1, size_t J2, size_t J3 > 
    void multiply_lateral( const tensor3< J1, J2, J3, T >& core, const matrix< I1, J1, T >& U1 ); //output: tensor3< I1, J2, J3, T > 
	
    template< size_t J1, size_t J2, size_t J3 > 
    void multiply_frontal( const tensor3< J1, J2, J3, T >& core, const matrix< I2, J2, T >& U2 ); //output: tensor3< J1, I2, J3, T >
	
    template< size_t J1, size_t J2, size_t J3 > 
	void full_tensor3_matrix_multiplication( const tensor3< J1, J2, J3, T >& core, const matrix< I1, J1, T >& U1, const matrix< I2, J2, T >& U2, const matrix< I3, J3, T >& U3 );
	
	void horizontal_matricization( matrix< I3, I1*I2, T>& matricization) const;
	void lateral_matricization( matrix< I1, I2*I3, T>& matricization) const;
	void frontal_matricization( matrix< I2, I1*I3, T>& matricization) const;
		
	
 	inline tensor3 operator+( T scalar ) const;
	inline tensor3 operator-( T scalar ) const;
	
	void operator+=( T scalar );
	void operator-=( T scalar );
	
	inline tensor3 operator+( const tensor3& other ) const;
	inline tensor3 operator-( const tensor3& other ) const;
	
	void operator+=( const tensor3& other );
	void operator-=( const tensor3& other );

	// 
	// tensor3-scalar operations / scaling 
	// 
    tensor3 operator*( T scalar );
    void operator*=( T scalar );
	
	//
	// matrix-vector operations
    //
	// transform column vector by matrix ( vec = matrix * vec )
    vector< I1, T > operator*( const vector< I2, T >& other ) const;
	
	// transform column vector by matrix ( vec = matrix * vec )
	// assume homogenous coords, e.g. vec3 = mat4x4 * vec3, with w = 1.0
    template< size_t O >
    vector< O, T > operator*( const vector< O, T >& vector_ ) const;
    
    inline tensor3< I1, I2, I3, T > operator-() const;
    tensor3< I1, I2, I3, T > negate() const;
	
    friend std::ostream& operator << ( std::ostream& os, const tensor3< I1, I2, I3, T >& t3 )
	{ 
		for(size_t i = 0; i < I3; ++i)
		{
			os << t3.array[ i ] << "***" << std::endl;			
		}	
		return os;
	}
	
	
	// static members
    static const tensor3< I1, I2, I3, T > ZERO;


protected:
    slice_type_frontal                                      array[ I3 ];
	

	


}; // class tensor3

#define VMML_TEMPLATE_STRING    template< size_t I1, size_t I2, size_t I3, typename T >
#define VMML_TEMPLATE_CLASSNAME tensor3< I1, I2, I3, T >

VMML_TEMPLATE_STRING
inline T&
VMML_TEMPLATE_CLASSNAME::at( size_t i1, size_t i2, size_t i3 )
{
    #ifdef VMMLIB_SAFE_ACCESSORS
    if ( i3 >= I3 )
        VMMLIB_ERROR( "at( i1, i2, i3 ) - index out of bounds", VMMLIB_HERE );
    #endif
    return array[ i3 ].at( i1, i2 );
}



VMML_TEMPLATE_STRING
const inline T&
VMML_TEMPLATE_CLASSNAME::at( size_t i1, size_t i2, size_t i3 ) const
{
    #ifdef VMMLIB_SAFE_ACCESSORS
    if ( i3 >= I3 )
        VMMLIB_ERROR( "at( i1, i2, i3 ) - i3 index out of bounds", VMMLIB_HERE );
    #endif
    return array[ i3 ].at( i1, i2 );
}



VMML_TEMPLATE_STRING
inline T&
VMML_TEMPLATE_CLASSNAME::operator()( size_t i1, size_t i2, size_t i3 )
{
    return at( i1, i2, i3 );
}



VMML_TEMPLATE_STRING
const inline T&
VMML_TEMPLATE_CLASSNAME::operator()(  size_t i1, size_t i2, size_t i3 ) const
{
    return at( i1, i2, i3 );
}


VMML_TEMPLATE_STRING
inline void 
VMML_TEMPLATE_CLASSNAME::
get_I2_vector( size_t i1, size_t i3, vmml::vector< I2, T >& data ) const
{
#ifdef VMMLIB_SAFE_ACCESSORS
	
    if ( i3 >= I3 )
        VMMLIB_ERROR( "get_I1_vector() - i3 index out of bounds.", VMMLIB_HERE );
	
#endif
    
	array[ i3 ].get_row( i1, data );	
}

VMML_TEMPLATE_STRING
inline void 
VMML_TEMPLATE_CLASSNAME::
get_I1_vector( size_t i2, size_t i3, vmml::vector< I1, T >& data ) const
{
#ifdef VMMLIB_SAFE_ACCESSORS
	
    if ( i3 >= I3 )
        VMMLIB_ERROR( "get_I2_vector() - i3 index out of bounds.", VMMLIB_HERE );
	
#endif
    
	array[ i3 ].get_column( i2, data );	
	
}

VMML_TEMPLATE_STRING
inline void 
VMML_TEMPLATE_CLASSNAME::
get_I3_vector( size_t i1, size_t i2, vmml::vector< I3, T >& data ) const
{
	for (size_t i3 = 0; i3 < I3; ++i3)
	{
		data[ i3 ] = array[ i3 ].at( i1, i2 );		
	}

}


VMML_TEMPLATE_STRING
inline void 
VMML_TEMPLATE_CLASSNAME::
get_row( size_t i1, size_t i3, vmml::vector< I2, T >& data ) const
{
    get_I2_vector( i1, i3, data );
}

VMML_TEMPLATE_STRING
inline void 
VMML_TEMPLATE_CLASSNAME::
get_column( size_t i2, size_t i3, vmml::vector< I1, T >& data ) const
{
    get_I1_vector( i2, i3, data );
}

VMML_TEMPLATE_STRING
inline void 
VMML_TEMPLATE_CLASSNAME::
get_tube( size_t i1, size_t i2, vmml::vector< I3, T >& data ) const
{
	get_I3_vector( i1, i2, data );
}


VMML_TEMPLATE_STRING
inline void 
VMML_TEMPLATE_CLASSNAME::
set_I2_vector( size_t i1, size_t i3, const vmml::vector< I2, T >& data )
{
#ifdef VMMLIB_SAFE_ACCESSORS
	
    if ( i3 >= I3 )
        VMMLIB_ERROR( "set_I1_vector() - i3 index out of bounds.", VMMLIB_HERE );
	
#endif
    
	array[ i3 ].set_row( i1, data );	//@SUS: bug fix
}


VMML_TEMPLATE_STRING
inline void 
VMML_TEMPLATE_CLASSNAME::
set_I1_vector( size_t i2, size_t i3, const vmml::vector< I1, T >& data )
{
#ifdef VMMLIB_SAFE_ACCESSORS
	
    if ( i3 >= I3 )
        VMMLIB_ERROR( "set_I2_vector() - i3 index out of bounds.", VMMLIB_HERE );
	
#endif
    
	array[ i3 ].set_column( i2, data );	
	
}

VMML_TEMPLATE_STRING
inline void 
VMML_TEMPLATE_CLASSNAME::
set_I3_vector( size_t i1, size_t i2, const vmml::vector< I3, T >& data )
{
	for (size_t i3 = 0; i3 < I3; ++i3)
	{
		array[ i3 ].at( i1, i2 ) = data[ i3 ];
	}
	
}

VMML_TEMPLATE_STRING
inline void 
VMML_TEMPLATE_CLASSNAME::
set_row( size_t i1, size_t i3, const vmml::vector< I2, T >& data )
{
	set_I2_vector( i1, i3, data );
}


VMML_TEMPLATE_STRING
inline void 
VMML_TEMPLATE_CLASSNAME::
set_column( size_t i2, size_t i3, const vmml::vector< I1, T >& data )
{
	set_I1_vector( i2, i3, data );
}

VMML_TEMPLATE_STRING
inline void 
VMML_TEMPLATE_CLASSNAME::
set_tube( size_t i1, size_t i2, const vmml::vector< I3, T >& data )
{
	set_I3_vector( i1, i2, data );
}


VMML_TEMPLATE_STRING
inline void 
VMML_TEMPLATE_CLASSNAME::
get_frontal_slice( size_t i3, vmml::matrix< I1, I2, T >& data ) const
{
#ifdef VMMLIB_SAFE_ACCESSORS
    if ( i3 >= I3 )
        VMMLIB_ERROR( "get_frontal_slice() - index out of bounds.", VMMLIB_HERE );
#endif
	
	data = array[ i3 ];
}


VMML_TEMPLATE_STRING
inline void 
VMML_TEMPLATE_CLASSNAME::
get_lateral_slice( size_t i2, vmml::matrix< I1, I3, T >& data ) const
{
#ifdef VMMLIB_SAFE_ACCESSORS
    if ( i2 >= I2 )
        VMMLIB_ERROR( "get_lateral_slice() - index out of bounds.", VMMLIB_HERE );
#endif
	for( size_t i3 = 0; i3 < I3; ++i3 )
	{
		data.set_column( i3, array[ i3 ].get_column( i2 ));
	}
}

VMML_TEMPLATE_STRING
inline void 
VMML_TEMPLATE_CLASSNAME::
get_horizontal_slice( size_t i1, vmml::matrix< I2, I3, T >& data ) const
{
#ifdef VMMLIB_SAFE_ACCESSORS
    if ( i1 >= I1 )
        VMMLIB_ERROR( "get_horizontal_slice() - index out of bounds.", VMMLIB_HERE );
#endif
	for( size_t i3 = 0; i3 < I3; ++i3 )
	{
		data.set_column( i3, array[ i3 ].get_row( i1 )); //or for every i2 get/set column
	}
}


VMML_TEMPLATE_STRING
inline void 
VMML_TEMPLATE_CLASSNAME::
set_frontal_slice( size_t i3, const vmml::matrix< I1, I2, T >& data )
{
#ifdef VMMLIB_SAFE_ACCESSORS
    if ( i3 >= I3 )
        VMMLIB_ERROR( "set_frontal_slice() - index out of bounds.", VMMLIB_HERE );
#endif
	
	array[ i3 ] = data;
}

VMML_TEMPLATE_STRING
inline void 
VMML_TEMPLATE_CLASSNAME::
set_lateral_slice( size_t i2, const vmml::matrix< I1, I3, T >& data )
{
#ifdef VMMLIB_SAFE_ACCESSORS
    if ( i2 >= I2 )
        VMMLIB_ERROR( "set_lateral_slice() - index out of bounds.", VMMLIB_HERE );
#endif
	
	for( size_t i3 = 0; i3 < I3; ++i3 )
	{
		array[ i3 ].set_column(i2, data.get_column( i3 ));
	}
}

VMML_TEMPLATE_STRING
inline void 
VMML_TEMPLATE_CLASSNAME::
set_horizontal_slice( size_t i1, const vmml::matrix< I2, I3, T >& data )
{
#ifdef VMMLIB_SAFE_ACCESSORS
    if ( i1 >= I1 )
        VMMLIB_ERROR( "set_horizontal_slice() - index out of bounds.", VMMLIB_HERE );
#endif
	
	for( size_t i3 = 0; i3 < I3; ++i3 )
	{
		array[ i3 ].set_row( i1, data.get_column( i3 ));
	}
	
}


VMML_TEMPLATE_STRING
void
VMML_TEMPLATE_CLASSNAME::
fill( T fillValue )
{
	for( size_t i3 = 0; i3 < I3; ++i3 )
	{
		array[ i3 ].fill( fillValue );
	}
}


VMML_TEMPLATE_STRING
void
VMML_TEMPLATE_CLASSNAME::
fill_random( )
{
	double fillValue = 0.0f;
	srand(time(NULL));
	for( size_t i3 = 0; i3 < I3; ++i3 )
	{
		for( size_t i1 = 0; i1 < I1; ++i1 )
		{
			for( size_t i2 = 0; i2 < I2; ++i2 )
			{
				fillValue = rand();
				fillValue /= RAND_MAX;
				fillValue *= std::numeric_limits< T >::max();
				at( i1, i2, i3 ) = static_cast< T >( fillValue )  ;
			}
		}
	}
}

VMML_TEMPLATE_STRING
void
VMML_TEMPLATE_CLASSNAME::
fill_random_signed( )
{
	double fillValue = 0.0f;
	srand(time(NULL));
	for( size_t i3 = 0; i3 < I3; ++i3 )
	{
		for( size_t i1 = 0; i1 < I1; ++i1 )
		{
			for( size_t i2 = 0; i2 < I2; ++i2 )
			{
				fillValue = rand();
				fillValue /= RAND_MAX;
				fillValue *= std::numeric_limits< T >::max();
				T fillValue2 = static_cast< T >(fillValue) % std::numeric_limits< T >::max();
				fillValue2 -= std::numeric_limits< T >::max()/2;
				at( i1, i2, i3 ) = fillValue2  ;
			}
		}
	}
}


VMML_TEMPLATE_STRING
void
VMML_TEMPLATE_CLASSNAME::
fill_increasing_values( )
{
	double fillValue = 0.0f;
	for( size_t i3 = 0; i3 < I3; ++i3 )
	{
		for( size_t i1 = 0; i1 < I1; ++i1 )
		{
			for( size_t i2 = 0; i2 < I2; ++i2 )
			{
				at( i1, i2, i3 ) = static_cast< T >( fillValue );
				fillValue++;
			}
		}
	}
}

VMML_TEMPLATE_STRING
void 
VMML_TEMPLATE_CLASSNAME::RangeThreshold(tensor3<I1, I2, I3, T>& other_, const T& start_value, const T& end_value) const
{
	
	for( size_t i3 = 0; i3 < I3; ++i3 )
	{
		for( size_t i1 = 0; i1 < I1; ++i1 )
		{
			for( size_t i2 = 0; i2 < I2; ++i2 )
			{
				T value = at( i1, i2, i3 );
				if (value >= start_value && value <= end_value)
					other_.at(i1, i2, i3 ) = static_cast< T >( value );
			}
		}
	}
}


VMML_TEMPLATE_STRING
void 
VMML_TEMPLATE_CLASSNAME::zero()
{
    fill( static_cast< T >( 0.0 ) );
}


VMML_TEMPLATE_STRING
bool
VMML_TEMPLATE_CLASSNAME::operator==( const tensor3< I1, I2, I3, T >& other ) const
{
    bool ok = true;
    for( size_t i3 = 0; ok && i3 < I3; ++i3 )
	{
        ok = array[ i3 ] == other.array[ i3 ];
	}
	
    return ok;
}


VMML_TEMPLATE_STRING
bool
VMML_TEMPLATE_CLASSNAME::operator!=( const tensor3< I1, I2, I3, T >& other ) const
{
    return ! operator==( other );
}


VMML_TEMPLATE_STRING
bool equals( const tensor3< I1, I2, I3, T >& t3_0, const tensor3< I1, I2, I3, T >& t3_1, T tolerance )
{
    return t3_0.equals( t3_1, tolerance );
}

VMML_TEMPLATE_STRING
bool
VMML_TEMPLATE_CLASSNAME::equals( const tensor3< I1, I2, I3, T >& other, T tolerance ) const
{
    bool ok = true;
	for (size_t i3 = 0; ok && i3 < I3; ++i3 ) 
	{
		ok = array[ i3 ].equals(other.array[ i3 ], tolerance);
	}
    return ok;	
}





VMML_TEMPLATE_STRING
size_t
VMML_TEMPLATE_CLASSNAME::size() const
{
    return I1 * I2 * I3;
}



VMML_TEMPLATE_STRING
typename VMML_TEMPLATE_CLASSNAME::iterator
VMML_TEMPLATE_CLASSNAME::begin()
{
    return array[0].array;
}




VMML_TEMPLATE_STRING
typename VMML_TEMPLATE_CLASSNAME::iterator
VMML_TEMPLATE_CLASSNAME::end()
{
    return array[ I3 ].array + array[ I3 ].size();
}



VMML_TEMPLATE_STRING
typename VMML_TEMPLATE_CLASSNAME::const_iterator
VMML_TEMPLATE_CLASSNAME::begin() const
{
    return array[0].array;
}



VMML_TEMPLATE_STRING
typename VMML_TEMPLATE_CLASSNAME::const_iterator
VMML_TEMPLATE_CLASSNAME::end() const
{
    return array[ I3 ].array + array[I3 ].size();
}



VMML_TEMPLATE_STRING
typename VMML_TEMPLATE_CLASSNAME::reverse_iterator
VMML_TEMPLATE_CLASSNAME::rbegin()
{
    return array[ I3 ].array + array[ I3 ].size() -1;
}



VMML_TEMPLATE_STRING
typename VMML_TEMPLATE_CLASSNAME::reverse_iterator
VMML_TEMPLATE_CLASSNAME::rend()
{
    return array[ I3 ].array - 1;
}



VMML_TEMPLATE_STRING
typename VMML_TEMPLATE_CLASSNAME::const_reverse_iterator
VMML_TEMPLATE_CLASSNAME::rbegin() const
{
    return array[ I3 ].array + array[ I3 ].size() -1;
}



VMML_TEMPLATE_STRING
typename VMML_TEMPLATE_CLASSNAME::const_reverse_iterator
VMML_TEMPLATE_CLASSNAME::rend() const
{
    return array[ I3 ].array -1;
}


VMML_TEMPLATE_STRING
template< typename input_iterator_t >
void 
VMML_TEMPLATE_CLASSNAME::set( input_iterator_t begin_, input_iterator_t end_, bool row_major_layout )
{
	input_iterator_t it( begin_ );
    if( row_major_layout )
    {
        for ( size_t i3 = 0; i3 < I3; ++i3 )
		{
			for( size_t i1 = 0; i1 < I1; ++i1 )
			{
				for( size_t i2 = 0; i2 < I2; ++i2, ++it )
				{
					if ( it == end_ )
						return;
					at( i1, i2, i3 ) = static_cast< T >( *it );
				}
			}
		}
    }
    else
    {
        std::copy( it, it + ( I1 * I2 * I3 ), begin() );
    }
}



VMML_TEMPLATE_STRING
template< typename init_functor_t >
const VMML_TEMPLATE_CLASSNAME
VMML_TEMPLATE_CLASSNAME::get_initialized_tensor3()
{
	tensor3< I1, I2, I3, T > tensor3_;
	init_functor_t()( tensor3_ );
	return tensor3_;
}


VMML_TEMPLATE_STRING
VMML_TEMPLATE_CLASSNAME::tensor3()
{
    // no initialization for performance reasons.
}


VMML_TEMPLATE_STRING
template< typename U >
VMML_TEMPLATE_CLASSNAME::tensor3( const tensor3< I1, I2, I3, U >& source_ )
{
    (*this) = source_;
}

VMML_TEMPLATE_STRING
template< size_t J1, size_t J2, size_t J3 >
VMML_TEMPLATE_CLASSNAME::tensor3( const tensor3< J1, J2, J3, T >& source_ )
{
	const size_t minL =  J1 < I1 ? J1 : I1;
	const size_t minC =  J2 < I2 ? J2 : I2;
	const size_t minS =  J3 < I3 ? J3 : I3;
	
	//(*this) = ZERO;
	zero();
	
	for ( size_t i = 0 ; i < minL ; i++ )
		for ( size_t j = 0 ; j < minC ; j++ )
			for ( size_t k = 0 ; k < minS ; k++ )
		{
			at( i,j, k ) = source_( i, j, k ); 
		}
}



VMML_TEMPLATE_STRING
inline VMML_TEMPLATE_CLASSNAME 
VMML_TEMPLATE_CLASSNAME::operator+( const tensor3< I1, I2, I3, T >& other ) const
{
	tensor3< I1, I2, I3, T > result( *this );
	result += other;
	return result;
}



VMML_TEMPLATE_STRING
void
VMML_TEMPLATE_CLASSNAME::operator+=( const tensor3< I1, I2, I3, T >& other )
{
    iterator it = begin(), it_end = end(); 
    const_iterator other_it = other.begin();
    for( ; it != it_end; ++it, ++other_it )
    {
        *it += *other_it;
    }
}



VMML_TEMPLATE_STRING
inline VMML_TEMPLATE_CLASSNAME 
VMML_TEMPLATE_CLASSNAME::operator-( const tensor3< I1, I2, I3, T >& other ) const
{
	tensor3< I1, I2, I3, T > result( *this );
	result -= other;
	return result;
}



VMML_TEMPLATE_STRING
void
VMML_TEMPLATE_CLASSNAME::operator-=( const tensor3< I1, I2, I3, T >& other )
{
    iterator it = begin(), it_end = end(); 
    const_iterator other_it = other.begin();
    for( ; it != it_end; ++it, ++other_it )
    {
        *it -= *other_it;
    }
}


//sum with scalar


VMML_TEMPLATE_STRING
inline VMML_TEMPLATE_CLASSNAME 
VMML_TEMPLATE_CLASSNAME::operator+( T scalar ) const
{
	tensor3< I1, I2, I3, T > result( *this );
	result += scalar;
	return result;
}



VMML_TEMPLATE_STRING
void
VMML_TEMPLATE_CLASSNAME::operator+=( T scalar )
{
    iterator it = begin(), it_end = end(); 
    for( ; it != it_end; ++it )
    {
        *it += scalar;
    }
}



VMML_TEMPLATE_STRING
inline VMML_TEMPLATE_CLASSNAME 
VMML_TEMPLATE_CLASSNAME::operator-( T scalar ) const
{
	tensor3< I1, I2, I3, T > result( *this );
	result -= scalar;
	return result;
}



VMML_TEMPLATE_STRING
void
VMML_TEMPLATE_CLASSNAME::operator-=( T scalar )
{
    iterator it = begin(), it_end = end(); 
    for( ; it != it_end; ++it )
    {
        *it -= scalar;
    }
}




//tensor matrix multiplications

VMML_TEMPLATE_STRING
template< size_t J1, size_t J2, size_t J3 > 
void
VMML_TEMPLATE_CLASSNAME::multiply_horizontal( const tensor3< J1, J2, J3, T >& core, const matrix< I3, J3, T >& U3 )
{
	matrix< J2, J3, T> slice;
	matrix< J3, J2, T> slice_t;
	matrix< I3, J2, T> slice_new;
	matrix< J2, I3, T> slice_new_t;
	for (size_t i1 = 0; i1 < J1; ++i1)
	{
		core.get_horizontal_slice( i1, slice );
		slice.transpose_to( slice_t );
		slice_new.multiply( U3, slice_t );
		slice_new.transpose_to( slice_new_t );
		set_horizontal_slice( i1, slice_new_t );		
	}
}

VMML_TEMPLATE_STRING
template< size_t J1, size_t J2, size_t J3 > 
void
VMML_TEMPLATE_CLASSNAME::multiply_lateral( const tensor3< J1, J2, J3, T >& core, const matrix< I1, J1, T >& U1 )
{
	matrix< J1, J3, T> slice;
	matrix< I1, J3, T> slice_new;
	for ( size_t i2 = 0; i2 < J2; ++i2 )
	{
		core.get_lateral_slice( i2, slice );
		slice_new.multiply( U1, slice );
		set_lateral_slice( i2, slice_new );		
	}
}


VMML_TEMPLATE_STRING
template< size_t J1, size_t J2, size_t J3 > 
void
VMML_TEMPLATE_CLASSNAME::multiply_frontal( const tensor3< J1, J2, J3, T >& core, const matrix< I2, J2, T >& U2 )
{
	matrix< J1, J2, T> slice;
	matrix< J2, J1, T> slice_t;
	matrix< I2, J1, T> slice_new;
	matrix< J1, I2, T> slice_new_t;
	for ( size_t i3 = 0; i3 < J3; ++i3 )
	{
		core.get_frontal_slice( i3, slice ); 
		slice.transpose_to( slice_t );
		slice_new.multiply( U2, slice_t );
		slice_new.transpose_to( slice_new_t );
		set_frontal_slice( i3, slice_new_t );		
	}
}



VMML_TEMPLATE_STRING
template< size_t J1, size_t J2, size_t J3 > 
void
VMML_TEMPLATE_CLASSNAME::full_tensor3_matrix_multiplication(  const tensor3< J1, J2, J3, T >& core, 
															const matrix< I1, J1, T >& U1, 
															const matrix< I2, J2, T >& U2, 
															const matrix< I3, J3, T >& U3 )
{
	
	tensor3< I1, J2, J3, T> t3_result_1;
	tensor3< I1, I2, J3, T> t3_result_2;
	
	t3_result_1.multiply_lateral( core, U1 );
	t3_result_2.multiply_frontal( t3_result_1, U2 );
	multiply_horizontal( t3_result_2, U3 );
}

VMML_TEMPLATE_STRING
void 
VMML_TEMPLATE_CLASSNAME::horizontal_matricization( matrix< I3, I1*I2, T>& matricization) const
{
	matrix< I2, I3, T> horizontal_slice;
	matrix< I3, I2, T> horizontal_slice_t;
	for( size_t i = 0; i < I1; ++i )
	{
		get_horizontal_slice(i, horizontal_slice );
		horizontal_slice.transpose_to( horizontal_slice_t );
		for( size_t col = 0; col < I2; ++col )
		{
			matricization.set_column( i*I2+col,  horizontal_slice_t.get_column(col));
		} 
	}
}

VMML_TEMPLATE_STRING
void 
VMML_TEMPLATE_CLASSNAME::lateral_matricization( matrix< I1, I2*I3, T>& matricization) const
{
	matrix< I1, I3, T> lateral_slice;
	for( size_t i = 0; i < I2; ++i )
	{
		get_lateral_slice(i, lateral_slice );
		for( size_t col = 0; col < I3; ++col )
		{
			matricization.set_column( i*I3+col,  lateral_slice.get_column(col));
		} 
	}
	
}


VMML_TEMPLATE_STRING
void 
VMML_TEMPLATE_CLASSNAME::frontal_matricization( matrix< I2, I1*I3, T>& matricization) const
{
	matrix< I1, I2, T> frontal_slice;
	matrix< I2, I1, T> frontal_slice_t;
	for( size_t i = 0; i < I3; ++i )
	{
		get_frontal_slice(i, frontal_slice );
		frontal_slice.transpose_to( frontal_slice_t );
		for( size_t col = 0; col < I1; ++col )
		{
			matricization.set_column( i*I1+col, frontal_slice_t.get_column(col));
		} 
	}	
}




VMML_TEMPLATE_STRING
tensor3< I1, I2, I3, T >
VMML_TEMPLATE_CLASSNAME::operator*( T scalar )
{
    tensor3< I1, I2, I3, T > result;
    
	for( size_t i3 = 0; i3 < I3; ++i3 )
	{
		result.array[ i3 ] = array[ i3 ] * scalar;
	}

    return result;
}


VMML_TEMPLATE_STRING
void
VMML_TEMPLATE_CLASSNAME::operator*=( T scalar )
{
	for( size_t i3 = 0; i3 < I3; ++i3 )
	{
		array[ i3 ] *= scalar;
	}
	
}



VMML_TEMPLATE_STRING
inline tensor3< I1, I2, I3, T >
VMML_TEMPLATE_CLASSNAME::operator-() const
{
    return negate();
}


VMML_TEMPLATE_STRING
tensor3< I1, I2, I3, T >
VMML_TEMPLATE_CLASSNAME::negate() const
{
    tensor3< I1, I2, I3, T > result;
    result *= -1.0;
    return result;
}




#undef VMML_TEMPLATE_STRING
#undef VMML_TEMPLATE_CLASSNAME

} // namespace vmml

#endif

