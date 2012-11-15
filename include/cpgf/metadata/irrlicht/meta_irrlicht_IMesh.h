// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IMESH_H
#define __META_IRRLICHT_IMESH_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IMesh(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getMeshBufferCount", &D::ClassType::getMeshBufferCount);
    _d.CPGF_MD_TEMPLATE _method("getMeshBuffer", (IMeshBuffer * (D::ClassType::*) (u32) const)&D::ClassType::getMeshBuffer);
    _d.CPGF_MD_TEMPLATE _method("getMeshBuffer", (IMeshBuffer * (D::ClassType::*) (const video::SMaterial &) const)&D::ClassType::getMeshBuffer);
    _d.CPGF_MD_TEMPLATE _method("getBoundingBox", &D::ClassType::getBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("setBoundingBox", &D::ClassType::setBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("setMaterialFlag", &D::ClassType::setMaterialFlag);
    _d.CPGF_MD_TEMPLATE _method("setHardwareMappingHint", &D::ClassType::setHardwareMappingHint)
        ._default(copyVariantFromCopyable(EBT_VERTEX_AND_INDEX))
    ;
    _d.CPGF_MD_TEMPLATE _method("setDirty", &D::ClassType::setDirty)
        ._default(copyVariantFromCopyable(EBT_VERTEX_AND_INDEX))
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
