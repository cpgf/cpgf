// Auto generated file, don't modify.

#ifndef CPGF_META_TEST_METAGEN_RETURN_THIS_H
#define CPGF_META_TEST_METAGEN_RETURN_THIS_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_test { 


template <typename D>
void buildMetaClass_MetagenReturnThis(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method("check", &D::ClassType::check);
    _d.CPGF_MD_TEMPLATE _method("selfRef", &D::ClassType::selfRef);
    _d.CPGF_MD_TEMPLATE _method("selfPointer", &D::ClassType::selfPointer);
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
