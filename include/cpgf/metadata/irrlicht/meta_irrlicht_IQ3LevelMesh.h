// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IQ3LEVELMESH_H
#define __META_IRRLICHT_IQ3LEVELMESH_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IQ3LevelMesh(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getShader", (const quake3::IShader * (D::ClassType::*) (const c8 *, bool))&D::ClassType::getShader)
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("getShader", (const quake3::IShader * (D::ClassType::*) (u32) const)&D::ClassType::getShader);
    _d.CPGF_MD_TEMPLATE _method("getEntityList", &D::ClassType::getEntityList);
    _d.CPGF_MD_TEMPLATE _method("getBrushEntityMesh", (IMesh * (D::ClassType::*) (s32) const)&D::ClassType::getBrushEntityMesh);
    _d.CPGF_MD_TEMPLATE _method("getBrushEntityMesh", (IMesh * (D::ClassType::*) (quake3::IEntity &) const)&D::ClassType::getBrushEntityMesh);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
