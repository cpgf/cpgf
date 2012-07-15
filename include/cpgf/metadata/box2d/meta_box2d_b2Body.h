// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2BODY_H
#define __META_BOX2D_B2BODY_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_Global_b2body(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<b2BodyType>("b2BodyType")
        ._element("b2_staticBody", b2_staticBody)
        ._element("b2_kinematicBody", b2_kinematicBody)
        ._element("b2_dynamicBody", b2_dynamicBody)
    ;
}


template <typename D>
void buildMetaClass_B2Body(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("CreateFixture", (b2Fixture * (D::ClassType::*) (const b2FixtureDef *))&D::ClassType::CreateFixture);
    _d.CPGF_MD_TEMPLATE _method("CreateFixture", (b2Fixture * (D::ClassType::*) (const b2Shape *, float32))&D::ClassType::CreateFixture);
    _d.CPGF_MD_TEMPLATE _method("DestroyFixture", &D::ClassType::DestroyFixture);
    _d.CPGF_MD_TEMPLATE _method("SetTransform", &D::ClassType::SetTransform);
    _d.CPGF_MD_TEMPLATE _method("GetTransform", &D::ClassType::GetTransform);
    _d.CPGF_MD_TEMPLATE _method("GetPosition", &D::ClassType::GetPosition);
    _d.CPGF_MD_TEMPLATE _method("GetAngle", &D::ClassType::GetAngle);
    _d.CPGF_MD_TEMPLATE _method("GetWorldCenter", &D::ClassType::GetWorldCenter);
    _d.CPGF_MD_TEMPLATE _method("GetLocalCenter", &D::ClassType::GetLocalCenter);
    _d.CPGF_MD_TEMPLATE _method("SetLinearVelocity", &D::ClassType::SetLinearVelocity);
    _d.CPGF_MD_TEMPLATE _method("GetLinearVelocity", &D::ClassType::GetLinearVelocity);
    _d.CPGF_MD_TEMPLATE _method("SetAngularVelocity", &D::ClassType::SetAngularVelocity);
    _d.CPGF_MD_TEMPLATE _method("GetAngularVelocity", &D::ClassType::GetAngularVelocity);
    _d.CPGF_MD_TEMPLATE _method("ApplyForce", &D::ClassType::ApplyForce);
    _d.CPGF_MD_TEMPLATE _method("ApplyTorque", &D::ClassType::ApplyTorque);
    _d.CPGF_MD_TEMPLATE _method("ApplyLinearImpulse", &D::ClassType::ApplyLinearImpulse);
    _d.CPGF_MD_TEMPLATE _method("ApplyAngularImpulse", &D::ClassType::ApplyAngularImpulse);
    _d.CPGF_MD_TEMPLATE _method("GetMass", &D::ClassType::GetMass);
    _d.CPGF_MD_TEMPLATE _method("GetInertia", &D::ClassType::GetInertia);
    _d.CPGF_MD_TEMPLATE _method("GetMassData", &D::ClassType::GetMassData);
    _d.CPGF_MD_TEMPLATE _method("SetMassData", &D::ClassType::SetMassData);
    _d.CPGF_MD_TEMPLATE _method("ResetMassData", &D::ClassType::ResetMassData);
    _d.CPGF_MD_TEMPLATE _method("GetWorldPoint", &D::ClassType::GetWorldPoint);
    _d.CPGF_MD_TEMPLATE _method("GetWorldVector", &D::ClassType::GetWorldVector);
    _d.CPGF_MD_TEMPLATE _method("GetLocalPoint", &D::ClassType::GetLocalPoint);
    _d.CPGF_MD_TEMPLATE _method("GetLocalVector", &D::ClassType::GetLocalVector);
    _d.CPGF_MD_TEMPLATE _method("GetLinearVelocityFromWorldPoint", &D::ClassType::GetLinearVelocityFromWorldPoint);
    _d.CPGF_MD_TEMPLATE _method("GetLinearVelocityFromLocalPoint", &D::ClassType::GetLinearVelocityFromLocalPoint);
    _d.CPGF_MD_TEMPLATE _method("GetLinearDamping", &D::ClassType::GetLinearDamping);
    _d.CPGF_MD_TEMPLATE _method("SetLinearDamping", &D::ClassType::SetLinearDamping);
    _d.CPGF_MD_TEMPLATE _method("GetAngularDamping", &D::ClassType::GetAngularDamping);
    _d.CPGF_MD_TEMPLATE _method("SetAngularDamping", &D::ClassType::SetAngularDamping);
    _d.CPGF_MD_TEMPLATE _method("SetType", &D::ClassType::SetType);
    _d.CPGF_MD_TEMPLATE _method("GetType", &D::ClassType::GetType);
    _d.CPGF_MD_TEMPLATE _method("SetBullet", &D::ClassType::SetBullet);
    _d.CPGF_MD_TEMPLATE _method("IsBullet", &D::ClassType::IsBullet);
    _d.CPGF_MD_TEMPLATE _method("SetSleepingAllowed", &D::ClassType::SetSleepingAllowed);
    _d.CPGF_MD_TEMPLATE _method("IsSleepingAllowed", &D::ClassType::IsSleepingAllowed);
    _d.CPGF_MD_TEMPLATE _method("SetAwake", &D::ClassType::SetAwake);
    _d.CPGF_MD_TEMPLATE _method("IsAwake", &D::ClassType::IsAwake);
    _d.CPGF_MD_TEMPLATE _method("SetActive", &D::ClassType::SetActive);
    _d.CPGF_MD_TEMPLATE _method("IsActive", &D::ClassType::IsActive);
    _d.CPGF_MD_TEMPLATE _method("SetFixedRotation", &D::ClassType::SetFixedRotation);
    _d.CPGF_MD_TEMPLATE _method("IsFixedRotation", &D::ClassType::IsFixedRotation);
    _d.CPGF_MD_TEMPLATE _method("GetFixtureList", (b2Fixture * (D::ClassType::*) ())&D::ClassType::GetFixtureList);
    _d.CPGF_MD_TEMPLATE _method("GetFixtureList", (const b2Fixture * (D::ClassType::*) () const)&D::ClassType::GetFixtureList);
    _d.CPGF_MD_TEMPLATE _method("GetJointList", (b2JointEdge * (D::ClassType::*) ())&D::ClassType::GetJointList);
    _d.CPGF_MD_TEMPLATE _method("GetJointList", (const b2JointEdge * (D::ClassType::*) () const)&D::ClassType::GetJointList);
    _d.CPGF_MD_TEMPLATE _method("GetContactList", (b2ContactEdge * (D::ClassType::*) ())&D::ClassType::GetContactList);
    _d.CPGF_MD_TEMPLATE _method("GetContactList", (const b2ContactEdge * (D::ClassType::*) () const)&D::ClassType::GetContactList);
    _d.CPGF_MD_TEMPLATE _method("GetNext", (b2Body * (D::ClassType::*) ())&D::ClassType::GetNext);
    _d.CPGF_MD_TEMPLATE _method("GetNext", (const b2Body * (D::ClassType::*) () const)&D::ClassType::GetNext);
    _d.CPGF_MD_TEMPLATE _method("GetUserData", &D::ClassType::GetUserData);
    _d.CPGF_MD_TEMPLATE _method("SetUserData", &D::ClassType::SetUserData);
    _d.CPGF_MD_TEMPLATE _method("GetWorld", (b2World * (D::ClassType::*) ())&D::ClassType::GetWorld);
    _d.CPGF_MD_TEMPLATE _method("GetWorld", (const b2World * (D::ClassType::*) () const)&D::ClassType::GetWorld);
}


