// Auto generated file, don't modify.

#ifndef __META_TEST_SIMPLEOBJECT_H
#define __META_TEST_SIMPLEOBJECT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace cpgf;


namespace meta_test { 


template <typename D>
void buildMetaClass_Global_simpleobject(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getSimpleObjectClass", (cpgf::IMetaClass * (*) (cpgf::IMetaClass *))&getSimpleObjectClass);
    _d.CPGF_MD_TEMPLATE _method("metaClassIsSimpleObject", (bool (*) (cpgf::IMetaClass *))&metaClassIsSimpleObject);
    _d.CPGF_MD_TEMPLATE _method("createSharedSimpleObject", (cpgf::GSharedPointer< SimpleObject > (*) ())&createSharedSimpleObject);
}


template <typename D>
void buildMetaClass_SimpleAtom(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("value", &D::ClassType::value);
}


template <typename D>
void buildMetaClass_SimpleData(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (int, const std::string &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(""))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _constructor<void * (const SimpleData &)>();
    _d.CPGF_MD_TEMPLATE _field("n", &D::ClassType::n);
    _d.CPGF_MD_TEMPLATE _field("s", &D::ClassType::s);
    _d.CPGF_MD_TEMPLATE _field("ws", &D::ClassType::ws);
    _d.CPGF_MD_TEMPLATE _field("atom", &D::ClassType::atom);
}


template <typename D>
void buildMetaClass_SimpleObject(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("data", &D::ClassType::data);
    _d.CPGF_MD_TEMPLATE _method("getData", &D::ClassType::getData);
    _d.CPGF_MD_TEMPLATE _method("getDataPtr", &D::ClassType::getDataPtr);
    _d.CPGF_MD_TEMPLATE _method("setData", &D::ClassType::setData);
    _d.CPGF_MD_TEMPLATE _method("setDataPtr", &D::ClassType::setDataPtr);
    _d.CPGF_MD_TEMPLATE _method("checkData", (bool (D::ClassType::*) (int) const)&D::ClassType::checkData);
    _d.CPGF_MD_TEMPLATE _method("checkData", (bool (D::ClassType::*) (const std::string &) const)&D::ClassType::checkData, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("checkAtom", &D::ClassType::checkAtom);
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
