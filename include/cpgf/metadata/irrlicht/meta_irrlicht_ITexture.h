// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ITEXTURE_H
#define __META_IRRLICHT_ITEXTURE_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_itexture(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<E_TEXTURE_CREATION_FLAG>("E_TEXTURE_CREATION_FLAG")
        ._element("ETCF_ALWAYS_16_BIT", irr::video::ETCF_ALWAYS_16_BIT)
        ._element("ETCF_ALWAYS_32_BIT", irr::video::ETCF_ALWAYS_32_BIT)
        ._element("ETCF_OPTIMIZED_FOR_QUALITY", irr::video::ETCF_OPTIMIZED_FOR_QUALITY)
        ._element("ETCF_OPTIMIZED_FOR_SPEED", irr::video::ETCF_OPTIMIZED_FOR_SPEED)
        ._element("ETCF_CREATE_MIP_MAPS", irr::video::ETCF_CREATE_MIP_MAPS)
        ._element("ETCF_NO_ALPHA_CHANNEL", irr::video::ETCF_NO_ALPHA_CHANNEL)
        ._element("ETCF_ALLOW_NON_POWER_2", irr::video::ETCF_ALLOW_NON_POWER_2)
        ._element("ETCF_FORCE_32_BIT_DO_NOT_USE", irr::video::ETCF_FORCE_32_BIT_DO_NOT_USE)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_TEXTURE_LOCK_MODE>("E_TEXTURE_LOCK_MODE")
        ._element("ETLM_READ_WRITE", irr::video::ETLM_READ_WRITE)
        ._element("ETLM_READ_ONLY", irr::video::ETLM_READ_ONLY)
        ._element("ETLM_WRITE_ONLY", irr::video::ETLM_WRITE_ONLY)
    ;
}


template <typename D>
void buildMetaClass_ITexture(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("lock", &D::ClassType::lock)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(ETLM_READ_WRITE))
    ;
    _d.CPGF_MD_TEMPLATE _method("unlock", &D::ClassType::unlock);
    _d.CPGF_MD_TEMPLATE _method("getOriginalSize", &D::ClassType::getOriginalSize);
    _d.CPGF_MD_TEMPLATE _method("getSize", &D::ClassType::getSize);
    _d.CPGF_MD_TEMPLATE _method("getDriverType", &D::ClassType::getDriverType);
    _d.CPGF_MD_TEMPLATE _method("getColorFormat", &D::ClassType::getColorFormat);
    _d.CPGF_MD_TEMPLATE _method("getPitch", &D::ClassType::getPitch);
    _d.CPGF_MD_TEMPLATE _method("hasMipMaps", &D::ClassType::hasMipMaps);
    _d.CPGF_MD_TEMPLATE _method("hasAlpha", &D::ClassType::hasAlpha);
    _d.CPGF_MD_TEMPLATE _method("regenerateMipMapLevels", &D::ClassType::regenerateMipMapLevels)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("isRenderTarget", &D::ClassType::isRenderTarget);
    _d.CPGF_MD_TEMPLATE _method("getName", &D::ClassType::getName);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
