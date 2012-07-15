// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2REVOLUTEJOINT_H
#define __META_BOX2D_B2REVOLUTEJOINT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2RevoluteJoint(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("GetAnchorA", &D::ClassType::GetAnchorA);
    _d.CPGF_MD_TEMPLATE _method("GetAnchorB", &D::ClassType::GetAnchorB);
    _d.CPGF_MD_TEMPLATE _method("GetReactionForce", &D::ClassType::GetReactionForce);
    _d.CPGF_MD_TEMPLATE _method("GetReactionTorque", &D::ClassType::GetReactionTorque);
    _d.CPGF_MD_TEMPLATE _method("GetJointAngle", &D::ClassType::GetJointAngle);
    _d.CPGF_MD_TEMPLATE _method("GetJointSpeed", &D::ClassType::GetJointSpeed);
    _d.CPGF_MD_TEMPLATE _method("IsLimitEnabled", &D::ClassType::IsLimitEnabled);
    _d.CPGF_MD_TEMPLATE _method("EnableLimit", &D::ClassType::EnableLimit);
    _d.CPGF_MD_TEMPLATE _method("GetLowerLimit", &D::ClassType::GetLowerLimit);
    _d.CPGF_MD_TEMPLATE _method("GetUpperLimit", &D::ClassType::GetUpperLimit);
    _d.CPGF_MD_TEMPLATE _method("SetLimits", &D::ClassType::SetLimits);
    _d.CPGF_MD_TEMPLATE _method("IsMotorEnabled", &D::ClassType::IsMotorEnabled);
    _d.CPGF_MD_TEMPLATE _method("EnableMotor", &D::ClassType::EnableMotor);
    _d.CPGF_MD_TEMPLATE _method("SetMotorSpeed", &D::ClassType::SetMotorSpeed);
    _d.CPGF_MD_TEMPLATE _method("GetMotorSpeed", &D::ClassType::GetMotorSpeed);
    _d.CPGF_MD_TEMPLATE _method("SetMaxMotorTorque", &D::ClassType::SetMaxMotorTorque);
    _d.CPGF_MD_TEMPLATE _method("GetMotorTorque", &D::ClassType::GetMotorTorque);
}


template <typename D>
void buildMetaClass_B2RevoluteJointDef(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("localAnchorA", &D::ClassType::localAnchorA);
    _d.CPGF_MD_TEMPLATE _field("localAnchorB", &D::ClassType::localAnchorB);
    _d.CPGF_MD_TEMPLATE _field("referenceAngle", &D::ClassType::referenceAngle);
    _d.CPGF_MD_TEMPLATE _field("enableLimit", &D::ClassType::enableLimit);
    _d.CPGF_MD_TEMPLATE _field("lowerAngle", &D::ClassType::lowerAngle);
    _d.CPGF_MD_TEMPLATE _field("upperAngle", &D::ClassType::upperAngle);
    _d.CPGF_MD_TEMPLATE _field("enableMotor", &D::ClassType::enableMotor);
    _d.CPGF_MD_TEMPLATE _field("motorSpeed", &D::ClassType::motorSpeed);
    _d.CPGF_MD_TEMPLATE _field("maxMotorTorque", &D::ClassType::maxMotorTorque);
    _d.CPGF_MD_TEMPLATE _method("Initialize", &D::ClassType::Initialize);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
