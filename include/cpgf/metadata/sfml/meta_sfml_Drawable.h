// Auto generated file, don't modify.

#ifndef __META_SFML_DRAWABLE_H
#define __META_SFML_DRAWABLE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindapi.h"


using namespace sf;
using namespace sf::Blend;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Global_drawable(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<Mode>(replaceName("Mode", _r))
        ._element(replaceName("Alpha", _r), sf::Blend::Alpha)
        ._element(replaceName("Add", _r), sf::Blend::Add)
        ._element(replaceName("Multiply", _r), sf::Blend::Multiply)
        ._element(replaceName("None", _r), sf::Blend::None)
    ;
}


template <typename D>
void buildMetaClass_Drawable(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetPosition", _r), (void (D::ClassType::*) (float, float))&D::ClassType::SetPosition);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetPosition", _r), (void (D::ClassType::*) (const Vector2f &))&D::ClassType::SetPosition);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetX", _r), &D::ClassType::SetX);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetY", _r), &D::ClassType::SetY);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetScale", _r), (void (D::ClassType::*) (float, float))&D::ClassType::SetScale);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetScale", _r), (void (D::ClassType::*) (const Vector2f &))&D::ClassType::SetScale);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetScaleX", _r), &D::ClassType::SetScaleX);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetScaleY", _r), &D::ClassType::SetScaleY);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetCenter", _r), (void (D::ClassType::*) (float, float))&D::ClassType::SetCenter);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetCenter", _r), (void (D::ClassType::*) (const Vector2f &))&D::ClassType::SetCenter);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetRotation", _r), &D::ClassType::SetRotation);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetColor", _r), &D::ClassType::SetColor);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetBlendMode", _r), &D::ClassType::SetBlendMode);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetPosition", _r), &D::ClassType::GetPosition);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetScale", _r), &D::ClassType::GetScale);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetCenter", _r), &D::ClassType::GetCenter);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetRotation", _r), &D::ClassType::GetRotation);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetColor", _r), &D::ClassType::GetColor);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetBlendMode", _r), &D::ClassType::GetBlendMode);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Move", _r), (void (D::ClassType::*) (float, float))&D::ClassType::Move);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Move", _r), (void (D::ClassType::*) (const Vector2f &))&D::ClassType::Move);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Scale", _r), (void (D::ClassType::*) (float, float))&D::ClassType::Scale);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Scale", _r), (void (D::ClassType::*) (const Vector2f &))&D::ClassType::Scale);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Rotate", _r), &D::ClassType::Rotate);
    _d.CPGF_MD_TEMPLATE _method(replaceName("TransformToLocal", _r), &D::ClassType::TransformToLocal);
    _d.CPGF_MD_TEMPLATE _method(replaceName("TransformToGlobal", _r), &D::ClassType::TransformToGlobal);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
