// Auto generated file, don't modify.

#ifndef __META_TEST_GLOBAL_H
#define __META_TEST_GLOBAL_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_test { 


template <typename D>
void buildMetaClass_Global_global(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("globalInt", _r), &globalInt);
    _d.CPGF_MD_TEMPLATE _field(replaceName("globalString", _r), &globalString);
    _d.CPGF_MD_TEMPLATE _field(replaceName("globalStdString", _r), &globalStdString);
    _d.CPGF_MD_TEMPLATE _method(replaceName("hello", _r), (const char * (*) (const char *))&hello);
    _d.CPGF_MD_TEMPLATE _method(replaceName("getStdString", _r), (std::string (*) (int))&getStdString);
    _d.CPGF_MD_TEMPLATE _enum<GlobalEnum>(replaceName("GlobalEnum", _r))
        ._element(replaceName("globalEnumA", _r), globalEnumA)
        ._element(replaceName("globalEnumB", _r), globalEnumB)
    ;
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
