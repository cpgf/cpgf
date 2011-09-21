#ifndef __VMML__VECTOR__HPP__
#define __VMML__VECTOR__HPP__

#include <vmmlib/vmmlib_config.hpp>
#include <vmmlib/math.hpp>
#include <vmmlib/enable_if.hpp>
#include <vmmlib/exception.hpp>

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <limits>
#include <algorithm>

namespace vmml
{

template< size_t M, typename T = float >
class vector
{
public:
    typedef T                                       value_type;
	typedef T*                                      pointer;
	typedef T&                                      reference;
    typedef T*                                      iterator;
    typedef const T*                                const_iterator;
    typedef std::reverse_iterator< iterator >       reverse_iterator;
    typedef std::reverse_iterator< const_iterator > const_reverse_iterator;
    
    static const size_t DIMENSION = M;

    // iterators
    inline iterator begin();
    inline iterator end();
    inline const_iterator begin() const;
    inline const_iterator end() const;
    inline reverse_iterator rbegin();
    inline reverse_iterator rend();
    inline const_reverse_iterator rbegin() const;
    inline const_reverse_iterator rend() const;
    
    #ifndef VMMLIB_NO_CONVERSION_OPERATORS
    // conversion operators
    inline operator T*();
    inline operator const T*() const;
    #else
    inline T& operator[]( size_t index );
    inline const T& operator[]( size_t index ) const;
    #endif
    
    // accessors 
    inline T& operator()( size_t index );
    inline const T& operator()( size_t index ) const;
    #if 0
    inline T& operator[]( size_t index );
    inline const T& operator[]( size_t index ) const;
    #endif
    
    inline T& at( size_t index );
    inline const T& at( size_t index ) const;

    // element accessors for M <= 4;
    inline T& x();
    inline T& y();
    inline T& z();
    inline T& w();
    inline const T& x() const;
    inline const T& y() const;
    inline const T& z() const;
    inline const T& w() const;

    // pixel color element accessors for M<= 4
    inline T& r();
    inline T& g();
    inline T& b();
    inline T& a();
    inline const T& r() const;
    inline const T& g() const;
    inline const T& b() const;
    inline const T& a() const;

    bool operator==( const vector& other ) const;
    bool operator!=( const vector& other ) const;
    bool equals( const vector& other, 
                 T tolerance = std::numeric_limits< T >::epsilon() ) const;
   
    // remember kids: c_arrays are dangerous and evil!
    const vector& operator=( const T* c_array );
    T operator=( T filler );
    
    const vector& operator=( const vector& other );
    // returns void to avoid 'silent' loss of precision when chaining
    template< typename U >
    void operator=( const vector< M, U >& other );
    
    // to-homogenous-coordinates assignment operator
    // non-chainable because of sfinae
    template< size_t N >
    typename enable_if< N == M - 1 >::type*
        operator=( const vector< N, T >& source_ );
        
    // from-homogenous-coordinates assignment operator
    // non-chainable because of sfinae
    template< size_t N >
    typename enable_if< N == M + 1 >::type*
        operator=( const vector< N, T >& source_ );
    
    vector operator*( const vector& other ) const;
    vector operator/( const vector& other ) const;    
    vector operator+( const vector& other ) const; 
    vector operator-( const vector& other ) const;

    void operator*=( const vector& other );
    void operator/=( const vector& other );    
    void operator+=( const vector& other ); 
    void operator-=( const vector& other );

    vector operator*( const T other ) const;
    vector operator/( const T other ) const;    
    vector operator+( const T other ) const; 
    vector operator-( const T other ) const;

    void operator*=( const T other );
    void operator/=( const T other );    
    void operator+=( const T other ); 
    void operator-=( const T other );

    vector operator-() const;

    const vector& negate();

    // constructors 
    vector() {}; // std ctor - WARNING: NO INITIALIZATION
    vector( const T& a ); // sets all components to a;
    vector( const T& x, const T& y );
    vector( const T& x, const T& y, const T& z );
    vector( const T& x, const T& y, const T& z, const T& w );
    
    // initializes the first M-1 values from vector_, the last from last_
    vector( const vector< M-1, T >& vector_, T last_ );
    
    vector( const T* values );

    // vec< M > with homogeneous coordinates <-> vec< M-1 > conversion ctor
    // to-homogenous-coordinates ctor
    template< size_t N >
    vector( const vector< N, T >& source_,
        typename enable_if< N == M - 1 >::type* = 0 );

