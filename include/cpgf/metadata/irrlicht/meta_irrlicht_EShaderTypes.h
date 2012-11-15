// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ESHADERTYPES_H
#define __META_IRRLICHT_ESHADERTYPES_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_eshadertypes(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("VERTEX_SHADER_TYPE_NAMES", &VERTEX_SHADER_TYPE_NAMES);
    _d.CPGF_MD_TEMPLATE _field("PIXEL_SHADER_TYPE_NAMES", &PIXEL_SHADER_TYPE_NAMES);
    _d.CPGF_MD_TEMPLATE _field("GEOMETRY_SHADER_TYPE_NAMES", &GEOMETRY_SHADER_TYPE_NAMES);
    _d.CPGF_MD_TEMPLATE _enum<E_VERTEX_SHADER_TYPE>("E_VERTEX_SHADER_TYPE")
        ._element("EVST_VS_1_1", irr::video::EVST_VS_1_1)
        ._element("EVST_VS_2_0", irr::video::EVST_VS_2_0)
        ._element("EVST_VS_2_a", irr::video::EVST_VS_2_a)
        ._element("EVST_VS_3_0", irr::video::EVST_VS_3_0)
        ._element("EVST_VS_4_0", irr::video::EVST_VS_4_0)
        ._element("EVST_VS_4_1", irr::video::EVST_VS_4_1)
        ._element("EVST_VS_5_0", irr::video::EVST_VS_5_0)
        ._element("EVST_COUNT", irr::video::EVST_COUNT)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_PIXEL_SHADER_TYPE>("E_PIXEL_SHADER_TYPE")
        ._element("EPST_PS_1_1", irr::video::EPST_PS_1_1)
        ._element("EPST_PS_1_2", irr::video::EPST_PS_1_2)
        ._element("EPST_PS_1_3", irr::video::EPST_PS_1_3)
        ._element("EPST_PS_1_4", irr::video::EPST_PS_1_4)
        ._element("EPST_PS_2_0", irr::video::EPST_PS_2_0)
        ._element("EPST_PS_2_a", irr::video::EPST_PS_2_a)
        ._element("EPST_PS_2_b", irr::video::EPST_PS_2_b)
        ._element("EPST_PS_3_0", irr::video::EPST_PS_3_0)
        ._element("EPST_PS_4_0", irr::video::EPST_PS_4_0)
        ._element("EPST_PS_4_1", irr::video::EPST_PS_4_1)
        ._element("EPST_PS_5_0", irr::video::EPST_PS_5_0)
        ._element("EPST_COUNT", irr::video::EPST_COUNT)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_GEOMETRY_SHADER_TYPE>("E_GEOMETRY_SHADER_TYPE")
        ._element("EGST_GS_4_0", irr::video::EGST_GS_4_0)
        ._element("EGST_COUNT", irr::video::EGST_COUNT)
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
