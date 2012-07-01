/* 
* VMMLib - Vector & Matrix Math Lib
*  
* @author Stefan Eilemann
*
* @license revised BSD license, check LICENSE
*/ 

#ifndef __VMML__FRUSTUM__HPP__
#define __VMML__FRUSTUM__HPP__

#include <vmmlib/vmmlib_config.hpp>
#include <vmmlib/matrix.hpp>

#include <cmath>

// - declaration -

namespace vmml
{

template< typename T > 
class frustum
{
public:
    VMMLIB_ALIGN( T array[6] );
       
    // contructors
    frustum(); // warning: components NOT initialised ( for performance )
    frustum( const T left, const T right, const T bottom, const T top,
             const T near_plane, const T far_plane );

    template< typename U >
    frustum( const frustum< U >& source_ );
        
    //the pointer 'values' must be a valid 6 component c array of the resp. type
    template< typename U >
    frustum( const U* values );

    ~frustum();
    
    const frustum& operator=( const frustum& source_ );
    template< typename U >
    void operator=( const frustum< U >& source_ );

    void set( const T _left, const T _right, const T _bottom, 
        const T _top, const T _near, const T _far );
        
    // set the frustum using the same parameters as gluPerspective. 
    void set_perspective( T field_of_view_y, T aspect_ratio, T near_plane_,
        T far_plane );
             
    matrix< 4, 4, T > compute_matrix() const;
    matrix< 4, 4, T > compute_ortho_matrix() const;

    void compute_matrix( matrix< 4, 4, T >& matrix_ ) const;
    void compute_ortho_matrix( matrix< 4, 4, T >& matrix_ ) const;
    void apply_jitter( const vector< 2, T >& jitter_ );

    // 'move' the frustum. this function changes the near_plane, and adjusts the
    // other parameters in a way that the 'perspective pyramid' stays the same.
    void adjust_near( const T near_plane );

    inline T& left();
    inline const T& left() const;

    inline T& right();
    inline const T& right() const;

    inline T& bottom();
    inline const T& bottom() const;

    inline T& top();
    inline const T& top() const;

    inline T& near_plane();
    inline const T& near_plane() const;

    inline T& far_plane();
    inline const T& far_plane() const;

    inline T get_width() const;
    inline T get_height() const;

    friend std::ostream& operator << ( std::ostream& os, const frustum& frustum_ )
    {
        const std::ios::fmtflags flags = os.flags();
        const int                prec  = os.precision();

        os.setf( std::ios::right, std::ios::adjustfield );
        os.precision( 5 );
        os << "[" << std::setw(10) << frustum_.left() << " " 
           << std::setw(10) << frustum_.right()  << " " 
           << std::setw(10) << frustum_.bottom() << " " 
           << std::setw(10) << frustum_.top()    << " " 
           << std::setw(10) << frustum_.near_plane()   << " " 
           << std::setw(10) << frustum_.far_plane()    << "]";
        os.precision( prec );
        os.setf( flags );
        return os;
    };  

    static const frustum DEFAULT;
};

#ifndef VMMLIB_NO_TYPEDEFS
typedef frustum< float >  frustumf;
typedef frustum< double > frustumd;
#endif

} // namespace vmml

// - implementation - //

