// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2PULLEYJOINT_H
#define __META_BOX2D_B2PULLEYJOINT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_Global_b2pulleyjoint(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("b2_minPulleyLength", &b2_minPulleyLength);
}


template <typename D>
void buildMetaClass_B2PulleyJoint(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("GetAnchorA", &D::ClassType::GetAnchorA);
    _d.CPGF_MD_TEMPLATE _method("GetAnchorB", &D::ClassType::GetAnchorB);
    _d.CPGF_MD_TEMPLATE _method("GetReactionForce", &D::ClassType::GetReactionForce);
    _d.CPGF_MD_TEMPLATE _method("GetReactionTorque", &D::ClassType::GetReactionTorque);
    _d.CPGF_MD_TEMPLATE _method("GetGroundAnchorA", &D::ClassType::GetGroundAnchorA);
    _d.CPGF_MD_TEMPLATE _method("GetGroundAnchorB", &D::ClassType::GetGroundAnchorB);
    _d.CPGF_MD_TEMPLATE _method("GetLength1", &D::ClassType::GetLength1);
    _d.CPGF_MD_TEMPLATE _method("GetLength2", &D::ClassType::GetLength2);
    _d.CPGF_MD_TEMPLATE _method("GetRatio", &D::ClassType::GetRatio);
}


template <typename D>
void buildMetaClass_B2PulleyJointDef(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("groundAnchorA", &D::ClassType::groundAnchorA);
    _d.CPGF_MD_TEMPLATE _field("groundAnchorB", &D::ClassType::groundAnchorB);
    _d.CPGF_MD_TEMPLATE _field("localAnchorA", &D::ClassType::localAnchorA);
    _d.CPGF_MD_TEMPLATE _field("localAnchorB", &D::ClassType::localAnchorB);
    _d.CPGF_MD_TEMPLATE _field("lengthA", &D::ClassType::lengthA);
    _d.CPGF_MD_TEMPLATE _field("maxLengthA", &D::ClassType::maxLengthA);
    _d.CPGF_MD_TEMPLATE _field("lengthB", &D::ClassType::lengthB);
    _d.CPGF_MD_TEMPLATE _field("maxLengthB", &D::ClassType::maxLengthB);
    _d.CPGF_MD_TEMPLATE _field("ratio", &D::ClassType::ratio);
    _d.CPGF_MD_TEMPLATE _method("Initialize", &D::ClassType::Initialize);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
