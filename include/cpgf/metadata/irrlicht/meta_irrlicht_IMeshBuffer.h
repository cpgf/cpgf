// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IMESHBUFFER_H
#define __META_IRRLICHT_IMESHBUFFER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IMeshBuffer(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getMaterial", (video::SMaterial & (D::ClassType::*) ())&D::ClassType::getMaterial);
    _d.CPGF_MD_TEMPLATE _method("getMaterial", (const video::SMaterial & (D::ClassType::*) () const)&D::ClassType::getMaterial);
    _d.CPGF_MD_TEMPLATE _method("getVertexType", &D::ClassType::getVertexType);
    _d.CPGF_MD_TEMPLATE _method("getVertices", (const void * (D::ClassType::*) () const)&D::ClassType::getVertices);
    _d.CPGF_MD_TEMPLATE _method("getVertices", (void * (D::ClassType::*) ())&D::ClassType::getVertices);
    _d.CPGF_MD_TEMPLATE _method("getVertexCount", &D::ClassType::getVertexCount);
    _d.CPGF_MD_TEMPLATE _method("getIndexType", &D::ClassType::getIndexType);
    _d.CPGF_MD_TEMPLATE _method("getIndices", (const u16 * (D::ClassType::*) () const)&D::ClassType::getIndices);
    _d.CPGF_MD_TEMPLATE _method("getIndices", (u16 * (D::ClassType::*) ())&D::ClassType::getIndices);
    _d.CPGF_MD_TEMPLATE _method("getIndexCount", &D::ClassType::getIndexCount);
    _d.CPGF_MD_TEMPLATE _method("getBoundingBox", &D::ClassType::getBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("setBoundingBox", &D::ClassType::setBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("recalculateBoundingBox", &D::ClassType::recalculateBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("getPosition", (const core::vector3df & (D::ClassType::*) (u32) const)&D::ClassType::getPosition);
    _d.CPGF_MD_TEMPLATE _method("getPosition", (core::vector3df & (D::ClassType::*) (u32))&D::ClassType::getPosition);
    _d.CPGF_MD_TEMPLATE _method("getNormal", (const core::vector3df & (D::ClassType::*) (u32) const)&D::ClassType::getNormal);
    _d.CPGF_MD_TEMPLATE _method("getNormal", (core::vector3df & (D::ClassType::*) (u32))&D::ClassType::getNormal);
    _d.CPGF_MD_TEMPLATE _method("getTCoords", (const core::vector2df & (D::ClassType::*) (u32) const)&D::ClassType::getTCoords);
    _d.CPGF_MD_TEMPLATE _method("getTCoords", (core::vector2df & (D::ClassType::*) (u32))&D::ClassType::getTCoords);
    _d.CPGF_MD_TEMPLATE _method("append", (void (D::ClassType::*) (const void *const, u32, const u16 *const, u32))&D::ClassType::append);
    _d.CPGF_MD_TEMPLATE _method("append", (void (D::ClassType::*) (const IMeshBuffer *const))&D::ClassType::append);
    _d.CPGF_MD_TEMPLATE _method("getHardwareMappingHint_Vertex", &D::ClassType::getHardwareMappingHint_Vertex);
    _d.CPGF_MD_TEMPLATE _method("getHardwareMappingHint_Index", &D::ClassType::getHardwareMappingHint_Index);
    _d.CPGF_MD_TEMPLATE _method("setHardwareMappingHint", &D::ClassType::setHardwareMappingHint)
        ._default(copyVariantFromCopyable(EBT_VERTEX_AND_INDEX))
    ;
    _d.CPGF_MD_TEMPLATE _method("setDirty", &D::ClassType::setDirty)
        ._default(copyVariantFromCopyable(EBT_VERTEX_AND_INDEX))
    ;
    _d.CPGF_MD_TEMPLATE _method("getChangedID_Vertex", &D::ClassType::getChangedID_Vertex);
    _d.CPGF_MD_TEMPLATE _method("getChangedID_Index", &D::ClassType::getChangedID_Index);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
