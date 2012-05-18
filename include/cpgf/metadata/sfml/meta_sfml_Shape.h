// Auto generated file, don't modify.

#ifndef __META_SFML_SHAPE_H
#define __META_SFML_SHAPE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Shape(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method(replaceName("AddPoint", _r), (void (D::ClassType::*) (float, float, const Color &, const Color &))&D::ClassType::AddPoint)
        ._default(copyVariantFromCopyable(Color(0, 0, 0)))
        ._default(copyVariantFromCopyable(Color(255, 255, 255)))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("AddPoint", _r), (void (D::ClassType::*) (const Vector2f &, const Color &, const Color &))&D::ClassType::AddPoint)
        ._default(copyVariantFromCopyable(Color(0, 0, 0)))
        ._default(copyVariantFromCopyable(Color(255, 255, 255)))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetNbPoints", _r), &D::ClassType::GetNbPoints);
    _d.CPGF_MD_TEMPLATE _method(replaceName("EnableFill", _r), &D::ClassType::EnableFill);
    _d.CPGF_MD_TEMPLATE _method(replaceName("EnableOutline", _r), &D::ClassType::EnableOutline);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetPointPosition", _r), (void (D::ClassType::*) (unsigned int, const Vector2f &))&D::ClassType::SetPointPosition);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetPointPosition", _r), (void (D::ClassType::*) (unsigned int, float, float))&D::ClassType::SetPointPosition);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetPointColor", _r), &D::ClassType::SetPointColor);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetPointOutlineColor", _r), &D::ClassType::SetPointOutlineColor);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetOutlineWidth", _r), &D::ClassType::SetOutlineWidth);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetPointPosition", _r), &D::ClassType::GetPointPosition);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetPointColor", _r), &D::ClassType::GetPointColor);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetPointOutlineColor", _r), &D::ClassType::GetPointOutlineColor);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetOutlineWidth", _r), &D::ClassType::GetOutlineWidth);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Line", _r), (Shape (*) (float, float, float, float, float, const Color &, float, const Color &))&D::ClassType::Line)
        ._default(copyVariantFromCopyable(sf::Color(0, 0, 0)))
        ._default(copyVariantFromCopyable(0.f))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("Line", _r), (Shape (*) (const Vector2f &, const Vector2f &, float, const Color &, float, const Color &))&D::ClassType::Line)
        ._default(copyVariantFromCopyable(sf::Color(0, 0, 0)))
        ._default(copyVariantFromCopyable(0.f))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("Rectangle", _r), (Shape (*) (float, float, float, float, const Color &, float, const Color &))&D::ClassType::Rectangle)
        ._default(copyVariantFromCopyable(sf::Color(0, 0, 0)))
        ._default(copyVariantFromCopyable(0.f))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("Rectangle", _r), (Shape (*) (const Vector2f &, const Vector2f &, const Color &, float, const Color &))&D::ClassType::Rectangle)
        ._default(copyVariantFromCopyable(sf::Color(0, 0, 0)))
        ._default(copyVariantFromCopyable(0.f))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("Circle", _r), (Shape (*) (float, float, float, const Color &, float, const Color &))&D::ClassType::Circle)
        ._default(copyVariantFromCopyable(sf::Color(0, 0, 0)))
        ._default(copyVariantFromCopyable(0.f))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("Circle", _r), (Shape (*) (const Vector2f &, float, const Color &, float, const Color &))&D::ClassType::Circle)
        ._default(copyVariantFromCopyable(sf::Color(0, 0, 0)))
        ._default(copyVariantFromCopyable(0.f))
    ;
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
