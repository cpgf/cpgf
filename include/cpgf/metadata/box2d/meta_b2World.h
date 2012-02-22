// Auto generated file, don't modify.

#ifndef __META_B2WORLD_H
#define __META_B2WORLD_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


namespace meta_box2d { 


template <typename D, typename Policy>
void buildMetaClass_B2World(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (const b2Vec2 &, bool)>(_p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetDestructionListener", _r), &D::ClassType::SetDestructionListener, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetContactFilter", _r), &D::ClassType::SetContactFilter, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetContactListener", _r), &D::ClassType::SetContactListener, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetDebugDraw", _r), &D::ClassType::SetDebugDraw, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("CreateBody", _r), &D::ClassType::CreateBody, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DestroyBody", _r), &D::ClassType::DestroyBody, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("CreateJoint", _r), &D::ClassType::CreateJoint, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DestroyJoint", _r), &D::ClassType::DestroyJoint, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Step", _r), &D::ClassType::Step, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ClearForces", _r), &D::ClassType::ClearForces, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DrawDebugData", _r), &D::ClassType::DrawDebugData, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("QueryAABB", _r), &D::ClassType::QueryAABB, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("RayCast", _r), &D::ClassType::RayCast, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetBodyList", _r), &D::ClassType::GetBodyList, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetJointList", _r), &D::ClassType::GetJointList, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetContactList", _r), &D::ClassType::GetContactList, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetWarmStarting", _r), &D::ClassType::SetWarmStarting, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetContinuousPhysics", _r), &D::ClassType::SetContinuousPhysics, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetProxyCount", _r), &D::ClassType::GetProxyCount, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetBodyCount", _r), &D::ClassType::GetBodyCount, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetJointCount", _r), &D::ClassType::GetJointCount, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetContactCount", _r), &D::ClassType::GetContactCount, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetGravity", _r), &D::ClassType::SetGravity, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetGravity", _r), &D::ClassType::GetGravity, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsLocked", _r), &D::ClassType::IsLocked, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetAutoClearForces", _r), &D::ClassType::SetAutoClearForces, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAutoClearForces", _r), &D::ClassType::GetAutoClearForces, _p);
}


} // namespace meta_box2d 




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif