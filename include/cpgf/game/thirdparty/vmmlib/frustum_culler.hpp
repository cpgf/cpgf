/* 
* VMMLib - Vector & Matrix Math Lib
*  
* @author Stefan Eilemann
*
* @license revised BSD license, check LICENSE
*/ 

#ifndef __VMML__FRUSTUM_CULLER__HPP__
#define __VMML__FRUSTUM_CULLER__HPP__

#include <vmmlib/vector.hpp>
#include <vmmlib/matrix.hpp>
#include <vmmlib/visibility.hpp>

// - declaration -

namespace vmml
{

/** Helper class for OpenGL view frustum culling. */
template< class T > 
class frustum_culler
{
public:
    typedef vector< 3, T >              vec3;
    typedef vector< 4, T >              vec4;

    // contructors
    frustum_culler() {}// warning: components NOT initialised ( for performance )
    ~frustum_culler(){}

    void setup( const matrix< 4, 4, T >& proj_modelview );
    Visibility test_sphere( const vector< 4, T >& sphere );

private:
    inline void _normalize_plane( vector< 4, T >& plane ) const;

    vec4    _left_plane;
    vec4    _right_plane;
    vec4    _bottom_plane;
    vec4    _top_plane;
    vec4    _near_plane;
    vec4    _far_plane;

}; // class frustum_culler


#ifndef VMMLIB_NO_TYPEDEFS

typedef frustum_culler< float >  frustum_cullerf;
typedef frustum_culler< double > frustum_cullerd;

#endif

} // namespace vmml

// - implementation - //


namespace vmml
{

/** 
 * Setup the culler by extracting the frustum planes from the projection
 * matrix. The projection matrix should contain the viewing transformation.
 */
template < class T > 
void frustum_culler< T >::setup( const matrix< 4, 4, T >& proj_modelview )
{
    // See http://www2.ravensoft.com/users/ggribb/plane%20extraction.pdf pp.5
    
    const vec4& row0 = proj_modelview.get_row( 0 );
    const vec4& row1 = proj_modelview.get_row( 1 );
    const vec4& row2 = proj_modelview.get_row( 2 );
    const vec4& row3 = proj_modelview.get_row( 3 );

    _left_plane   = row3 + row0;
    _right_plane  = row3 - row0;
    _bottom_plane = row3 + row1;
    _top_plane    = row3 - row1;
    _near_plane   = row3 + row2;
    _far_plane    = row3 - row2;
    
    _normalize_plane( _left_plane );
    _normalize_plane( _right_plane );
    _normalize_plane( _bottom_plane );
    _normalize_plane( _top_plane );
    _normalize_plane( _near_plane );
    _normalize_plane( _far_plane );

}



template < class T > 
inline void
frustum_culler< T >::_normalize_plane( vector< 4, T >& plane ) const
{
    const vec3& v3 = plane.template get_sub_vector< 3 >();
    const T len_i = 1.0 / v3.length();
    plane.x() *= len_i;
    plane.y() *= len_i;
    plane.z() *= len_i;
    plane.w() *= len_i;
}



template < class T > 
Visibility frustum_culler< T >::test_sphere( const vector< 4, T >& sphere )
{
    Visibility visibility = VISIBILITY_FULL;

    // see http://www.flipcode.com/articles/article_frustumculling.shtml
    // distance = plane.normal . sphere.center + plane.distance
    // Test all planes:
    // - if sphere behind plane: not visible
    // - if sphere intersects one plane: partially visible
    // - else: fully visible

    T distance = _left_plane.x() * sphere.x() +
                 _left_plane.y() * sphere.y() +
                 _left_plane.z() * sphere.z() + _left_plane.w();
    if( distance <= -sphere.w() )
        return VISIBILITY_NONE;
    if( distance < sphere.w() )
        visibility = VISIBILITY_PARTIAL;

    distance = _right_plane.x() * sphere.x() +
               _right_plane.y() * sphere.y() +
               _right_plane.z() * sphere.z() + _right_plane.w();
    if( distance <= -sphere.w() )
        return VISIBILITY_NONE;
    if( distance < sphere.w() )
        visibility = VISIBILITY_PARTIAL;

    distance = _bottom_plane.x() * sphere.x() +
               _bottom_plane.y() * sphere.y() +
               _bottom_plane.z() * sphere.z() + _bottom_plane.w();
    if( distance <= -sphere.w() )
        return VISIBILITY_NONE;
    if( distance < sphere.w() )
        visibility = VISIBILITY_PARTIAL;

    distance = _top_plane.x() * sphere.x() +
               _top_plane.y() * sphere.y() +
               _top_plane.z() * sphere.z() + _top_plane.w();
    if( distance <= -sphere.w() )
        return VISIBILITY_NONE;
    if( distance < sphere.w() )
        visibility = VISIBILITY_PARTIAL;

    distance = _near_plane.x() * sphere.x() +
               _near_plane.y() * sphere.y() +
               _near_plane.z() * sphere.z() + _near_plane.w();
    if( distance <= -sphere.w() )
        return VISIBILITY_NONE;
    if( distance < sphere.w() )
        visibility = VISIBILITY_PARTIAL;

    distance = _far_plane.x() * sphere.x() +
               _far_plane.y() * sphere.y() +
               _far_plane.z() * sphere.z() + _far_plane.w();
    if( distance <= -sphere.w() )
        return VISIBILITY_NONE;
    if( distance < sphere.w() )
        visibility = VISIBILITY_PARTIAL;

    return visibility;
}	


} // namespace vmml

#endif // include protection
