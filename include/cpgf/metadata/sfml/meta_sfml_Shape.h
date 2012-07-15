// Auto generated file, don't modify.

#ifndef __META_SFML_SHAPE_H
#define __META_SFML_SHAPE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Shape(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method("AddPoint", (void (D::ClassType::*) (float, float, const Color &, const Color &))&D::ClassType::AddPoint)
        ._default(copyVariantFromCopyable(Color(0, 0, 0)))
        ._default(copyVariantFromCopyable(Color(255, 255, 255)))
    ;
    _d.CPGF_MD_TEMPLATE _method("AddPoint", (void (D::ClassType::*) (const Vector2f &, const Color &, const Color &))&D::ClassType::AddPoint)
        ._default(copyVariantFromCopyable(Color(0, 0, 0)))
        ._default(copyVariantFromCopyable(Color(255, 255, 255)))
    ;
    _d.CPGF_MD_TEMPLATE _method("GetNbPoints", &D::ClassType::GetNbPoints);
    _d.CPGF_MD_TEMPLATE _method("EnableFill", &D::ClassType::EnableFill);
    _d.CPGF_MD_TEMPLATE _method("EnableOutline", &D::ClassType::EnableOutline);
    _d.CPGF_MD_TEMPLATE _method("SetPointPosition", (void (D::ClassType::*) (unsigned int, const Vector2f &))&D::ClassType::SetPointPosition);
    _d.CPGF_MD_TEMPLATE _method("SetPointPosition", (void (D::ClassType::*) (unsigned int, float, float))&D::ClassType::SetPointPosition);
    _d.CPGF_MD_TEMPLATE _method("SetPointColor", &D::ClassType::SetPointColor);
    _d.CPGF_MD_TEMPLATE _method("SetPointOutlineColor", &D::ClassType::SetPointOutlineColor);
    _d.CPGF_MD_TEMPLATE _method("SetOutlineWidth", &D::ClassType::SetOutlineWidth);
    _d.CPGF_MD_TEMPLATE _method("GetPointPosition", &D::ClassType::GetPointPosition);
    _d.CPGF_MD_TEMPLATE _method("GetPointColor", &D::ClassType::GetPointColor);
    _d.CPGF_MD_TEMPLATE _method("GetPointOutlineColor", &D::ClassType::GetPointOutlineColor);
    _d.CPGF_MD_TEMPLATE _method("GetOutlineWidth", &D::ClassType::GetOutlineWidth);
    _d.CPGF_MD_TEMPLATE _method("Line", (Shape (*) (float, float, float, float, float, const Color &, float, const Color &))&D::ClassType::Line)
        ._default(copyVariantFromCopyable(sf::Color(0, 0, 0)))
        ._default(copyVariantFromCopyable(0.f))
    ;
    _d.CPGF_MD_TEMPLATE _method("Line", (Shape (*) (const Vector2f &, const Vector2f &, float, const Color &, float, const Color &))&D::ClassType::Line)
        ._default(copyVariantFromCopyable(sf::Color(0, 0, 0)))
        ._default(copyVariantFromCopyable(0.f))
    ;
    _d.CPGF_MD_TEMPLATE _method("Rectangle", (Shape (*) (float, float, float, float, const Color &, float, const Color &))&D::ClassType::Rectangle)
        ._default(copyVariantFromCopyable(sf::Color(0, 0, 0)))
        ._default(copyVariantFromCopyable(0.f))
    ;
    _d.CPGF_MD_TEMPLATE _method("Rectangle", (Shape (*) (const Vector2f &, const Vector2f &, const Color &, float, const Color &))&D::ClassType::Rectangle)
        ._default(copyVariantFromCopyable(sf::Color(0, 0, 0)))
        ._default(copyVariantFromCopyable(0.f))
    ;
    _d.CPGF_MD_TEMPLATE _method("Circle", (Shape (*) (float, float, float, const Color &, float, const Color &))&D::ClassType::Circle)
        ._default(copyVariantFromCopyable(sf::Color(0, 0, 0)))
        ._default(copyVariantFromCopyable(0.f))
    ;
    _d.CPGF_MD_TEMPLATE _method("Circle", (Shape (*) (const Vector2f &, float, const Color &, float, const Color &))&D::ClassType::Circle)
        ._default(copyVariantFromCopyable(sf::Color(0, 0, 0)))
        ._default(copyVariantFromCopyable(0.f))
    ;
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
