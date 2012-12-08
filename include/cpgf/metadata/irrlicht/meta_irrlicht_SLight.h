// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_SLIGHT_H
#define __META_IRRLICHT_SLIGHT_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_slight(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("LightTypeNames", &LightTypeNames);
    _d.CPGF_MD_TEMPLATE _enum<E_LIGHT_TYPE>("E_LIGHT_TYPE")
        ._element("ELT_POINT", irr::video::ELT_POINT)
        ._element("ELT_SPOT", irr::video::ELT_SPOT)
        ._element("ELT_DIRECTIONAL", irr::video::ELT_DIRECTIONAL)
        ._element("ELT_COUNT", irr::video::ELT_COUNT)
    ;
}


inline bool bItFiEldWrapper_SLight_CastShadows_getter(SLight * self) {
    return self->CastShadows;
}

inline void bItFiEldWrapper_SLight_CastShadows_setter(SLight * self, bool value) {
    self->CastShadows = value;
}


template <typename D>
void buildMetaClass_SLight(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("AmbientColor", &D::ClassType::AmbientColor);
    _d.CPGF_MD_TEMPLATE _field("DiffuseColor", &D::ClassType::DiffuseColor);
    _d.CPGF_MD_TEMPLATE _field("SpecularColor", &D::ClassType::SpecularColor);
    _d.CPGF_MD_TEMPLATE _field("Attenuation", &D::ClassType::Attenuation);
    _d.CPGF_MD_TEMPLATE _field("OuterCone", &D::ClassType::OuterCone);
    _d.CPGF_MD_TEMPLATE _field("InnerCone", &D::ClassType::InnerCone);
    _d.CPGF_MD_TEMPLATE _field("Falloff", &D::ClassType::Falloff);
    _d.CPGF_MD_TEMPLATE _field("Position", &D::ClassType::Position);
    _d.CPGF_MD_TEMPLATE _field("Direction", &D::ClassType::Direction);
    _d.CPGF_MD_TEMPLATE _field("Radius", &D::ClassType::Radius);
    _d.CPGF_MD_TEMPLATE _field("Type", &D::ClassType::Type);
    _d.CPGF_MD_TEMPLATE _property("CastShadows", &bItFiEldWrapper_SLight_CastShadows_getter, &bItFiEldWrapper_SLight_CastShadows_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
