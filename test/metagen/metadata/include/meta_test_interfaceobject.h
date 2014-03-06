// Auto generated file, don't modify.

#ifndef CPGF_META_TEST_INTERFACEOBJECT_H
#define CPGF_META_TEST_INTERFACEOBJECT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_test { 


template <typename D>
void buildMetaClass_MyInterfaceHolder(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (cpgf::IObject *)>();
}


template <typename D>
void buildMetaClass_MyInterfaceObject(D _d)
{
    (void)_d;
    using namespace cpgf;
    
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
