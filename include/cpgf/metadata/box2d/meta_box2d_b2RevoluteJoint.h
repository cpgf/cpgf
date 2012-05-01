// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2REVOLUTEJOINT_H
#define __META_BOX2D_B2REVOLUTEJOINT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


namespace meta_box2d { 


template <typename D, typename Policy>
void buildMetaClass_B2RevoluteJoint(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAnchorA", _r), &D::ClassType::GetAnchorA, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAnchorB", _r), &D::ClassType::GetAnchorB, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetReactionForce", _r), &D::ClassType::GetReactionForce, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetReactionTorque", _r), &D::ClassType::GetReactionTorque, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetJointAngle", _r), &D::ClassType::GetJointAngle, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetJointSpeed", _r), &D::ClassType::GetJointSpeed, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsLimitEnabled", _r), &D::ClassType::IsLimitEnabled, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("EnableLimit", _r), &D::ClassType::EnableLimit, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetLowerLimit", _r), &D::ClassType::GetLowerLimit, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetUpperLimit", _r), &D::ClassType::GetUpperLimit, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetLimits", _r), &D::ClassType::SetLimits, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsMotorEnabled", _r), &D::ClassType::IsMotorEnabled, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("EnableMotor", _r), &D::ClassType::EnableMotor, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetMotorSpeed", _r), &D::ClassType::SetMotorSpeed, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetMotorSpeed", _r), &D::ClassType::GetMotorSpeed, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetMaxMotorTorque", _r), &D::ClassType::SetMaxMotorTorque, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetMotorTorque", _r), &D::ClassType::GetMotorTorque, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2RevoluteJointDef(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("localAnchorA", _r), &D::ClassType::localAnchorA, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("localAnchorB", _r), &D::ClassType::localAnchorB, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("referenceAngle", _r), &D::ClassType::referenceAngle, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("enableLimit", _r), &D::ClassType::enableLimit, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("lowerAngle", _r), &D::ClassType::lowerAngle, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("upperAngle", _r), &D::ClassType::upperAngle, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("enableMotor", _r), &D::ClassType::enableMotor, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("motorSpeed", _r), &D::ClassType::motorSpeed, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("maxMotorTorque", _r), &D::ClassType::maxMotorTorque, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Initialize", _r), &D::ClassType::Initialize, _p);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
