// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2WORLD_H
#define __META_BOX2D_B2WORLD_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2World(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (const b2Vec2 &, bool)>();
    _d.CPGF_MD_TEMPLATE _method("SetDestructionListener", &D::ClassType::SetDestructionListener);
    _d.CPGF_MD_TEMPLATE _method("SetContactFilter", &D::ClassType::SetContactFilter);
    _d.CPGF_MD_TEMPLATE _method("SetContactListener", &D::ClassType::SetContactListener);
    _d.CPGF_MD_TEMPLATE _method("SetDebugDraw", &D::ClassType::SetDebugDraw);
    _d.CPGF_MD_TEMPLATE _method("CreateBody", &D::ClassType::CreateBody);
    _d.CPGF_MD_TEMPLATE _method("DestroyBody", &D::ClassType::DestroyBody);
    _d.CPGF_MD_TEMPLATE _method("CreateJoint", &D::ClassType::CreateJoint);
    _d.CPGF_MD_TEMPLATE _method("DestroyJoint", &D::ClassType::DestroyJoint);
    _d.CPGF_MD_TEMPLATE _method("Step", &D::ClassType::Step);
    _d.CPGF_MD_TEMPLATE _method("ClearForces", &D::ClassType::ClearForces);
    _d.CPGF_MD_TEMPLATE _method("DrawDebugData", &D::ClassType::DrawDebugData);
    _d.CPGF_MD_TEMPLATE _method("QueryAABB", &D::ClassType::QueryAABB);
    _d.CPGF_MD_TEMPLATE _method("RayCast", &D::ClassType::RayCast);
    _d.CPGF_MD_TEMPLATE _method("GetBodyList", &D::ClassType::GetBodyList);
    _d.CPGF_MD_TEMPLATE _method("GetJointList", &D::ClassType::GetJointList);
    _d.CPGF_MD_TEMPLATE _method("GetContactList", &D::ClassType::GetContactList);
    _d.CPGF_MD_TEMPLATE _method("SetWarmStarting", &D::ClassType::SetWarmStarting);
    _d.CPGF_MD_TEMPLATE _method("SetContinuousPhysics", &D::ClassType::SetContinuousPhysics);
    _d.CPGF_MD_TEMPLATE _method("GetProxyCount", &D::ClassType::GetProxyCount);
    _d.CPGF_MD_TEMPLATE _method("GetBodyCount", &D::ClassType::GetBodyCount);
    _d.CPGF_MD_TEMPLATE _method("GetJointCount", &D::ClassType::GetJointCount);
    _d.CPGF_MD_TEMPLATE _method("GetContactCount", &D::ClassType::GetContactCount);
    _d.CPGF_MD_TEMPLATE _method("SetGravity", &D::ClassType::SetGravity);
    _d.CPGF_MD_TEMPLATE _method("GetGravity", &D::ClassType::GetGravity);
    _d.CPGF_MD_TEMPLATE _method("IsLocked", &D::ClassType::IsLocked);
    _d.CPGF_MD_TEMPLATE _method("SetAutoClearForces", &D::ClassType::SetAutoClearForces);
    _d.CPGF_MD_TEMPLATE _method("GetAutoClearForces", &D::ClassType::GetAutoClearForces);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