template <typename D>
void buildMetaClass_B2BodyDef(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("type", &D::ClassType::type);
    _d.CPGF_MD_TEMPLATE _field("position", &D::ClassType::position);
    _d.CPGF_MD_TEMPLATE _field("angle", &D::ClassType::angle);
    _d.CPGF_MD_TEMPLATE _field("linearVelocity", &D::ClassType::linearVelocity);
    _d.CPGF_MD_TEMPLATE _field("angularVelocity", &D::ClassType::angularVelocity);
    _d.CPGF_MD_TEMPLATE _field("linearDamping", &D::ClassType::linearDamping);
    _d.CPGF_MD_TEMPLATE _field("angularDamping", &D::ClassType::angularDamping);
    _d.CPGF_MD_TEMPLATE _field("allowSleep", &D::ClassType::allowSleep);
    _d.CPGF_MD_TEMPLATE _field("awake", &D::ClassType::awake);
    _d.CPGF_MD_TEMPLATE _field("fixedRotation", &D::ClassType::fixedRotation);
    _d.CPGF_MD_TEMPLATE _field("bullet", &D::ClassType::bullet);
    _d.CPGF_MD_TEMPLATE _field("active", &D::ClassType::active);
    _d.CPGF_MD_TEMPLATE _field("userData", &D::ClassType::userData);
    _d.CPGF_MD_TEMPLATE _field("inertiaScale", &D::ClassType::inertiaScale);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
