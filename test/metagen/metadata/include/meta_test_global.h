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
    _d.CPGF_MD_TEMPLATE _field(replaceName("globalStdWideString", _r), &globalStdWideString);
    _d.CPGF_MD_TEMPLATE _field(replaceName("globalRaw", _r), &globalRaw);
    _d.CPGF_MD_TEMPLATE _method(replaceName("getCString", _r), (const char * (*) (int))&getCString);
    _d.CPGF_MD_TEMPLATE _method(replaceName("getStdString", _r), (std::string (*) (const char *))&getStdString);
    _d.CPGF_MD_TEMPLATE _method(replaceName("getCWideString", _r), (const wchar_t * (*) (int))&getCWideString);
    _d.CPGF_MD_TEMPLATE _method(replaceName("getStdWideString", _r), (std::wstring (*) (const wchar_t *))&getStdWideString);
    _d.CPGF_MD_TEMPLATE _method(replaceName("checkGlobalRaw", _r), (bool (*) (const void ***))&checkGlobalRaw);
    _d.CPGF_MD_TEMPLATE _enum<GlobalEnum>(replaceName("GlobalEnum", _r))
        ._element(replaceName("globalEnumA", _r), globalEnumA)
        ._element(replaceName("globalEnumB", _r), globalEnumB)
    ;
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
