// Auto generated file, don't modify.

#ifndef __META_TEST_SIMPLEOBJECT_H
#define __META_TEST_SIMPLEOBJECT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_test { 


template <typename D>
void buildMetaClass_SimpleAtom(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("value", _r), &D::ClassType::value);
}


template <typename D>
void buildMetaClass_SimpleData(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (int, const std::string &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(""))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _constructor<void * (const SimpleData &)>();
    _d.CPGF_MD_TEMPLATE _field(replaceName("n", _r), &D::ClassType::n);
    _d.CPGF_MD_TEMPLATE _field(replaceName("s", _r), &D::ClassType::s);
    _d.CPGF_MD_TEMPLATE _field(replaceName("ws", _r), &D::ClassType::ws);
    _d.CPGF_MD_TEMPLATE _field(replaceName("atom", _r), &D::ClassType::atom);
}


template <typename D>
void buildMetaClass_SimpleObject(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("data", _r), &D::ClassType::data);
    _d.CPGF_MD_TEMPLATE _method(replaceName("getData", _r), &D::ClassType::getData);
    _d.CPGF_MD_TEMPLATE _method(replaceName("getDataPtr", _r), &D::ClassType::getDataPtr);
    _d.CPGF_MD_TEMPLATE _method(replaceName("setData", _r), &D::ClassType::setData);
    _d.CPGF_MD_TEMPLATE _method(replaceName("setDataPtr", _r), &D::ClassType::setDataPtr);
    _d.CPGF_MD_TEMPLATE _method(replaceName("checkData", _r), (bool (D::ClassType::*) (int) const)&D::ClassType::checkData);
    _d.CPGF_MD_TEMPLATE _method(replaceName("checkData", _r), (bool (D::ClassType::*) (const std::string &) const)&D::ClassType::checkData, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("checkAtom", _r), &D::ClassType::checkAtom);
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
