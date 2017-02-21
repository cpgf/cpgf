// Auto generated file, don't modify.

#ifndef CPGF_META_TEST_TESTMETAMETHOD_H
#define CPGF_META_TEST_TESTMETAMETHOD_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_test { 


template <typename D>
void buildMetaClass_TestMethodMethodClass(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method("func_null_38_abc", &D::ClassType::func_null_38_abc);
    _d.CPGF_MD_TEMPLATE _method("func_return_What", &D::ClassType::func_return_What);
    _d.CPGF_MD_TEMPLATE _method("func_return_ReferenceToString", &D::ClassType::func_return_ReferenceToString, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
    _d.CPGF_MD_TEMPLATE _method("func_return_data", &D::ClassType::func_return_data);
    _d.CPGF_MD_TEMPLATE _method("sfunc_dEf_neg99", &D::ClassType::sfunc_dEf_neg99, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
}


template <typename D>
void buildMetaClass_TestMethodMethodData(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method("check", &D::ClassType::check);
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
