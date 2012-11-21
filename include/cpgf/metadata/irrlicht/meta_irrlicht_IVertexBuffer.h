// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IVERTEXBUFFER_H
#define __META_IRRLICHT_IVERTEXBUFFER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


inline video::S3DVertex & opErAToRWrapper_IVertexBuffer__opArrayGet(const IVertexBuffer * self, const u32 index) {
    return (*self)[index];
}
inline void opErAToRWrapper_IVertexBuffer__opArraySet(const IVertexBuffer * self, const u32 index, const cpgf::RemoveReference<video::S3DVertex & >::Result & OpsEt_vALue) {
    (*self)[index] = OpsEt_vALue;
}


template <typename D>
void buildMetaClass_IVertexBuffer(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getData", &D::ClassType::getData);
    _d.CPGF_MD_TEMPLATE _method("getType", &D::ClassType::getType);
    _d.CPGF_MD_TEMPLATE _method("setType", &D::ClassType::setType);
    _d.CPGF_MD_TEMPLATE _method("stride", &D::ClassType::stride);
    _d.CPGF_MD_TEMPLATE _method("size", &D::ClassType::size);
    _d.CPGF_MD_TEMPLATE _method("push_back", &D::ClassType::push_back);
    _d.CPGF_MD_TEMPLATE _method("getLast", &D::ClassType::getLast);
    _d.CPGF_MD_TEMPLATE _method("set_used", &D::ClassType::set_used);
    _d.CPGF_MD_TEMPLATE _method("reallocate", &D::ClassType::reallocate);
    _d.CPGF_MD_TEMPLATE _method("allocated_size", &D::ClassType::allocated_size);
    _d.CPGF_MD_TEMPLATE _method("pointer", &D::ClassType::pointer);
    _d.CPGF_MD_TEMPLATE _method("getHardwareMappingHint", &D::ClassType::getHardwareMappingHint);
    _d.CPGF_MD_TEMPLATE _method("setHardwareMappingHint", &D::ClassType::setHardwareMappingHint);
    _d.CPGF_MD_TEMPLATE _method("setDirty", &D::ClassType::setDirty);
    _d.CPGF_MD_TEMPLATE _method("getChangedID", &D::ClassType::getChangedID);
    _d.CPGF_MD_TEMPLATE _operator<video::S3DVertex & (*)(const cpgf::GMetaSelf &, const u32)>(mopHolder[0]);
    _d.CPGF_MD_TEMPLATE _method("_opArrayGet", (video::S3DVertex & (*) (const IVertexBuffer *, const u32))&opErAToRWrapper_IVertexBuffer__opArrayGet, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _method("_opArraySet", (void (*) (const IVertexBuffer *, const u32, const cpgf::RemoveReference<video::S3DVertex & >::Result &))&opErAToRWrapper_IVertexBuffer__opArraySet, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
