// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2FIXTURE_H
#define __META_BOX2D_B2FIXTURE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2Filter(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("categoryBits", &D::ClassType::categoryBits);
    _d.CPGF_MD_TEMPLATE _field("maskBits", &D::ClassType::maskBits);
    _d.CPGF_MD_TEMPLATE _field("groupIndex", &D::ClassType::groupIndex);
}


template <typename D>
void buildMetaClass_B2Fixture(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("GetType", &D::ClassType::GetType);
    _d.CPGF_MD_TEMPLATE _method("GetShape", (b2Shape * (D::ClassType::*) ())&D::ClassType::GetShape);
    _d.CPGF_MD_TEMPLATE _method("GetShape", (const b2Shape * (D::ClassType::*) () const)&D::ClassType::GetShape);
    _d.CPGF_MD_TEMPLATE _method("SetSensor", &D::ClassType::SetSensor);
    _d.CPGF_MD_TEMPLATE _method("IsSensor", &D::ClassType::IsSensor);
    _d.CPGF_MD_TEMPLATE _method("SetFilterData", &D::ClassType::SetFilterData);
    _d.CPGF_MD_TEMPLATE _method("GetFilterData", &D::ClassType::GetFilterData);
    _d.CPGF_MD_TEMPLATE _method("GetBody", (b2Body * (D::ClassType::*) ())&D::ClassType::GetBody);
    _d.CPGF_MD_TEMPLATE _method("GetBody", (const b2Body * (D::ClassType::*) () const)&D::ClassType::GetBody);
    _d.CPGF_MD_TEMPLATE _method("GetNext", (b2Fixture * (D::ClassType::*) ())&D::ClassType::GetNext);
    _d.CPGF_MD_TEMPLATE _method("GetNext", (const b2Fixture * (D::ClassType::*) () const)&D::ClassType::GetNext);
    _d.CPGF_MD_TEMPLATE _method("GetUserData", &D::ClassType::GetUserData);
    _d.CPGF_MD_TEMPLATE _method("SetUserData", &D::ClassType::SetUserData);
    _d.CPGF_MD_TEMPLATE _method("TestPoint", &D::ClassType::TestPoint);
    _d.CPGF_MD_TEMPLATE _method("RayCast", &D::ClassType::RayCast);
    _d.CPGF_MD_TEMPLATE _method("GetMassData", &D::ClassType::GetMassData);
    _d.CPGF_MD_TEMPLATE _method("SetDensity", &D::ClassType::SetDensity);
    _d.CPGF_MD_TEMPLATE _method("GetDensity", &D::ClassType::GetDensity);
    _d.CPGF_MD_TEMPLATE _method("GetFriction", &D::ClassType::GetFriction);
    _d.CPGF_MD_TEMPLATE _method("SetFriction", &D::ClassType::SetFriction);
    _d.CPGF_MD_TEMPLATE _method("GetRestitution", &D::ClassType::GetRestitution);
    _d.CPGF_MD_TEMPLATE _method("SetRestitution", &D::ClassType::SetRestitution);
    _d.CPGF_MD_TEMPLATE _method("GetAABB", &D::ClassType::GetAABB);
}


template <typename D>
void buildMetaClass_B2FixtureDef(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("shape", &D::ClassType::shape);
    _d.CPGF_MD_TEMPLATE _field("userData", &D::ClassType::userData);
    _d.CPGF_MD_TEMPLATE _field("friction", &D::ClassType::friction);
    _d.CPGF_MD_TEMPLATE _field("restitution", &D::ClassType::restitution);
    _d.CPGF_MD_TEMPLATE _field("density", &D::ClassType::density);
    _d.CPGF_MD_TEMPLATE _field("isSensor", &D::ClassType::isSensor);
    _d.CPGF_MD_TEMPLATE _field("filter", &D::ClassType::filter);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
