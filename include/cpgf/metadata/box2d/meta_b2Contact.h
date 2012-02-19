//auto generated file, don't modify.
#ifndef __META_B2CONTACT_H
#define __META_B2CONTACT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


template <typename D, typename Policy>
void buildMetaClass_B2Contact(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetManifold", _r), (b2Manifold * (D::ClassType::*) ())&D::ClassType::GetManifold, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetManifold", _r), (const b2Manifold * (D::ClassType::*) () const)&D::ClassType::GetManifold, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetWorldManifold", _r), &D::ClassType::GetWorldManifold, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsTouching", _r), &D::ClassType::IsTouching, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetEnabled", _r), &D::ClassType::SetEnabled, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsEnabled", _r), &D::ClassType::IsEnabled, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetNext", _r), (b2Contact * (D::ClassType::*) ())&D::ClassType::GetNext, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetNext", _r), (const b2Contact * (D::ClassType::*) () const)&D::ClassType::GetNext, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetFixtureA", _r), (b2Fixture * (D::ClassType::*) ())&D::ClassType::GetFixtureA, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetFixtureA", _r), (const b2Fixture * (D::ClassType::*) () const)&D::ClassType::GetFixtureA, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetFixtureB", _r), (b2Fixture * (D::ClassType::*) ())&D::ClassType::GetFixtureB, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetFixtureB", _r), (const b2Fixture * (D::ClassType::*) () const)&D::ClassType::GetFixtureB, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Evaluate", _r), &D::ClassType::Evaluate, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2ContactEdge(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("other", _r), &D::ClassType::other, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("contact", _r), &D::ClassType::contact, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("prev", _r), &D::ClassType::prev, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("next", _r), &D::ClassType::next, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2ContactRegister(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("createFcn", _r), &D::ClassType::createFcn, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("destroyFcn", _r), &D::ClassType::destroyFcn, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("primary", _r), &D::ClassType::primary, _p);
}


template <typename D, typename Policy>
void buildMetaFile_B2Contact(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    buildMetaClass_B2Contact(config, _d, _r, _p);
    buildMetaClass_B2ContactEdge(config, _d, _r, _p);
    buildMetaClass_B2ContactRegister(config, _d, _r, _p);
}




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif