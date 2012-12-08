// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ITRIANGLESELECTOR_H
#define __META_IRRLICHT_ITRIANGLESELECTOR_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_ITriangleSelector(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getTriangleCount", &D::ClassType::getTriangleCount);
    _d.CPGF_MD_TEMPLATE _method("getTriangles", (void (D::ClassType::*) (core::triangle3df *, s32, s32 &, const core::matrix4 *) const)&D::ClassType::getTriangles)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getTriangles", (void (D::ClassType::*) (core::triangle3df *, s32, s32 &, const core::aabbox3d< f32 > &, const core::matrix4 *) const)&D::ClassType::getTriangles)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getTriangles", (void (D::ClassType::*) (core::triangle3df *, s32, s32 &, const core::line3d< f32 > &, const core::matrix4 *) const)&D::ClassType::getTriangles)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getSceneNodeForTriangle", &D::ClassType::getSceneNodeForTriangle);
    _d.CPGF_MD_TEMPLATE _method("getSelectorCount", &D::ClassType::getSelectorCount);
    _d.CPGF_MD_TEMPLATE _method("getSelector", (ITriangleSelector * (D::ClassType::*) (u32))&D::ClassType::getSelector);
    _d.CPGF_MD_TEMPLATE _method("getSelector", (const ITriangleSelector * (D::ClassType::*) (u32) const)&D::ClassType::getSelector);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
