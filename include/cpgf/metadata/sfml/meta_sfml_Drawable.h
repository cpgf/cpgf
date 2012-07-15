// Auto generated file, don't modify.

#ifndef __META_SFML_DRAWABLE_H
#define __META_SFML_DRAWABLE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;
using namespace sf::Blend;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Global_drawable(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<Mode>("Mode")
        ._element("Alpha", sf::Blend::Alpha)
        ._element("Add", sf::Blend::Add)
        ._element("Multiply", sf::Blend::Multiply)
        ._element("None", sf::Blend::None)
    ;
}


template <typename D>
void buildMetaClass_Drawable(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("SetPosition", (void (D::ClassType::*) (float, float))&D::ClassType::SetPosition);
    _d.CPGF_MD_TEMPLATE _method("SetPosition", (void (D::ClassType::*) (const Vector2f &))&D::ClassType::SetPosition);
    _d.CPGF_MD_TEMPLATE _method("SetX", &D::ClassType::SetX);
    _d.CPGF_MD_TEMPLATE _method("SetY", &D::ClassType::SetY);
    _d.CPGF_MD_TEMPLATE _method("SetScale", (void (D::ClassType::*) (float, float))&D::ClassType::SetScale);
    _d.CPGF_MD_TEMPLATE _method("SetScale", (void (D::ClassType::*) (const Vector2f &))&D::ClassType::SetScale);
    _d.CPGF_MD_TEMPLATE _method("SetScaleX", &D::ClassType::SetScaleX);
    _d.CPGF_MD_TEMPLATE _method("SetScaleY", &D::ClassType::SetScaleY);
    _d.CPGF_MD_TEMPLATE _method("SetCenter", (void (D::ClassType::*) (float, float))&D::ClassType::SetCenter);
    _d.CPGF_MD_TEMPLATE _method("SetCenter", (void (D::ClassType::*) (const Vector2f &))&D::ClassType::SetCenter);
    _d.CPGF_MD_TEMPLATE _method("SetRotation", &D::ClassType::SetRotation);
    _d.CPGF_MD_TEMPLATE _method("SetColor", &D::ClassType::SetColor);
    _d.CPGF_MD_TEMPLATE _method("SetBlendMode", &D::ClassType::SetBlendMode);
    _d.CPGF_MD_TEMPLATE _method("GetPosition", &D::ClassType::GetPosition);
    _d.CPGF_MD_TEMPLATE _method("GetScale", &D::ClassType::GetScale);
    _d.CPGF_MD_TEMPLATE _method("GetCenter", &D::ClassType::GetCenter);
    _d.CPGF_MD_TEMPLATE _method("GetRotation", &D::ClassType::GetRotation);
    _d.CPGF_MD_TEMPLATE _method("GetColor", &D::ClassType::GetColor);
    _d.CPGF_MD_TEMPLATE _method("GetBlendMode", &D::ClassType::GetBlendMode);
    _d.CPGF_MD_TEMPLATE _method("Move", (void (D::ClassType::*) (float, float))&D::ClassType::Move);
    _d.CPGF_MD_TEMPLATE _method("Move", (void (D::ClassType::*) (const Vector2f &))&D::ClassType::Move);
    _d.CPGF_MD_TEMPLATE _method("Scale", (void (D::ClassType::*) (float, float))&D::ClassType::Scale);
    _d.CPGF_MD_TEMPLATE _method("Scale", (void (D::ClassType::*) (const Vector2f &))&D::ClassType::Scale);
    _d.CPGF_MD_TEMPLATE _method("Rotate", &D::ClassType::Rotate);
    _d.CPGF_MD_TEMPLATE _method("TransformToLocal", &D::ClassType::TransformToLocal);
    _d.CPGF_MD_TEMPLATE _method("TransformToGlobal", &D::ClassType::TransformToGlobal);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
