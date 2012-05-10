// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2FIXTURE_H
#define __META_BOX2D_B2FIXTURE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2Filter(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("categoryBits", _r), &D::ClassType::categoryBits);
    _d.CPGF_MD_TEMPLATE _field(replaceName("maskBits", _r), &D::ClassType::maskBits);
    _d.CPGF_MD_TEMPLATE _field(replaceName("groupIndex", _r), &D::ClassType::groupIndex);
}


template <typename D>
void buildMetaClass_B2Fixture(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetType", _r), &D::ClassType::GetType);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetShape", _r), (b2Shape * (D::ClassType::*) ())&D::ClassType::GetShape);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetShape", _r), (const b2Shape * (D::ClassType::*) () const)&D::ClassType::GetShape);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetSensor", _r), &D::ClassType::SetSensor);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsSensor", _r), &D::ClassType::IsSensor);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetFilterData", _r), &D::ClassType::SetFilterData);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetFilterData", _r), &D::ClassType::GetFilterData);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetBody", _r), (b2Body * (D::ClassType::*) ())&D::ClassType::GetBody);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetBody", _r), (const b2Body * (D::ClassType::*) () const)&D::ClassType::GetBody);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetNext", _r), (b2Fixture * (D::ClassType::*) ())&D::ClassType::GetNext);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetNext", _r), (const b2Fixture * (D::ClassType::*) () const)&D::ClassType::GetNext);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetUserData", _r), &D::ClassType::GetUserData);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetUserData", _r), &D::ClassType::SetUserData);
    _d.CPGF_MD_TEMPLATE _method(replaceName("TestPoint", _r), &D::ClassType::TestPoint);
    _d.CPGF_MD_TEMPLATE _method(replaceName("RayCast", _r), &D::ClassType::RayCast);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetMassData", _r), &D::ClassType::GetMassData);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetDensity", _r), &D::ClassType::SetDensity);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetDensity", _r), &D::ClassType::GetDensity);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetFriction", _r), &D::ClassType::GetFriction);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetFriction", _r), &D::ClassType::SetFriction);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetRestitution", _r), &D::ClassType::GetRestitution);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetRestitution", _r), &D::ClassType::SetRestitution);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAABB", _r), &D::ClassType::GetAABB);
}


template <typename D>
void buildMetaClass_B2FixtureDef(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field(replaceName("shape", _r), &D::ClassType::shape);
    _d.CPGF_MD_TEMPLATE _field(replaceName("userData", _r), &D::ClassType::userData);
    _d.CPGF_MD_TEMPLATE _field(replaceName("friction", _r), &D::ClassType::friction);
    _d.CPGF_MD_TEMPLATE _field(replaceName("restitution", _r), &D::ClassType::restitution);
    _d.CPGF_MD_TEMPLATE _field(replaceName("density", _r), &D::ClassType::density);
    _d.CPGF_MD_TEMPLATE _field(replaceName("isSensor", _r), &D::ClassType::isSensor);
    _d.CPGF_MD_TEMPLATE _field(replaceName("filter", _r), &D::ClassType::filter);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
