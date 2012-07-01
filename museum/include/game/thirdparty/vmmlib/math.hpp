#ifndef __VMML__MATH__HPP__
#define __VMML__MATH__HPP__

#include <cmath>

namespace vmml
{

namespace math
{
// helpers for certain cmath functions


template< class T >
inline T squared( const T a )
{
    return ( a == 0.0 ) ? 0.0 : a * a;
}



/*
 * Computes (a2 + b2)1/2 without destructive underflow or overflow.
 */
template< class T >
inline T pythag( T a, T b )
{
    a = fabs(a);
    b = fabs(b);
    if ( a > b )
        return a * sqrt( 1.0 + squared( b / a ) );
    else
        return ( b == 0.0 ) ? 0.0 : b * sqrt( 1.0 + squared( a / b ) );
}



template< class T >
inline T sign( T a, T b )
{
    return ( b >= 0.0 ) ? fabs( a ) : -fabs( a );
}



template< typename T >
struct abs_less
{
    T operator()( const T& a, const T& b )
    {
        return fabs(a) < fabs( b );
    }
};


template< typename T >
struct abs_greater
{
    T operator()( const T& a, const T& b )
    {
        return fabs(a) > fabs( b );
    }
};


} // namespace math

} // namespace vmml

#endif

