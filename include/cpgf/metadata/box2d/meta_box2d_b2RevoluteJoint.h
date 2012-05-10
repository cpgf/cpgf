// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2REVOLUTEJOINT_H
#define __META_BOX2D_B2REVOLUTEJOINT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2RevoluteJoint(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAnchorA", _r), &D::ClassType::GetAnchorA);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAnchorB", _r), &D::ClassType::GetAnchorB);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetReactionForce", _r), &D::ClassType::GetReactionForce);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetReactionTorque", _r), &D::ClassType::GetReactionTorque);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetJointAngle", _r), &D::ClassType::GetJointAngle);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetJointSpeed", _r), &D::ClassType::GetJointSpeed);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsLimitEnabled", _r), &D::ClassType::IsLimitEnabled);
    _d.CPGF_MD_TEMPLATE _method(replaceName("EnableLimit", _r), &D::ClassType::EnableLimit);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetLowerLimit", _r), &D::ClassType::GetLowerLimit);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetUpperLimit", _r), &D::ClassType::GetUpperLimit);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetLimits", _r), &D::ClassType::SetLimits);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsMotorEnabled", _r), &D::ClassType::IsMotorEnabled);
    _d.CPGF_MD_TEMPLATE _method(replaceName("EnableMotor", _r), &D::ClassType::EnableMotor);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetMotorSpeed", _r), &D::ClassType::SetMotorSpeed);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetMotorSpeed", _r), &D::ClassType::GetMotorSpeed);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetMaxMotorTorque", _r), &D::ClassType::SetMaxMotorTorque);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetMotorTorque", _r), &D::ClassType::GetMotorTorque);
}


template <typename D>
void buildMetaClass_B2RevoluteJointDef(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field(replaceName("localAnchorA", _r), &D::ClassType::localAnchorA);
    _d.CPGF_MD_TEMPLATE _field(replaceName("localAnchorB", _r), &D::ClassType::localAnchorB);
    _d.CPGF_MD_TEMPLATE _field(replaceName("referenceAngle", _r), &D::ClassType::referenceAngle);
    _d.CPGF_MD_TEMPLATE _field(replaceName("enableLimit", _r), &D::ClassType::enableLimit);
    _d.CPGF_MD_TEMPLATE _field(replaceName("lowerAngle", _r), &D::ClassType::lowerAngle);
    _d.CPGF_MD_TEMPLATE _field(replaceName("upperAngle", _r), &D::ClassType::upperAngle);
    _d.CPGF_MD_TEMPLATE _field(replaceName("enableMotor", _r), &D::ClassType::enableMotor);
    _d.CPGF_MD_TEMPLATE _field(replaceName("motorSpeed", _r), &D::ClassType::motorSpeed);
    _d.CPGF_MD_TEMPLATE _field(replaceName("maxMotorTorque", _r), &D::ClassType::maxMotorTorque);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Initialize", _r), &D::ClassType::Initialize);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
