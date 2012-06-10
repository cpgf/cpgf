// Auto generated file, don't modify.

#ifndef __META_SFML_PACKET_H
#define __META_SFML_PACKET_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Packet(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method(replaceName("Append", _r), &D::ClassType::Append);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Clear", _r), &D::ClassType::Clear);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetData", _r), &D::ClassType::GetData);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetDataSize", _r), &D::ClassType::GetDataSize);
    _d.CPGF_MD_TEMPLATE _method(replaceName("EndOfPacket", _r), &D::ClassType::EndOfPacket);
    _d.CPGF_MD_TEMPLATE _operator< bool (cpgf::GMetaSelf)>(mopHolder(), cpgf::MakePolicy<cpgf::GMetaRuleParamNoncopyable<-1> >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, bool &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Int8 &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Uint8 &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Int16 &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Uint16 &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Int32 &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Uint32 &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, float &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, double &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, char *)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, std::string &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, wchar_t *)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, std::wstring &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, bool)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Int8)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Uint8)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Int16)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Uint16)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Int32)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Uint32)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, float)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, double)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, const char *)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder << mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, const wchar_t *)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, const std::wstring &)>(mopHolder << mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
