// Auto generated file, don't modify.

#ifndef CPGF_META_TEST_TESTOBJECTARRAY_H
#define CPGF_META_TEST_TESTOBJECTARRAY_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_test { 


template <typename D>
void buildMetaClass_Global_testobjectarray(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getOAObjectN", (int (*) (OAObject *))&getOAObjectN);
    _d.CPGF_MD_TEMPLATE _method("getOAObjectN", (int (*) (OAObject *, int))&getOAObjectN);
    _d.CPGF_MD_TEMPLATE _method("getOAObjectS", (std::string (*) (const OAObject &))&getOAObjectS);
}


template <typename D>
void buildMetaClass_OAData(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (int, const std::string &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _field("n", &D::ClassType::n);
    _d.CPGF_MD_TEMPLATE _field("s", &D::ClassType::s);
}


template <typename D>
void buildMetaClass_OAObject(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (int, const std::string &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _field("data", &D::ClassType::data);
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
