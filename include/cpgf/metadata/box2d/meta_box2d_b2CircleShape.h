// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2CIRCLESHAPE_H
#define __META_BOX2D_B2CIRCLESHAPE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2CircleShape(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("m_p", &D::ClassType::m_p);
    _d.CPGF_MD_TEMPLATE _method("Clone", &D::ClassType::Clone);
    _d.CPGF_MD_TEMPLATE _method("TestPoint", &D::ClassType::TestPoint);
    _d.CPGF_MD_TEMPLATE _method("RayCast", &D::ClassType::RayCast);
    _d.CPGF_MD_TEMPLATE _method("ComputeAABB", &D::ClassType::ComputeAABB);
    _d.CPGF_MD_TEMPLATE _method("ComputeMass", &D::ClassType::ComputeMass);
    _d.CPGF_MD_TEMPLATE _method("GetSupport", &D::ClassType::GetSupport);
    _d.CPGF_MD_TEMPLATE _method("GetSupportVertex", &D::ClassType::GetSupportVertex);
    _d.CPGF_MD_TEMPLATE _method("GetVertexCount", &D::ClassType::GetVertexCount);
    _d.CPGF_MD_TEMPLATE _method("GetVertex", &D::ClassType::GetVertex);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
