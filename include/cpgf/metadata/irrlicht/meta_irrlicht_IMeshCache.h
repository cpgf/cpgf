// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IMESHCACHE_H
#define __META_IRRLICHT_IMESHCACHE_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IMeshCache(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("addMesh", &D::ClassType::addMesh, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("removeMesh", &D::ClassType::removeMesh);
    _d.CPGF_MD_TEMPLATE _method("getMeshCount", &D::ClassType::getMeshCount);
    _d.CPGF_MD_TEMPLATE _method("getMeshIndex", &D::ClassType::getMeshIndex);
    _d.CPGF_MD_TEMPLATE _method("getMeshByIndex", &D::ClassType::getMeshByIndex);
    _d.CPGF_MD_TEMPLATE _method("getMeshByFilename", &D::ClassType::getMeshByFilename, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getMeshFilename", (_IRR_DEPRECATED_ const io::path & (D::ClassType::*) (u32) const)&D::ClassType::getMeshFilename);
    _d.CPGF_MD_TEMPLATE _method("getMeshFilename", (_IRR_DEPRECATED_ const io::path & (D::ClassType::*) (const IMesh *const) const)&D::ClassType::getMeshFilename);
    _d.CPGF_MD_TEMPLATE _method("setMeshFilename", (_IRR_DEPRECATED_ bool (D::ClassType::*) (u32, const io::path &))&D::ClassType::setMeshFilename, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("setMeshFilename", (_IRR_DEPRECATED_ bool (D::ClassType::*) (const IMesh *const, const io::path &))&D::ClassType::setMeshFilename, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("getMeshByName", &D::ClassType::getMeshByName, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getMeshName", (const io::SNamedPath & (D::ClassType::*) (u32) const)&D::ClassType::getMeshName);
    _d.CPGF_MD_TEMPLATE _method("getMeshName", (const io::SNamedPath & (D::ClassType::*) (const IMesh *const) const)&D::ClassType::getMeshName);
    _d.CPGF_MD_TEMPLATE _method("renameMesh", (bool (D::ClassType::*) (u32, const io::path &))&D::ClassType::renameMesh, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("renameMesh", (bool (D::ClassType::*) (const IMesh *const, const io::path &))&D::ClassType::renameMesh, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("isMeshLoaded", &D::ClassType::isMeshLoaded, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("clear", &D::ClassType::clear);
    _d.CPGF_MD_TEMPLATE _method("clearUnusedMeshes", &D::ClassType::clearUnusedMeshes);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