namespace vmml
{

template< typename T > 
const frustum< T > frustum< T >::DEFAULT( static_cast< T >( -1.0 ),
                                          static_cast< T >( 1.0 ), 
                                          static_cast< T >( -1.0 ),
                                          static_cast< T >( 1.0 ), 
                                          static_cast< T >( 0.1 ), 
                                          static_cast< T >( 100.0 ) );



template < typename T > 
frustum< T >::frustum() 
{} 



template < typename T > 
frustum<T>::frustum( const T _left, const T _right, const T _bottom, 
                     const T _top, const T _near, const T _far )
{
    set( _left, _right, _bottom, _top, _near, _far );
} 


template < typename T > 
template< typename U >
frustum< T >::frustum( const frustum< U >& source_ )
{
    (*this) = source_;
}



template < typename T > 
template< typename U >
frustum< T >::frustum( const U* values )
{
    assert( values && 
            "frustum: Nullpointer argument as source for initialisation!" );
    left()      = static_cast< T > ( values[0] );
    right()     = static_cast< T > ( values[1] );
    bottom()    = static_cast< T > ( values[2] );
    top()       = static_cast< T > ( values[3] );
    near_plane() = static_cast< T > ( values[4] );
    far_plane()  = static_cast< T > ( values[5] );
}



template < typename T > 
frustum< T >::~frustum()
{}



template< typename T >
const frustum< T >&
frustum< T >::operator=( const frustum& source_ )
{
    memcpy( array, source_.array, 6 * sizeof( T ) );
    return *this;
}



template< typename T >
template< typename U >
void
frustum< T >::operator=( const frustum< U >& source_ )
{
    for( size_t index = 0; index < 6; ++index )
    {
        array[ index ] = static_cast< T >( source_.array[ index ] );
    }
}



template < typename T > 
void 
frustum< T >::set( const T _left, const T _right, const T _bottom, 
    const T _top, const T _near, const T _far )
{
    left()      = _left;
    right()     = _right;
    bottom()    = _bottom;
    top()       = _top;
    near_plane() = _near;
    far_plane()  = _far;
}


// 'move' the frustum. this function changes the near_plane, and adjusts the
// other parameters in a way that the 'perspective pyramid' stays the same.
template < typename T > 
void
frustum<T>::adjust_near( const T new_near )
{
	if( new_near == near_plane() )
		return;

	const T ratio = new_near / near_plane();
	right()     *= ratio;
	left()      *= ratio;
	top()       *= ratio;
	bottom()    *= ratio;
	near_plane()  = new_near;
}



// set the frustum using the same parameters as gluPerspective. 
template < typename T > 
void
frustum<T>::set_perspective( T fov_y, T aspect_ratio, T near_plane_,
    T far_plane_ )
{
    near_plane() = near_plane_;
    far_plane()   = far_plane_;
    
    top()       = tan( 0.5 * fov_y * M_PI / 180.0 ) * 0.5;
    bottom()    = - top();
    
    left()      = bottom() * aspect_ratio;
    right()     = top() * aspect_ratio;    
}



template < typename T > 
matrix< 4, 4, T >
frustum<T>::compute_matrix() const
{
    matrix< 4, 4, T > matrix_;
    compute_matrix( matrix_ );
    return matrix_;
}



template < typename T > 
void
frustum<T>::compute_matrix( matrix< 4, 4, T >& M ) const
{
    M( 0,0 ) = 2.0 * near_plane() / ( right() - left() );
    M( 0,1 ) = 0.0;
    M( 0,2 ) = ( right() + left() ) / ( right() - left() );
    M( 0,3 ) = 0.0;
    
    M( 1,0 ) = 0.0;
    M( 1,1 ) = 2.0 * near_plane() / ( top() - bottom() );
    M( 1,2 ) = ( top() + bottom() ) / ( top() - bottom() );
    M( 1,3 ) = 0.0;

    M( 2,0 ) = 0.0;
    M( 2,1 ) = 0.0;
    // NOTE: Some glfrustum man pages say wrongly '(far + near) / (far - near)'
    M( 2,2 ) = -( far_plane() + near_plane() ) / ( far_plane() - near_plane() );
    M( 2,3 ) = -2.0 * far_plane() * near_plane() / ( far_plane() - near_plane() );

    M( 3,0 ) = 0.0;
    M( 3,1 ) = 0.0;
    M( 3,2 ) = -1.0;
    M( 3,3 ) =  0.0;
}



template < typename T > 
matrix< 4, 4, T >
frustum< T >::compute_ortho_matrix() const
{
    matrix< 4, 4, T > matrix_;
    compute_ortho_matrix( matrix_ );
    return matrix_;
}



template < typename T > 
void
frustum< T >::compute_ortho_matrix( matrix< 4, 4, T >& M ) const
{
    M( 0,0 ) = 2.0 / ( right() - left() );
    M( 0,1 ) = 0.0;
    M( 0,2 ) = 0.0;
    M( 0,3 ) = -( right() + left() ) / ( right() - left() );
    
    M( 1,0 ) = 0.0;
    M( 1,1 ) = 2.0 / ( top() - bottom() );
    M( 1,2 ) = 0.0f;
    M( 1,3 ) = -( top() + bottom() ) / ( top() - bottom() );

    M( 2,0 ) = 0.0;
    M( 2,1 ) = 0.0;
    M( 2,2 ) = -2.0 / ( far_plane() - near_plane() );
    M( 2,3 ) = -( far_plane() + near_plane() ) / ( far_plane() - near_plane() );

    M( 3,0 ) = 0.0;
    M( 3,1 ) = 0.0;
    M( 3,2 ) = 0.0;
    M( 3,3 ) = 1.0f;
}

template < typename T >
void
frustum< T >::apply_jitter( const vector< 2, T >& jitter_ )
{
    left()   = left() + jitter_.x();
    right()  = right() + jitter_.x();
    bottom() = bottom() + jitter_.y();
    top()    = top() + jitter_.y();
}

template< typename T >
inline T&
frustum< T >::left()
{
    return array[ 0 ];
}



template< typename T >
inline const T&
frustum< T >::left() const
{
    return array[ 0 ];
}



template< typename T >
inline T&
frustum< T >::right()
{
    return array[ 1 ];
}



template< typename T >
inline const T&
frustum< T >::right() const
{
    return array[ 1 ];
}



template< typename T >
inline T&
frustum< T >::bottom()
{
    return array[ 2 ];
}



template< typename T >
inline const T&
frustum< T >::bottom() const
{
    return array[ 2 ];
}



template< typename T >
inline T&
frustum< T >::top()
{
    return array[ 3 ];
}



template< typename T >
inline const T&
frustum< T >::top() const
{
    return array[ 3 ];
}



template< typename T >
inline T&
frustum< T >::near_plane()
{
    return array[ 4 ];
}



template< typename T >
inline const T&
frustum< T >::near_plane() const
{
    return array[ 4 ];
}



template< typename T >
inline T&
frustum< T >::far_plane()
{
    return array[ 5 ];
}



template< typename T >
inline const T&
frustum< T >::far_plane() const
{
    return array[ 5 ];
}

template< typename T > inline T frustum< T >::get_width() const
{
    return fabs( right() - left( ));
}

template< typename T > inline T frustum< T >::get_height() const
{
    return fabs( top() - bottom( ));
}


} //namespace vmml

#endif
