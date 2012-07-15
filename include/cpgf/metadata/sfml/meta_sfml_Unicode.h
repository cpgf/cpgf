// Auto generated file, don't modify.

#ifndef __META_SFML_UNICODE_H
#define __META_SFML_UNICODE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Unicode(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    {
        GDefineMetaClass<Unicode::Text> _nd = GDefineMetaClass<Unicode::Text>::declare("Text");
        _nd.CPGF_MD_TEMPLATE _constructor<void * ()>();
        _nd.CPGF_MD_TEMPLATE _constructor<void * (const char *)>();
        _nd.CPGF_MD_TEMPLATE _constructor<void * (const wchar_t *)>();
        _nd.CPGF_MD_TEMPLATE _constructor<void * (const Uint8 *)>();
        _nd.CPGF_MD_TEMPLATE _constructor<void * (const Uint16 *)>();
        _nd.CPGF_MD_TEMPLATE _constructor<void * (const Uint32 *)>();
        _nd.CPGF_MD_TEMPLATE _constructor<void * (const std::string &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
        _nd.CPGF_MD_TEMPLATE _constructor<void * (const std::wstring &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
        _nd.CPGF_MD_TEMPLATE _constructor<void * (const Unicode::UTF8String &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
        _nd.CPGF_MD_TEMPLATE _constructor<void * (const Unicode::UTF16String &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
        _nd.CPGF_MD_TEMPLATE _constructor<void * (const Unicode::UTF32String &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
        _nd.CPGF_MD_TEMPLATE _operator< std::string (cpgf::GMetaSelf)>(mopHolder());
        _nd.CPGF_MD_TEMPLATE _operator< std::wstring (cpgf::GMetaSelf)>(mopHolder());
        _nd.CPGF_MD_TEMPLATE _operator< Unicode::UTF8String (cpgf::GMetaSelf)>(mopHolder());
        _nd.CPGF_MD_TEMPLATE _operator< Unicode::UTF16String (cpgf::GMetaSelf)>(mopHolder());
        _nd.CPGF_MD_TEMPLATE _operator< const Unicode::UTF32String & (cpgf::GMetaSelf)>(mopHolder(), cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
