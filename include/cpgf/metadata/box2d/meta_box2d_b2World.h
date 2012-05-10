// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2WORLD_H
#define __META_BOX2D_B2WORLD_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2World(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (const b2Vec2 &, bool)>();
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetDestructionListener", _r), &D::ClassType::SetDestructionListener);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetContactFilter", _r), &D::ClassType::SetContactFilter);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetContactListener", _r), &D::ClassType::SetContactListener);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetDebugDraw", _r), &D::ClassType::SetDebugDraw);
    _d.CPGF_MD_TEMPLATE _method(replaceName("CreateBody", _r), &D::ClassType::CreateBody);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DestroyBody", _r), &D::ClassType::DestroyBody);
    _d.CPGF_MD_TEMPLATE _method(replaceName("CreateJoint", _r), &D::ClassType::CreateJoint);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DestroyJoint", _r), &D::ClassType::DestroyJoint);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Step", _r), &D::ClassType::Step);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ClearForces", _r), &D::ClassType::ClearForces);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DrawDebugData", _r), &D::ClassType::DrawDebugData);
    _d.CPGF_MD_TEMPLATE _method(replaceName("QueryAABB", _r), &D::ClassType::QueryAABB);
    _d.CPGF_MD_TEMPLATE _method(replaceName("RayCast", _r), &D::ClassType::RayCast);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetBodyList", _r), &D::ClassType::GetBodyList);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetJointList", _r), &D::ClassType::GetJointList);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetContactList", _r), &D::ClassType::GetContactList);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetWarmStarting", _r), &D::ClassType::SetWarmStarting);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetContinuousPhysics", _r), &D::ClassType::SetContinuousPhysics);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetProxyCount", _r), &D::ClassType::GetProxyCount);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetBodyCount", _r), &D::ClassType::GetBodyCount);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetJointCount", _r), &D::ClassType::GetJointCount);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetContactCount", _r), &D::ClassType::GetContactCount);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetGravity", _r), &D::ClassType::SetGravity);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetGravity", _r), &D::ClassType::GetGravity);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsLocked", _r), &D::ClassType::IsLocked);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetAutoClearForces", _r), &D::ClassType::SetAutoClearForces);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAutoClearForces", _r), &D::ClassType::GetAutoClearForces);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
