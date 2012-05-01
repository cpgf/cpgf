// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2BODY_H
#define __META_BOX2D_B2BODY_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


namespace meta_box2d { 


template <typename D, typename Policy>
void buildMetaClass_Global_b2body(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<b2BodyType>(replaceName("b2BodyType", _r))
        ._element(replaceName("b2_staticBody", _r), b2_staticBody)
        ._element(replaceName("b2_kinematicBody", _r), b2_kinematicBody)
        ._element(replaceName("b2_dynamicBody", _r), b2_dynamicBody)
    ;
}


template <typename D, typename Policy>
void buildMetaClass_B2Body(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("CreateFixture", _r), (b2Fixture * (D::ClassType::*) (const b2FixtureDef *))&D::ClassType::CreateFixture, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("CreateFixture", _r), (b2Fixture * (D::ClassType::*) (const b2Shape *, float32))&D::ClassType::CreateFixture, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DestroyFixture", _r), &D::ClassType::DestroyFixture, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetTransform", _r), &D::ClassType::SetTransform, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetTransform", _r), &D::ClassType::GetTransform, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetPosition", _r), &D::ClassType::GetPosition, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAngle", _r), &D::ClassType::GetAngle, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetWorldCenter", _r), &D::ClassType::GetWorldCenter, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetLocalCenter", _r), &D::ClassType::GetLocalCenter, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetLinearVelocity", _r), &D::ClassType::SetLinearVelocity, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetLinearVelocity", _r), &D::ClassType::GetLinearVelocity, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetAngularVelocity", _r), &D::ClassType::SetAngularVelocity, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAngularVelocity", _r), &D::ClassType::GetAngularVelocity, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ApplyForce", _r), &D::ClassType::ApplyForce, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ApplyTorque", _r), &D::ClassType::ApplyTorque, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ApplyLinearImpulse", _r), &D::ClassType::ApplyLinearImpulse, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ApplyAngularImpulse", _r), &D::ClassType::ApplyAngularImpulse, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetMass", _r), &D::ClassType::GetMass, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetInertia", _r), &D::ClassType::GetInertia, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetMassData", _r), &D::ClassType::GetMassData, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetMassData", _r), &D::ClassType::SetMassData, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ResetMassData", _r), &D::ClassType::ResetMassData, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetWorldPoint", _r), &D::ClassType::GetWorldPoint, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetWorldVector", _r), &D::ClassType::GetWorldVector, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetLocalPoint", _r), &D::ClassType::GetLocalPoint, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetLocalVector", _r), &D::ClassType::GetLocalVector, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetLinearVelocityFromWorldPoint", _r), &D::ClassType::GetLinearVelocityFromWorldPoint, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetLinearVelocityFromLocalPoint", _r), &D::ClassType::GetLinearVelocityFromLocalPoint, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetLinearDamping", _r), &D::ClassType::GetLinearDamping, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetLinearDamping", _r), &D::ClassType::SetLinearDamping, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAngularDamping", _r), &D::ClassType::GetAngularDamping, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetAngularDamping", _r), &D::ClassType::SetAngularDamping, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetType", _r), &D::ClassType::SetType, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetType", _r), &D::ClassType::GetType, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetBullet", _r), &D::ClassType::SetBullet, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsBullet", _r), &D::ClassType::IsBullet, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetSleepingAllowed", _r), &D::ClassType::SetSleepingAllowed, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsSleepingAllowed", _r), &D::ClassType::IsSleepingAllowed, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetAwake", _r), &D::ClassType::SetAwake, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsAwake", _r), &D::ClassType::IsAwake, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetActive", _r), &D::ClassType::SetActive, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsActive", _r), &D::ClassType::IsActive, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetFixedRotation", _r), &D::ClassType::SetFixedRotation, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsFixedRotation", _r), &D::ClassType::IsFixedRotation, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetFixtureList", _r), (b2Fixture * (D::ClassType::*) ())&D::ClassType::GetFixtureList, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetFixtureList", _r), (const b2Fixture * (D::ClassType::*) () const)&D::ClassType::GetFixtureList, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetJointList", _r), (b2JointEdge * (D::ClassType::*) ())&D::ClassType::GetJointList, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetJointList", _r), (const b2JointEdge * (D::ClassType::*) () const)&D::ClassType::GetJointList, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetContactList", _r), (b2ContactEdge * (D::ClassType::*) ())&D::ClassType::GetContactList, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetContactList", _r), (const b2ContactEdge * (D::ClassType::*) () const)&D::ClassType::GetContactList, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetNext", _r), (b2Body * (D::ClassType::*) ())&D::ClassType::GetNext, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetNext", _r), (const b2Body * (D::ClassType::*) () const)&D::ClassType::GetNext, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetUserData", _r), &D::ClassType::GetUserData, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetUserData", _r), &D::ClassType::SetUserData, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetWorld", _r), (b2World * (D::ClassType::*) ())&D::ClassType::GetWorld, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetWorld", _r), (const b2World * (D::ClassType::*) () const)&D::ClassType::GetWorld, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2BodyDef(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("type", _r), &D::ClassType::type, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("position", _r), &D::ClassType::position, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("angle", _r), &D::ClassType::angle, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("linearVelocity", _r), &D::ClassType::linearVelocity, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("angularVelocity", _r), &D::ClassType::angularVelocity, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("linearDamping", _r), &D::ClassType::linearDamping, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("angularDamping", _r), &D::ClassType::angularDamping, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("allowSleep", _r), &D::ClassType::allowSleep, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("awake", _r), &D::ClassType::awake, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("fixedRotation", _r), &D::ClassType::fixedRotation, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("bullet", _r), &D::ClassType::bullet, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("active", _r), &D::ClassType::active, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("userData", _r), &D::ClassType::userData, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("inertiaScale", _r), &D::ClassType::inertiaScale, _p);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
