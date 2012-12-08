// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_EATTRIBUTES_H
#define __META_IRRLICHT_EATTRIBUTES_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::io;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_eattributes(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<E_ATTRIBUTE_TYPE>("E_ATTRIBUTE_TYPE")
        ._element("EAT_INT", irr::io::EAT_INT)
        ._element("EAT_FLOAT", irr::io::EAT_FLOAT)
        ._element("EAT_STRING", irr::io::EAT_STRING)
        ._element("EAT_BOOL", irr::io::EAT_BOOL)
        ._element("EAT_ENUM", irr::io::EAT_ENUM)
        ._element("EAT_COLOR", irr::io::EAT_COLOR)
        ._element("EAT_COLORF", irr::io::EAT_COLORF)
        ._element("EAT_VECTOR3D", irr::io::EAT_VECTOR3D)
        ._element("EAT_POSITION2D", irr::io::EAT_POSITION2D)
        ._element("EAT_VECTOR2D", irr::io::EAT_VECTOR2D)
        ._element("EAT_RECT", irr::io::EAT_RECT)
        ._element("EAT_MATRIX", irr::io::EAT_MATRIX)
        ._element("EAT_QUATERNION", irr::io::EAT_QUATERNION)
        ._element("EAT_BBOX", irr::io::EAT_BBOX)
        ._element("EAT_PLANE", irr::io::EAT_PLANE)
        ._element("EAT_TRIANGLE3D", irr::io::EAT_TRIANGLE3D)
        ._element("EAT_LINE2D", irr::io::EAT_LINE2D)
        ._element("EAT_LINE3D", irr::io::EAT_LINE3D)
        ._element("EAT_STRINGWARRAY", irr::io::EAT_STRINGWARRAY)
        ._element("EAT_FLOATARRAY", irr::io::EAT_FLOATARRAY)
        ._element("EAT_INTARRAY", irr::io::EAT_INTARRAY)
        ._element("EAT_BINARY", irr::io::EAT_BINARY)
        ._element("EAT_TEXTURE", irr::io::EAT_TEXTURE)
        ._element("EAT_USER_POINTER", irr::io::EAT_USER_POINTER)
        ._element("EAT_DIMENSION2D", irr::io::EAT_DIMENSION2D)
        ._element("EAT_COUNT", irr::io::EAT_COUNT)
        ._element("EAT_UNKNOWN", irr::io::EAT_UNKNOWN)
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
