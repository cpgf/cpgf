// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2CONTACT_H
#define __META_BOX2D_B2CONTACT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2Contact(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("GetManifold", (b2Manifold * (D::ClassType::*) ())&D::ClassType::GetManifold);
    _d.CPGF_MD_TEMPLATE _method("GetManifold", (const b2Manifold * (D::ClassType::*) () const)&D::ClassType::GetManifold);
    _d.CPGF_MD_TEMPLATE _method("GetWorldManifold", &D::ClassType::GetWorldManifold);
    _d.CPGF_MD_TEMPLATE _method("IsTouching", &D::ClassType::IsTouching);
    _d.CPGF_MD_TEMPLATE _method("SetEnabled", &D::ClassType::SetEnabled);
    _d.CPGF_MD_TEMPLATE _method("IsEnabled", &D::ClassType::IsEnabled);
    _d.CPGF_MD_TEMPLATE _method("GetNext", (b2Contact * (D::ClassType::*) ())&D::ClassType::GetNext);
    _d.CPGF_MD_TEMPLATE _method("GetNext", (const b2Contact * (D::ClassType::*) () const)&D::ClassType::GetNext);
    _d.CPGF_MD_TEMPLATE _method("GetFixtureA", (b2Fixture * (D::ClassType::*) ())&D::ClassType::GetFixtureA);
    _d.CPGF_MD_TEMPLATE _method("GetFixtureA", (const b2Fixture * (D::ClassType::*) () const)&D::ClassType::GetFixtureA);
    _d.CPGF_MD_TEMPLATE _method("GetFixtureB", (b2Fixture * (D::ClassType::*) ())&D::ClassType::GetFixtureB);
    _d.CPGF_MD_TEMPLATE _method("GetFixtureB", (const b2Fixture * (D::ClassType::*) () const)&D::ClassType::GetFixtureB);
    _d.CPGF_MD_TEMPLATE _method("Evaluate", &D::ClassType::Evaluate);
}


template <typename D>
void buildMetaClass_B2ContactEdge(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("other", &D::ClassType::other);
    _d.CPGF_MD_TEMPLATE _field("contact", &D::ClassType::contact);
    _d.CPGF_MD_TEMPLATE _field("prev", &D::ClassType::prev);
    _d.CPGF_MD_TEMPLATE _field("next", &D::ClassType::next);
}


template <typename D>
void buildMetaClass_B2ContactRegister(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("createFcn", &D::ClassType::createFcn);
    _d.CPGF_MD_TEMPLATE _field("destroyFcn", &D::ClassType::destroyFcn);
    _d.CPGF_MD_TEMPLATE _field("primary", &D::ClassType::primary);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
