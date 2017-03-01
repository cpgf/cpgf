// Auto generated file, don't modify.

#ifndef CPGF_META_TEST_SIMPLEOBJECT_H
#define CPGF_META_TEST_SIMPLEOBJECT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace cpgf;


namespace meta_test {


template <typename D>
void buildMetaClass_Global_simpleobject(D _d)
{
    (void)_d;
    using namespace cpgf;

    _d.CPGF_MD_TEMPLATE _method("getSimpleObjectClass", (cpgf::IMetaClass * (*) (cpgf::IMetaClass *))&getSimpleObjectClass);
    _d.CPGF_MD_TEMPLATE _method("metaClassIsSimpleObject", (bool (*) (cpgf::IMetaClass *))&metaClassIsSimpleObject);
    _d.CPGF_MD_TEMPLATE _method("createSharedSimpleObject", (cpgf::GSharedPointer< SimpleObject > (*) ())&createSharedSimpleObject);
    _d.CPGF_MD_TEMPLATE _method("getSharedSimpleObject", (SimpleObject * (*) (cpgf::GSharedPointer< SimpleObject >))&getSharedSimpleObject);
    _d.CPGF_MD_TEMPLATE _method("createCpp11SharedSimpleObject", (std::shared_ptr<SimpleObject> (*) ())&createCpp11SharedSimpleObject);
    _d.CPGF_MD_TEMPLATE _method("getCpp11SharedSimpleObject", (SimpleObject * (*) (std::shared_ptr<SimpleObject>))&getCpp11SharedSimpleObject);
    _d.CPGF_MD_TEMPLATE _method("createBoostSharedSimpleObject", (BOOST_SP (*) ())&createBoostSharedSimpleObject);
    _d.CPGF_MD_TEMPLATE _method("getBoostSharedSimpleObject", (SimpleObject * (*) (BOOST_SP))&getBoostSharedSimpleObject);
    _d.CPGF_MD_TEMPLATE _method("getSimpleObjectPointerN", (int (*) (SimpleObject *))&getSimpleObjectPointerN);
    _d.CPGF_MD_TEMPLATE _method("getSimpleObjectN", (int (*) (const SimpleObject &))&getSimpleObjectN);
    _d.CPGF_MD_TEMPLATE _method("setViaConstRefPtrApi", &setViaConstRefPtrApi);
    _d.CPGF_MD_TEMPLATE _method("returnFromConstRefPtrApi", &returnFromConstRefPtrApi);
}


template <typename D>
void buildMetaClass_SimpleAtom(D _d)
{
    (void)_d;
    using namespace cpgf;

    _d.CPGF_MD_TEMPLATE _field("value", &D::ClassType::value);
}


template <typename D>
void buildMetaClass_SimpleData(D _d)
{
    (void)_d;
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


inline SimpleData & opErAToRWrapper_SimpleObject__opFunction(SimpleObject * self) {
    return (*self)();
}
inline const SimpleData & opErAToRWrapper_SimpleObject__opFunction(const SimpleObject * self) {
    return (*self)();
}


template <typename D>
void buildMetaClass_SimpleObject(D _d)
{
    (void)_d;
    using namespace cpgf;

    _d.CPGF_MD_TEMPLATE _field("data", &D::ClassType::data);
    _d.CPGF_MD_TEMPLATE _field("pobj", &D::ClassType::pobj);
    _d.CPGF_MD_TEMPLATE _method("getData", &D::ClassType::getData);
    _d.CPGF_MD_TEMPLATE _method("getDataPtr", &D::ClassType::getDataPtr);
    _d.CPGF_MD_TEMPLATE _method("setData", &D::ClassType::setData);
    _d.CPGF_MD_TEMPLATE _method("setDataPtr", &D::ClassType::setDataPtr);
    _d.CPGF_MD_TEMPLATE _method("checkData", (bool (D::ClassType::*) (int) const)&D::ClassType::checkData);
    _d.CPGF_MD_TEMPLATE _method("checkData", (bool (D::ClassType::*) (const std::string &) const)&D::ClassType::checkData, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("checkAtom", &D::ClassType::checkAtom);
    _d.CPGF_MD_TEMPLATE _operator<SimpleData & (*)()>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _method("_opFunction", (SimpleData & (*) (SimpleObject *))&opErAToRWrapper_SimpleObject__opFunction, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<const SimpleData & (*)()>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _method("_opFunction", (const SimpleData & (*) (const SimpleObject *))&opErAToRWrapper_SimpleObject__opFunction, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
