// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2TIMESTEP_H
#define __META_BOX2D_B2TIMESTEP_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2TimeStep(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("dt", &D::ClassType::dt);
    _d.CPGF_MD_TEMPLATE _field("inv_dt", &D::ClassType::inv_dt);
    _d.CPGF_MD_TEMPLATE _field("dtRatio", &D::ClassType::dtRatio);
    _d.CPGF_MD_TEMPLATE _field("velocityIterations", &D::ClassType::velocityIterations);
    _d.CPGF_MD_TEMPLATE _field("positionIterations", &D::ClassType::positionIterations);
    _d.CPGF_MD_TEMPLATE _field("warmStarting", &D::ClassType::warmStarting);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
