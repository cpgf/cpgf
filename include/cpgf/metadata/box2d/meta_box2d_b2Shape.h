// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2SHAPE_H
#define __META_BOX2D_B2SHAPE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2MassData(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("mass", &D::ClassType::mass);
    _d.CPGF_MD_TEMPLATE _field("center", &D::ClassType::center);
    _d.CPGF_MD_TEMPLATE _field("I", &D::ClassType::I);
}


template <typename D>
void buildMetaClass_B2Shape(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("m_type", &D::ClassType::m_type);
    _d.CPGF_MD_TEMPLATE _field("m_radius", &D::ClassType::m_radius);
    _d.CPGF_MD_TEMPLATE _method("Clone", &D::ClassType::Clone);
    _d.CPGF_MD_TEMPLATE _method("GetType", &D::ClassType::GetType);
    _d.CPGF_MD_TEMPLATE _method("TestPoint", &D::ClassType::TestPoint);
    _d.CPGF_MD_TEMPLATE _method("RayCast", &D::ClassType::RayCast);
    _d.CPGF_MD_TEMPLATE _method("ComputeAABB", &D::ClassType::ComputeAABB);
    _d.CPGF_MD_TEMPLATE _method("ComputeMass", &D::ClassType::ComputeMass);
    _d.CPGF_MD_TEMPLATE _enum<typename D::ClassType::Type>("Type")
        ._element("e_unknown", D::ClassType::e_unknown)
        ._element("e_circle", D::ClassType::e_circle)
        ._element("e_polygon", D::ClassType::e_polygon)
        ._element("e_typeCount", D::ClassType::e_typeCount)
    ;
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
