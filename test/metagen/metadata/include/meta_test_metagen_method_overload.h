// Auto generated file, don't modify.

#ifndef CPGF_META_TEST_METAGEN_METHOD_OVERLOAD_H
#define CPGF_META_TEST_METAGEN_METHOD_OVERLOAD_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_test { 


template <typename D>
void buildMetaClass_MetagenMethodOverload(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("value", &D::ClassType::value);
    _d.CPGF_MD_TEMPLATE _method("makeConst", &D::ClassType::makeConst);
    _d.CPGF_MD_TEMPLATE _method("testOverloadByConst_ConstFirst", (int (D::ClassType::*) () const)&D::ClassType::testOverloadByConst_ConstFirst);
    _d.CPGF_MD_TEMPLATE _method("testOverloadByConst_ConstFirst", (int (D::ClassType::*) ())&D::ClassType::testOverloadByConst_ConstFirst);
    _d.CPGF_MD_TEMPLATE _method("testOverloadByConst_NonConstFirst", (int (D::ClassType::*) ())&D::ClassType::testOverloadByConst_NonConstFirst);
    _d.CPGF_MD_TEMPLATE _method("testOverloadByConst_NonConstFirst", (int (D::ClassType::*) () const)&D::ClassType::testOverloadByConst_NonConstFirst);
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
