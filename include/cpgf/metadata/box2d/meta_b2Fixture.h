// Auto generated file, don't modify.

#ifndef __META_B2FIXTURE_H
#define __META_B2FIXTURE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


template <typename D, typename Policy>
void buildMetaClass_B2Filter(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("categoryBits", _r), &D::ClassType::categoryBits, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("maskBits", _r), &D::ClassType::maskBits, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("groupIndex", _r), &D::ClassType::groupIndex, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2Fixture(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetType", _r), &D::ClassType::GetType, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetShape", _r), (b2Shape * (D::ClassType::*) ())&D::ClassType::GetShape, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetShape", _r), (const b2Shape * (D::ClassType::*) () const)&D::ClassType::GetShape, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetSensor", _r), &D::ClassType::SetSensor, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsSensor", _r), &D::ClassType::IsSensor, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetFilterData", _r), &D::ClassType::SetFilterData, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetFilterData", _r), &D::ClassType::GetFilterData, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetBody", _r), (b2Body * (D::ClassType::*) ())&D::ClassType::GetBody, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetBody", _r), (const b2Body * (D::ClassType::*) () const)&D::ClassType::GetBody, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetNext", _r), (b2Fixture * (D::ClassType::*) ())&D::ClassType::GetNext, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetNext", _r), (const b2Fixture * (D::ClassType::*) () const)&D::ClassType::GetNext, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetUserData", _r), &D::ClassType::GetUserData, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetUserData", _r), &D::ClassType::SetUserData, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("TestPoint", _r), &D::ClassType::TestPoint, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("RayCast", _r), &D::ClassType::RayCast, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetMassData", _r), &D::ClassType::GetMassData, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetDensity", _r), &D::ClassType::SetDensity, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetDensity", _r), &D::ClassType::GetDensity, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetFriction", _r), &D::ClassType::GetFriction, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetFriction", _r), &D::ClassType::SetFriction, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetRestitution", _r), &D::ClassType::GetRestitution, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetRestitution", _r), &D::ClassType::SetRestitution, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAABB", _r), &D::ClassType::GetAABB, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2FixtureDef(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("shape", _r), &D::ClassType::shape, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("userData", _r), &D::ClassType::userData, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("friction", _r), &D::ClassType::friction, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("restitution", _r), &D::ClassType::restitution, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("density", _r), &D::ClassType::density, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("isSensor", _r), &D::ClassType::isSensor, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("filter", _r), &D::ClassType::filter, _p);
}




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif