// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2FRICTIONJOINT_H
#define __META_BOX2D_B2FRICTIONJOINT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2FrictionJoint(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("GetAnchorA", &D::ClassType::GetAnchorA);
    _d.CPGF_MD_TEMPLATE _method("GetAnchorB", &D::ClassType::GetAnchorB);
    _d.CPGF_MD_TEMPLATE _method("GetReactionForce", &D::ClassType::GetReactionForce);
    _d.CPGF_MD_TEMPLATE _method("GetReactionTorque", &D::ClassType::GetReactionTorque);
    _d.CPGF_MD_TEMPLATE _method("SetMaxForce", &D::ClassType::SetMaxForce);
    _d.CPGF_MD_TEMPLATE _method("GetMaxForce", &D::ClassType::GetMaxForce);
    _d.CPGF_MD_TEMPLATE _method("SetMaxTorque", &D::ClassType::SetMaxTorque);
    _d.CPGF_MD_TEMPLATE _method("GetMaxTorque", &D::ClassType::GetMaxTorque);
}


template <typename D>
void buildMetaClass_B2FrictionJointDef(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("localAnchorA", &D::ClassType::localAnchorA);
    _d.CPGF_MD_TEMPLATE _field("localAnchorB", &D::ClassType::localAnchorB);
    _d.CPGF_MD_TEMPLATE _field("maxForce", &D::ClassType::maxForce);
    _d.CPGF_MD_TEMPLATE _field("maxTorque", &D::ClassType::maxTorque);
    _d.CPGF_MD_TEMPLATE _method("Initialize", &D::ClassType::Initialize);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