    // from-homogenous-coordinates ctor
    template< size_t N >
    vector( const vector< N, T >& source_,
        typename enable_if< N == M + 1 >::type* = 0  );

    template< typename U >
    vector( const vector< M, U >& source_ );
    
    void set( T a ); // sets all components to a;
    void set( const vector< M-1, T >& v, T a );

    // sets the first few components to a certain value
    void set( T x, T y );
    void set( T x, T y, T z ); 
    void set( T x, T y, T z, T w );
    
    template< typename input_iterator_t >
    void iter_set( input_iterator_t begin_, input_iterator_t end_ );

    // compute the cross product of two vectors
    // note: there's also a free function:
    // vector<> cross( const vector<>, const vector<> )

    // result = vec1.cross( vec2 ) => retval result = vec1 x vec2
    template< typename TT >
    inline vector cross( const vector< M, TT >& rhs, 
        typename enable_if< M == 3, TT >::type* = 0 ) const;

    // result.cross( vec1, vec2 ) => (this) = vec1 x vec2
    template< typename TT >
    void cross( const vector< M, TT >& a, const vector< M, TT >& b, 
        typename enable_if< M == 3, TT >::type* = 0 );
  

    // compute the dot product of two vectors
    // note: there's also a free function:
    // T dot( const vector<>, const vector<> );
    inline T dot( const vector& other ) const;


    // normalize the vector
    // note: there's also a free function:
    // vector<> normalize( const vector<> );
    inline T normalize();
    
    inline T length() const;
    inline T squared_length() const;
    
    inline T distance( const vector& other_vector_ ) const;
    inline T squared_distance( const vector& other_vector_ ) const;
    
    template< typename TT >
    vector< 3, T > rotate( const T theta, vector< M, TT > axis,
                           typename enable_if< M == 3, TT >::type* = 0 ) const;

    // right hand system, CCW triangle
    // (*this) = normal of v0,v1,v2
    void compute_normal( const vector& v0, const vector& v1, const vector& v2 );
    // retval = normal of (this), v1, v2
    vector compute_normal( const vector& v1, const vector& v2 ) const;
    
    template< size_t N >
    vector< N, T >& get_sub_vector( size_t offset = 0, 
        typename enable_if< M >= N >::type* = 0 );

    template< size_t N >
    const vector< N, T >& get_sub_vector( size_t offset = 0, 
        typename enable_if< M >= N >::type* = 0 ) const;
    
	// sphere functions - sphere layout: center xyz, radius w
    template< typename TT >
	inline vector< 3, T > project_point_onto_sphere( 
        const vector< 3, TT >& point, 
        typename enable_if< M == 4, TT >::type* = 0 ) const;
        
	// returns a negative distance if the point lies in the sphere
    template< typename TT >
	inline T distance_to_sphere( const vector< 3, TT >& point, 
        typename enable_if< M == 4, TT >::type* = 0 ) const;

	// plane functions - plane layout; normal xyz, distance w
    template< typename TT >
	inline T distance_to_plane( const vector< 3, TT >& point, 
        typename enable_if< M == 4, TT >::type* = 0 ) const;
    
    template< typename TT >
    inline vector< 3, T > project_point_onto_plane( 
        const vector< 3, TT >& point, 
        typename enable_if< M == 4, TT >::type* = 0 ) const;
    
    // returns the index of the minimal resp. maximal value in the vector
    size_t      find_min_index() const;
    size_t      find_max_index() const;

    // returns the index of the minimal resp. maximal value in the vector
    size_t      find_abs_min_index() const;
    size_t      find_abs_max_index() const;

    // returns minimal resp. maximal value in the vector
    T&          find_min();
    T&          find_max();
    const T&    find_min() const;
    const T&    find_max() const;
    
    void clamp( const T& min = 0.0, const T& max = 1.0 );

    template< typename TT >
    void scale_to( vector< M, TT >& scaled_vector, 
        T min_value = -1.0, T max_value = 1.0 ) const;
    
    inline static size_t size(); // returns M
    
    friend std::ostream& operator<< ( std::ostream& os, const vector& vector_ )
    {
#ifdef EQ_EXPORT
        const std::ios::fmtflags flags = os.flags();
        const int                prec  = os.precision();

        os.setf( std::ios::right, std::ios::adjustfield );
        os.precision( 5 );
        os << "[ ";
        for( size_t index = 0; index < M; ++index )
            os << std::setw(10) << vector_.at( index ) << " ";
        os << "]";
        os.precision( prec );
        os.setf( flags );
#else
        os << "(";
        size_t index = 0;
        for( ; index < M - 1; ++index )
        {
            os << vector_.at( index ) << ", ";
        }
        os << vector_.at( index ) << ") ";
#endif
        return os;
    }
        


