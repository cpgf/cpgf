// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_CDYNAMICMESHBUFFER_H
#define __META_IRRLICHT_CDYNAMICMESHBUFFER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_CDynamicMeshBuffer(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (video::E_VERTEX_TYPE, video::E_INDEX_TYPE)>();
    _d.CPGF_MD_TEMPLATE _field("Material", &D::ClassType::Material);
    _d.CPGF_MD_TEMPLATE _field("BoundingBox", &D::ClassType::BoundingBox);
    _d.CPGF_MD_TEMPLATE _method("getVertexBuffer", &D::ClassType::getVertexBuffer);
    _d.CPGF_MD_TEMPLATE _method("getIndexBuffer", &D::ClassType::getIndexBuffer);
    _d.CPGF_MD_TEMPLATE _method("setVertexBuffer", &D::ClassType::setVertexBuffer);
    _d.CPGF_MD_TEMPLATE _method("setIndexBuffer", &D::ClassType::setIndexBuffer);
    _d.CPGF_MD_TEMPLATE _method("getMaterial", (const video::SMaterial & (D::ClassType::*) () const)&D::ClassType::getMaterial);
    _d.CPGF_MD_TEMPLATE _method("getMaterial", (video::SMaterial & (D::ClassType::*) ())&D::ClassType::getMaterial);
    _d.CPGF_MD_TEMPLATE _method("getBoundingBox", &D::ClassType::getBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("setBoundingBox", &D::ClassType::setBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("recalculateBoundingBox", &D::ClassType::recalculateBoundingBox);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
