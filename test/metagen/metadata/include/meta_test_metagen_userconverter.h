// Auto generated file, don't modify.

#ifndef CPGF_META_TEST_METAGEN_USERCONVERTER_H
#define CPGF_META_TEST_METAGEN_USERCONVERTER_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_test { 


template <typename D>
void buildMetaClass_Global_metagen_userconverter(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("testUserConverterGetDoubleValue", (int (*) (const TestUserConverterData &))&testUserConverterGetDoubleValue, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
}


template <typename D>
void buildMetaClass_TestUserConverterData(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (int)>();
    _d.CPGF_MD_TEMPLATE _field("value", &D::ClassType::value);
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
