// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2SHAPE_H
#define __META_BOX2D_B2SHAPE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2MassData(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("mass", _r), &D::ClassType::mass);
    _d.CPGF_MD_TEMPLATE _field(replaceName("center", _r), &D::ClassType::center);
    _d.CPGF_MD_TEMPLATE _field(replaceName("I", _r), &D::ClassType::I);
}


template <typename D>
void buildMetaClass_B2Shape(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_type", _r), &D::ClassType::m_type);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_radius", _r), &D::ClassType::m_radius);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Clone", _r), &D::ClassType::Clone);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetType", _r), &D::ClassType::GetType);
    _d.CPGF_MD_TEMPLATE _method(replaceName("TestPoint", _r), &D::ClassType::TestPoint);
    _d.CPGF_MD_TEMPLATE _method(replaceName("RayCast", _r), &D::ClassType::RayCast);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ComputeAABB", _r), &D::ClassType::ComputeAABB);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ComputeMass", _r), &D::ClassType::ComputeMass);
    _d.CPGF_MD_TEMPLATE _enum<typename D::ClassType::Type>(replaceName("Type", _r))
        ._element(replaceName("e_unknown", _r), D::ClassType::e_unknown)
        ._element(replaceName("e_circle", _r), D::ClassType::e_circle)
        ._element(replaceName("e_polygon", _r), D::ClassType::e_polygon)
        ._element(replaceName("e_typeCount", _r), D::ClassType::e_typeCount)
    ;
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