        // storage
        VMMLIB_ALIGN( T array[ M ] );
    ;

    // Vector3 defaults
    static const vector FORWARD;
    static const vector BACKWARD;
    static const vector UP;
    static const vector DOWN;
    static const vector LEFT;
    static const vector RIGHT;

    static const vector ONE;
    static const vector ZERO;
    
    // Unit vectors
    static const vector UNIT_X;
    static const vector UNIT_Y;
    static const vector UNIT_Z;

}; // class vector

//
// typedefs and statics 
//

#ifndef VMMLIB_NO_TYPEDEFS
typedef vector< 2, float >  vec2f;
typedef vector< 2, double > vec2d;
typedef vector< 3, float >  vec3f;
typedef vector< 3, double > vec3d;
typedef vector< 4, float >  vec4f;
typedef vector< 4, double > vec4d;
#endif

template< size_t M, typename T >
const vector< M, T > vector< M, T >::FORWARD( 0, 0, -1 );
template< size_t M, typename T >
const vector< M, T > vector< M, T >::BACKWARD( 0, 0, 1 );
template< size_t M, typename T >
const vector< M, T > vector< M, T >::UP( 0, 1, 0 );
template< size_t M, typename T >
const vector< M, T > vector< M, T >::DOWN( 0, -1, 0 );
template< size_t M, typename T >
const vector< M, T > vector< M, T >::LEFT( -1, 0, 0 );
template< size_t M, typename T >
const vector< M, T > vector< M, T >::RIGHT( 1, 0, 0 );
template< size_t M, typename T >

const vector< M, T > vector< M, T >::ONE( static_cast< T >( 1 ));
template< size_t M, typename T >
const vector< M, T > vector< M, T >::ZERO( static_cast< T >( 0 ));
template< size_t M, typename T >

const vector< M, T > vector< M, T >::UNIT_X( 1, 0, 0 );
template< size_t M, typename T >
const vector< M, T > vector< M, T >::UNIT_Y( 0, 1, 0 );
template< size_t M, typename T >
const vector< M, T > vector< M, T >::UNIT_Z( 0, 0, 1 );

//
//  some free functions for convenience
//

template< size_t M, typename T >
bool equals( const vector< M, T >& a, const vector< M, T >& b )
{
    return a.equals( b );
}


// allows float * vector, not only vector * float 
template< size_t M, typename T >
static vector< M, T >
operator* ( T factor, const vector< M, T >& vector_ )
{
    return vector_ * factor;
}


template< size_t M, typename T >
inline T
dot( const vector< M, T >& first, const vector< M, T >& second ) 
{
    return first.dot( second );
}


template< size_t M, typename T >
inline vector< M, T >
cross( const vector< 3, T >& a, const vector< 3, T >& b )
{
    return a.cross( b );
}


template< size_t M, typename T >
inline vector< M, T >
normalize( const vector< M, T >& vector_ )
{
    vector< M, T > v( vector_ );
    v.normalize();
    return v;
}


template< size_t M, typename T >
vector< M, T >::vector( const T& _a )
{
    for( iterator it = begin(), it_end = end(); it != it_end; ++it )
    {
        *it = _a;
    }
}



template< size_t M, typename T >
vector< M, T >::vector( const T& _x, const T& _y )
{
    array[ 0 ] = _x;
    array[ 1 ] = _y;
}


template< size_t M, typename T >
vector< M, T >::vector( const T& _x, const T& _y, const T& _z )
{
    array[ 0 ] = _x;
    array[ 1 ] = _y;
    array[ 2 ] = _z;
}



template< size_t M, typename T >
vector< M, T >::vector( const T& _x, const T& _y, const T& _z, const T& _w )
{
    array[ 0 ] = _x;
    array[ 1 ] = _y;
    array[ 2 ] = _z;
    array[ 3 ] = _w;

}


template< size_t M, typename T >
vector< M, T >::vector( const T* values )
{
    memcpy( array, values, M * sizeof( T ));
}


template< size_t M, typename T >
// initializes the first M-1 values from vector_, the last from last_
vector< M, T >::vector( const vector< M-1, T >& vector_, T last_ )
{
    typename vector< M-1, T >::const_iterator
        it = vector_.begin(), it_end = vector_.end();

    iterator my_it = begin();

    for( ; it != it_end; ++it, ++my_it )
    {
        (*my_it) = *it;
    }
    (*my_it) = last_;
}



// to-homogenous-coordinates ctor
template< size_t M, typename T >
template< size_t N >
vector< M, T >::
vector( const vector< N, T >& source_, typename enable_if< N == M - 1 >::type* )
{
    (*this) = source_;
}




// from-homogenous-coordinates ctor
template< size_t M, typename T >
template< size_t N >
vector< M, T >::
vector( const vector< N, T >& source_, typename enable_if< N == M + 1 >::type*  )
{
    (*this) = source_;
}


template< size_t M, typename T >
template< typename U >
vector< M, T >::vector( const vector< M, U >& source_ )
{
    (*this) = source_;
}



template< size_t M, typename T >
void
vector< M, T >::set( T _a )
{
    for( iterator it = begin(), it_end = end(); it != it_end; ++it )
    {
        *it = _a;
    }
}



template< size_t M, typename T >
void
vector< M, T >::set( const vector< M-1, T >& v, T _a )
{
    memcpy( array, v.array, sizeof( T ) * (M-1) );
    at( M-1 ) = _a;
}




template< size_t M, typename T >
void
vector< M, T >::set( T _x, T _y )
{
    array[ 0 ] = _x;
    array[ 1 ] = _y;
}


template< size_t M, typename T >
void
vector< M, T >::set( T _x, T _y, T _z )
{
    array[ 0 ] = _x;
    array[ 1 ] = _y;
    array[ 2 ] = _z;
}



template< size_t M, typename T >
void
vector< M, T >::set( T _x, T _y, T _z, T _w )
{
    array[ 0 ] = _x;
    array[ 1 ] = _y;
    array[ 2 ] = _z;
    array[ 3 ] = _w;
}


template< size_t M, typename T >
inline T&
vector< M, T >::operator()( size_t index )
{
	return at( index );
}



template< size_t M, typename T >
inline const T&
vector< M, T >::operator()( size_t index ) const
{
	return at( index );
}



template< size_t M, typename T >
inline T&
vector< M, T >::at( size_t index )
{
    #ifdef VMMLIB_SAFE_ACCESSORS
    if ( index >= M )
    {
        VMMLIB_ERROR( "at() - index out of bounds", VMMLIB_HERE );
    }
    #endif
    return array[ index ];
}



template< size_t M, typename T >
inline const T&
vector< M, T >::at( size_t index ) const
{
    #ifdef VMMLIB_SAFE_ACCESSORS
    if ( index >= M )
    {
        VMMLIB_ERROR( "at() - index out of bounds", VMMLIB_HERE );
    }
    #endif
    return array[ index ];
}


#ifndef VMMLIB_NO_CONVERSION_OPERATORS

template< size_t M, typename T >
vector< M, T >::operator T*()
{
    return array;
}



template< size_t M, typename T >
vector< M, T >::operator const T*() const
{
    return array;
}
#else

template< size_t M, typename T >
T&
vector< M, T >::operator[]( size_t index )
{   
    return at( index );
}

template< size_t M, typename T >
const T&
vector< M, T >::operator[]( size_t index ) const
{   
    return at( index );
}


#endif


#if 0
template< size_t M, typename T >
inline T&
vector< M, T >::operator[]( size_t index )
{
    return at( index );
}



template< size_t M, typename T >
inline const T&
vector< M, T >::operator[]( size_t index ) const
{
    return at( index );
}
#endif


template< size_t M, typename T >
vector< M, T >
vector< M, T >::operator*( const vector< M, T >& other ) const
{
    vector< M, T > result;
    for( size_t index = 0; index < M; ++index )
    {
        result.at( index ) = at( index ) * other.at( index );
    }
    return result;
}



template< size_t M, typename T >
vector< M, T >
vector< M, T >::operator/( const vector< M, T >& other ) const
{
    vector< M, T > result;
    for( size_t index = 0; index < M; ++index )
    {
        result.at( index ) = at( index ) / other.at( index );
    }
    return result;
}



template< size_t M, typename T >
vector< M, T >
vector< M, T >::operator+( const vector< M, T >& other ) const
{
    vector< M, T > result;
    for( size_t index = 0; index < M; ++index )
    {
        result.at( index ) = at( index ) + other.at( index );
    }
    return result;
}



template< size_t M, typename T >
vector< M, T >
vector< M, T >::operator-( const vector< M, T >& other ) const
{
    vector< M, T > result;
    for( size_t index = 0; index < M; ++index )
    {
        result.at( index ) = at( index ) - other.at( index );
    }
    return result;
}




template< size_t M, typename T >
void
vector< M, T >::operator*=( const vector< M, T >& other )
{
    for( size_t index = 0; index < M; ++index )
    {
        at( index ) *= other.at( index );
    }
}



template< size_t M, typename T >
void
vector< M, T >::operator/=( const vector< M, T >& other )
{
    for( size_t index = 0; index < M; ++index )
    {
        at( index ) /= other.at( index );
    }
}



template< size_t M, typename T >
void
vector< M, T >::operator+=( const vector< M, T >& other )
{
    for( size_t index = 0; index < M; ++index )
    {
        at( index ) += other.at( index );
    }
}



template< size_t M, typename T >
void
vector< M, T >::operator-=( const vector< M, T >& other )
{
    for( size_t index = 0; index < M; ++index )
    {
        at( index ) -= other.at( index );
    }
}



template< size_t M, typename T >
vector< M, T >
vector< M, T >::operator*( const T other ) const
{
    vector< M, T > result;
    for( size_t index = 0; index < M; ++index )
    {
        result.at( index ) = at( index ) * other;
    }
    return result;
}



template< size_t M, typename T >
vector< M, T >
vector< M, T >::operator/( const T other ) const
{
    vector< M, T > result;
    for( size_t index = 0; index < M; ++index )
    {
        result.at( index ) = at( index ) / other;
    }
    return result;
}



template< size_t M, typename T >
vector< M, T >
vector< M, T >::operator+( const T other ) const
{
    vector< M, T > result;
    for( size_t index = 0; index < M; ++index )
    {
        result.at( index ) = at( index ) + other;
    }
    return result;
}



template< size_t M, typename T >
vector< M, T >
vector< M, T >::operator-( const T other ) const
{
    vector< M, T > result;
    for( size_t index = 0; index < M; ++index )
    {
        result.at( index ) = at( index ) - other;
    }
    return result;
}




template< size_t M, typename T >
void
vector< M, T >::operator*=( const T other )
{
    for( size_t index = 0; index < M; ++index )
    {
        at( index ) *= other;
    }
}



template< size_t M, typename T >
void
vector< M, T >::operator/=( const T other )
{
    for( size_t index = 0; index < M; ++index )
    {
        at( index ) /= other;
    }
}



template< size_t M, typename T >
void
vector< M, T >::operator+=( const T other )
{
    for( size_t index = 0; index < M; ++index )
    {
        at( index ) += other;
    }
}



template< size_t M, typename T >
void
vector< M, T >::operator-=( const T other )
{
    for( size_t index = 0; index < M; ++index )
    {
        at( index ) -= other;
    }
}



template< size_t M, typename T >
vector< M, T >
vector< M, T >::operator-() const
{
    vector< M, T > v( *this );
    return v.negate();
}



template< size_t M, typename T >
const vector< M, T >&
vector< M, T >::negate()
{
    for( size_t index = 0; index < M; ++index )
    {
        array[ index ] = -array[ index ];
    }
    return *this;
}



template< size_t M, typename T >
inline T&
vector< M, T >::x()
{
    return array[ 0 ];
}



template< size_t M, typename T >
inline T&
vector< M, T >::y()
{
    return array[ 1 ];
}



template< size_t M, typename T >
inline T&
vector< M, T >::z()
{
    return array[ 2 ];
}



template< size_t M, typename T >
inline T&
vector< M, T >::w()
{
    return array[ 3 ];
}



template< size_t M, typename T >
inline const T&
vector< M, T >::x() const
{
    return array[ 0 ];
}



template< size_t M, typename T >
inline const T&
vector< M, T >::y() const
{
    return array[ 1 ];
}



template< size_t M, typename T >
inline const T&
vector< M, T >::z() const
{
    return array[ 2 ];
}



template< size_t M, typename T >
inline const T&
vector< M, T >::w() const
{
    return array[ 3 ];
}


template< size_t M, typename T >
inline T&
vector< M, T >::r()
{
    return array[ 0 ];
}



template< size_t M, typename T >
inline T&
vector< M, T >::g()
{
    return array[ 1 ];
}



template< size_t M, typename T >
inline T&
vector< M, T >::b()
{
    return array[ 2 ];
}



template< size_t M, typename T >
inline T&
vector< M, T >::a()
{
    return array[ 3 ];
}



template< size_t M, typename T >
inline const T&
vector< M, T >::r() const
{
    return array[ 0 ];
}



template< size_t M, typename T >
inline const T&
vector< M, T >::g() const
{
    return array[ 1 ];
}



template< size_t M, typename T >
inline const T&
vector< M, T >::b() const
{
    return array[ 2 ];
}



template< size_t M, typename T >
inline const T&
vector< M, T >::a() const
{
    return array[ 3 ];
}

// result = vec1.cross( vec2 ) => result = vec1 x vec2
template< size_t M, typename T >
template< typename TT >
inline vector< M, T >
vector< M, T >::cross( const vector< M, TT >& rhs, 
    typename enable_if< M == 3, TT >::type* ) const
{
    vector< M, T > result;
    result.cross( *this, rhs );
    return result;
}



// result.cross( vec1, vec2 ) => (this) = vec1 x vec2
template< size_t M, typename T >
template< typename TT >
void
vector< M, T >::
cross( const vector< M, TT >& aa, const vector< M, TT >& bb, 
    typename enable_if< M == 3, TT >::type* )
{ 
    array[ 0 ] = aa.y() * bb.z() - aa.z() * bb.y(); 
    array[ 1 ] = aa.z() * bb.x() - aa.x() * bb.z(); 
    array[ 2 ] = aa.x() * bb.y() - aa.y() * bb.x(); 
}



template< size_t M, typename T >
inline T
vector< M, T >::dot( const vector< M, T >& other ) const
{
    T tmp = 0.0;
    for( size_t index = 0; index < M; ++index )
    {
        tmp += at( index ) * other.at( index );
    }
    return tmp;
}


template< size_t M, typename T >
inline T
vector< M, T >::normalize()
{
    T len = length();

    if ( len == 0 )
        return 0;

    T tmp = 1.0 / len;
    (*this) *= tmp;
    return len;
}



template< size_t M, typename T >
inline T
vector< M, T >::length() const
{
    return sqrt( squared_length() );
}



template< size_t M, typename T >
inline T
vector< M, T >::squared_length() const
{
    T _squared_length = 0.0;
    for( const_iterator it = begin(), it_end = end(); it != it_end; ++it )
    {
        _squared_length += (*it) * (*it);
    }
    return _squared_length;
}



template< size_t M, typename T >
inline T
vector< M, T >::distance( const vector< M, T >& other_vector_ ) const
{
    return sqrt( squared_distance( other_vector_ ) );
}



template< size_t M, typename T >
inline T
vector< M, T >::squared_distance( const vector< M, T >& other_vector_ ) const
{
    vector< M, T > tmp( *this );
    tmp -= other_vector_;
    return tmp.squared_length();
}



template< size_t M, typename T >
void
vector< M, T >::compute_normal(
    const vector< M, T >& aa, 
    const vector< M, T >& bb, 
    const vector< M, T >& cc
    )
{
    vector< M, T > u,v;
    // right hand system, CCW triangle
    u = bb - aa;
    v = cc - aa;
    cross( u, v );
    normalize();
}



template< size_t M, typename T >
vector< M, T >
vector< M, T >::compute_normal(
    const vector< M, T >& bb, 
    const vector< M, T >& cc
    ) const
{
    vector< M, T > tmp;
    tmp.compute_normal( *this, bb, cc);
    return tmp;
}

template< size_t M, typename T >
template< typename TT >
vector< 3, T > vector< M, T >::rotate( const T theta, vector< M, TT > axis,
			typename enable_if< M == 3, TT >::type* ) const
{
    axis.normalize();

    const T costheta = cos( theta );
    const T sintheta = sin( theta );

    return vector< 3, T >(
        (costheta + ( 1.0f - costheta ) * axis.x() * axis.x() ) * x()    +
        (( 1 - costheta ) * axis.x() * axis.y() - axis.z() * sintheta ) * y() +
        (( 1 - costheta ) * axis.x() * axis.z() + axis.y() * sintheta ) * z(),

        (( 1 - costheta ) * axis.x() * axis.y() + axis.z() * sintheta ) * x() +
        ( costheta + ( 1 - costheta ) * axis.y() * axis.y() ) * y() +
        (( 1 - costheta ) * axis.y() * axis.z() - axis.x() * sintheta ) * z(),

        (( 1 - costheta ) * axis.x() * axis.z() - axis.y() * sintheta ) * x() +
        (( 1 - costheta ) * axis.y() * axis.z() + axis.x() * sintheta ) * y() +
        ( costheta + ( 1 - costheta ) * axis.z() * axis.z() ) * z() );
} 


// sphere layout: center xyz, radius w
template< size_t M, typename T >
template< typename TT >
inline vector< 3, T >
vector< M, T >::
project_point_onto_sphere( const vector< 3, TT >& point, 
    typename enable_if< M == 4, TT >::type* ) const
{
    const vector< 3, T >& _center = get_sub_vector< 3 >( 0 );

    vector< 3, T > projected_point( point );
    projected_point -= _center;
    projected_point.normalize();
    projected_point *= w();
    return _center + projected_point;
}



// sphere layout: center xyz, radius w
template< size_t M, typename T >
template< typename TT >
inline T
vector< M, T >::
distance_to_sphere( const vector< 3, TT >& point, 
    typename enable_if< M == 4, TT >::type* ) const
{
    const vector< 3, T >& center_ = get_sub_vector< 3 >( 0 );
	return ( point - center_ ).length() - w();
}



template< size_t M, typename T >
template< size_t N >
inline vector< N, T >&
vector< M, T >::get_sub_vector( size_t offset, 
    typename enable_if< M >= N >::type* )
{
    assert( offset <= M - N );
    return reinterpret_cast< vector< N, T >& >( *( begin() + offset ) );
}



template< size_t M, typename T >
template< size_t N >
inline const vector< N, T >&
vector< M, T >::get_sub_vector( size_t offset, 
    typename enable_if< M >= N >::type* ) const
{
    assert( offset <= M - N );
    return reinterpret_cast< const vector< N, T >& >( *( begin() + offset ) );
}



// plane: normal xyz, distance w
template< size_t M, typename T >
template< typename TT >
inline T
vector< M, T >::distance_to_plane( const vector< 3, TT >& point, 
    typename enable_if< M == 4, TT >::type* ) const
{
    const vector< 3, T >& normal = get_sub_vector< 3 >( 0 );
    return normal.dot( point ) + w();
}



// plane: normal xyz, distance w
template< size_t M, typename T >
template< typename TT >
vector< 3, T >
vector< M, T >::project_point_onto_plane( const vector< 3, TT >& point, 
    typename enable_if< M == 4, TT >::type* ) const
{
    const vector< 3, T >& normal = get_sub_vector< 3 >( 0 );
    return point - ( normal * distance_to_plane( point ) );
}



template< size_t M, typename T >
bool
vector< M, T >::operator==( const vector< M, T >& other ) const
{
    bool ok = true;
    for( size_t index = 0; ok && index < M; ++index )
    {
        ok = at( index ) == other.at( index );
    }
    return ok;
}


template< size_t M, typename T >
bool
vector< M, T >::operator!=( const vector< M, T >& other ) const
{
    return ! this->operator==( other );
}


template< size_t M, typename T >
bool
vector< M, T >::
equals( const vector< M, T >& other, T tolerance ) const
{
    bool ok = true;
    for( size_t index = 0; ok && index < M; ++index )
    {
        ok = fabs( at( index ) - other( index ) ) < tolerance;
    }
    return ok;

}



// to-homogenous-coordinates assignment operator
// non-chainable because of sfinae
template< size_t M, typename T >
template< size_t N >
typename enable_if< N == M - 1 >::type*
vector< M, T >::
operator=( const vector< N, T >& source_ )
{
    std::copy( source_.begin(), source_.end(), begin() );
    at( M - 1 ) = static_cast< T >( 1.0 );
    return 0;
}

    
// from-homogenous-coordinates assignment operator
// non-chainable because of sfinae
template< size_t M, typename T >
template< size_t N >
typename enable_if< N == M + 1 >::type*
vector< M, T >::
operator=( const vector< N, T >& source_ )
{
    const T w_reci = static_cast< T >( 1.0 ) / source_( M );
    iterator it = begin(), it_end = end();
    for( size_t index = 0; it != it_end; ++it, ++index )
    {
        *it = source_( index ) * w_reci;
    }
    return 0;
}


template< size_t M, typename T >
const vector< M, T >&
vector< M, T >::operator=( const T* c_array )
{
    iter_set( c_array, c_array + M );
    return *this;
}



template< size_t M, typename T >
T
vector< M, T >::operator=( T filler_value )
{
    for( size_t index = 0; index < M; ++index )
    {
        at( index ) = filler_value;
    }
    return filler_value;
}




template< size_t M, typename T >
const vector< M, T >&
vector< M, T >::operator=( const vector< M, T >& other )
{
    memcpy( array, other.array, M * sizeof( T ) );
    return *this;
}



// returns void to avoid 'silent' loss of precision when chaining
template< size_t M, typename T >
template< typename U >
void
vector< M, T >::operator=( const vector< M, U >& source_ )
{
    typedef typename vector< M, U >::const_iterator u_c_iter;
    u_c_iter it = source_.begin(), it_end = source_.end();
    for( iterator my_it = begin(); it != it_end; ++it, ++my_it )
    {
        *my_it = static_cast< T >( *it );
    }
}



template< size_t M, typename T >
template< typename input_iterator_t >
void
vector< M, T >::iter_set( input_iterator_t begin_, input_iterator_t end_ )
{
    input_iterator_t in_it = begin_;
    iterator it = begin(), it_end = end();
    for( ; it != it_end && in_it != end_; ++it, ++in_it )
    {
        (*it) = static_cast< T >( *in_it );
    }

}

template< size_t M, typename T >
void vector< M, T >::clamp( const T& min, const T& max )
{
    for( size_t i = 0; i < M; ++i )
    {
        if( array[i] < min )
            array[i] = min;
        if( array[i] > max )
            array[i] = max;
    }
}



template< size_t M, typename T >
template< typename TT >
void
vector< M, T >::scale_to( vector< M, TT >& result_, 
    T min_value, T max_value ) const
{
    T range       = max_value-min_value;
    T half_range  = range * 0.5;
    T scale       = ( 1.0 / range ) * static_cast< T >( std::numeric_limits< TT >::max() );
    
    for( size_t index = 0; index < M; ++index )
    {
        result_.at( index ) 
            = static_cast< TT >( ( at( index ) + half_range ) * scale );
    }
    
}



template< size_t M, typename T >
inline size_t
vector< M, T >::size()
{
    return M;
}



template< size_t M, typename T >
size_t
vector< M, T >::find_min_index() const
{
    return std::min_element( begin(), end() ) - begin();
}



template< size_t M, typename T >
size_t
vector< M, T >::find_max_index() const
{
    return std::max_element( begin(), end() ) - begin();
}



template< size_t M, typename T >
size_t
vector< M, T >::find_abs_min_index() const
{
    return std::min_element( begin(), end(), vmml::math::abs_less< T >() ) - begin();
}



template< size_t M, typename T >
size_t
vector< M, T >::find_abs_max_index() const
{
    return std::max_element( begin(), end(), vmml::math::abs_greater< T >() ) - begin();
}



template< size_t M, typename T >
T&
vector< M, T >::find_min()
{
    return *std::min_element( begin(), end() );
}



template< size_t M, typename T >
const T&
vector< M, T >::find_min() const
{
    return *std::min_element( begin(), end() );
}



template< size_t M, typename T >
T&
vector< M, T >::find_max()
{
    return *std::max_element( begin(), end() );
}



template< size_t M, typename T >
const T&
vector< M, T >::find_max() const
{
    return *std::max_element( begin(), end() );
}


template< size_t M, typename T >
inline typename vector< M, T >::iterator
vector< M, T >::begin()
{
    return array;
}


template< size_t M, typename T >
inline typename vector< M, T >::iterator
vector< M, T >::end()
{
    return array + M; ;
}


template< size_t M, typename T >
inline typename vector< M, T >::const_iterator
vector< M, T >::begin() const
{
    return array;
}


template< size_t M, typename T >
inline typename vector< M, T >::const_iterator
vector< M, T >::end() const
{
    return array + M; ;
}



template< size_t M, typename T >
inline typename vector< M, T >::reverse_iterator
vector< M, T >::rbegin()
{
    return array + M - 1;
}


template< size_t M, typename T >
inline typename vector< M, T >::reverse_iterator
vector< M, T >::rend()
{
    return array - 1;
}


template< size_t M, typename T >
inline typename vector< M, T >::const_reverse_iterator
vector< M, T >::rbegin() const
{
    return array + M - 1;
}


template< size_t M, typename T >
inline typename vector< M, T >::const_reverse_iterator
vector< M, T >::rend() const
{
    return array - 1;
}

} // namespace vmml

#endif

