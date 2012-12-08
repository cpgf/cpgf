// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IMATERIALRENDERERSERVICES_H
#define __META_IRRLICHT_IMATERIALRENDERERSERVICES_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IMaterialRendererServices(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("setBasicRenderStates", &D::ClassType::setBasicRenderStates);
    _d.CPGF_MD_TEMPLATE _method("setVertexShaderConstant", (bool (D::ClassType::*) (const c8 *, const f32 *, int))&D::ClassType::setVertexShaderConstant);
    _d.CPGF_MD_TEMPLATE _method("setVertexShaderConstant", (bool (D::ClassType::*) (const c8 *, const bool *, int))&D::ClassType::setVertexShaderConstant);
    _d.CPGF_MD_TEMPLATE _method("setVertexShaderConstant", (bool (D::ClassType::*) (const c8 *, const s32 *, int))&D::ClassType::setVertexShaderConstant);
    _d.CPGF_MD_TEMPLATE _method("setVertexShaderConstant", (void (D::ClassType::*) (const f32 *, s32, s32))&D::ClassType::setVertexShaderConstant)
        ._default(copyVariantFromCopyable(1))
    ;
    _d.CPGF_MD_TEMPLATE _method("setPixelShaderConstant", (bool (D::ClassType::*) (const c8 *, const f32 *, int))&D::ClassType::setPixelShaderConstant);
    _d.CPGF_MD_TEMPLATE _method("setPixelShaderConstant", (bool (D::ClassType::*) (const c8 *, const bool *, int))&D::ClassType::setPixelShaderConstant);
    _d.CPGF_MD_TEMPLATE _method("setPixelShaderConstant", (bool (D::ClassType::*) (const c8 *, const s32 *, int))&D::ClassType::setPixelShaderConstant);
    _d.CPGF_MD_TEMPLATE _method("setPixelShaderConstant", (void (D::ClassType::*) (const f32 *, s32, s32))&D::ClassType::setPixelShaderConstant)
        ._default(copyVariantFromCopyable(1))
    ;
    _d.CPGF_MD_TEMPLATE _method("getVideoDriver", &D::ClassType::getVideoDriver);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
