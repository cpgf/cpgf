// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2DISTANCE_H
#define __META_BOX2D_B2DISTANCE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_Global_b2distance(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("b2Distance", (void (*) (b2DistanceOutput *, b2SimplexCache *, const b2DistanceInput *))&b2Distance);
}


template <typename D>
void buildMetaClass_B2DistanceInput(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("proxyA", &D::ClassType::proxyA);
    _d.CPGF_MD_TEMPLATE _field("proxyB", &D::ClassType::proxyB);
    _d.CPGF_MD_TEMPLATE _field("transformA", &D::ClassType::transformA);
    _d.CPGF_MD_TEMPLATE _field("transformB", &D::ClassType::transformB);
    _d.CPGF_MD_TEMPLATE _field("useRadii", &D::ClassType::useRadii);
}


template <typename D>
void buildMetaClass_B2DistanceOutput(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("pointA", &D::ClassType::pointA);
    _d.CPGF_MD_TEMPLATE _field("pointB", &D::ClassType::pointB);
    _d.CPGF_MD_TEMPLATE _field("distance", &D::ClassType::distance);
    _d.CPGF_MD_TEMPLATE _field("iterations", &D::ClassType::iterations);
}


template <typename D>
void buildMetaClass_B2DistanceProxy(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("m_vertices", &D::ClassType::m_vertices);
    _d.CPGF_MD_TEMPLATE _field("m_count", &D::ClassType::m_count);
    _d.CPGF_MD_TEMPLATE _field("m_radius", &D::ClassType::m_radius);
    _d.CPGF_MD_TEMPLATE _method("Set", &D::ClassType::Set);
    _d.CPGF_MD_TEMPLATE _method("GetSupport", &D::ClassType::GetSupport);
    _d.CPGF_MD_TEMPLATE _method("GetSupportVertex", &D::ClassType::GetSupportVertex);
    _d.CPGF_MD_TEMPLATE _method("GetVertexCount", &D::ClassType::GetVertexCount);
    _d.CPGF_MD_TEMPLATE _method("GetVertex", &D::ClassType::GetVertex);
}


template <typename D>
void buildMetaClass_B2SimplexCache(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("metric", &D::ClassType::metric);
    _d.CPGF_MD_TEMPLATE _field("count", &D::ClassType::count);
    _d.CPGF_MD_TEMPLATE _field("indexA", &D::ClassType::indexA);
    _d.CPGF_MD_TEMPLATE _field("indexB", &D::ClassType::indexB);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
