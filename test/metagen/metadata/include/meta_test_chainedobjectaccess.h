// Auto generated file, don't modify.

#ifndef CPGF_META_TEST_CHAINEDOBJECTACCESS_H
#define CPGF_META_TEST_CHAINEDOBJECTACCESS_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_test { 


template <typename D>
void buildMetaClass_ChainedObjectA(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method("getA", &D::ClassType::getA);
}


template <typename D>
void buildMetaClass_ChainedObjectB(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method("getA", &D::ClassType::getA);
    _d.CPGF_MD_TEMPLATE _method("getConstA", &D::ClassType::getConstA);
}


template <typename D>
void buildMetaClass_ChainedObjectC(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method("getB", &D::ClassType::getB);
    _d.CPGF_MD_TEMPLATE _method("getConstB", &D::ClassType::getConstB);
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
