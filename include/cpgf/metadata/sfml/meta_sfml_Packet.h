// Auto generated file, don't modify.

#ifndef __META_SFML_PACKET_H
#define __META_SFML_PACKET_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


inline Packet & opErAToRWrapper_Packet__opRightShift(Packet * self, bool & Data) {
    return (*self) >> Data;
}
inline Packet & opErAToRWrapper_Packet__opRightShift(Packet * self, Int8 & Data) {
    return (*self) >> Data;
}
inline Packet & opErAToRWrapper_Packet__opRightShift(Packet * self, Uint8 & Data) {
    return (*self) >> Data;
}
inline Packet & opErAToRWrapper_Packet__opRightShift(Packet * self, Int16 & Data) {
    return (*self) >> Data;
}
inline Packet & opErAToRWrapper_Packet__opRightShift(Packet * self, Uint16 & Data) {
    return (*self) >> Data;
}
inline Packet & opErAToRWrapper_Packet__opRightShift(Packet * self, Int32 & Data) {
    return (*self) >> Data;
}
inline Packet & opErAToRWrapper_Packet__opRightShift(Packet * self, Uint32 & Data) {
    return (*self) >> Data;
}
inline Packet & opErAToRWrapper_Packet__opRightShift(Packet * self, float & Data) {
    return (*self) >> Data;
}
inline Packet & opErAToRWrapper_Packet__opRightShift(Packet * self, double & Data) {
    return (*self) >> Data;
}
inline Packet & opErAToRWrapper_Packet__opRightShift(Packet * self, char * Data) {
    return (*self) >> Data;
}
inline Packet & opErAToRWrapper_Packet__opRightShift(Packet * self, std::string & Data) {
    return (*self) >> Data;
}
inline Packet & opErAToRWrapper_Packet__opRightShift(Packet * self, wchar_t * Data) {
    return (*self) >> Data;
}
inline Packet & opErAToRWrapper_Packet__opRightShift(Packet * self, std::wstring & Data) {
    return (*self) >> Data;
}
inline Packet & opErAToRWrapper_Packet__opLeftShift(Packet * self, bool Data) {
    return (*self) << Data;
}
inline Packet & opErAToRWrapper_Packet__opLeftShift(Packet * self, Int8 Data) {
    return (*self) << Data;
}
inline Packet & opErAToRWrapper_Packet__opLeftShift(Packet * self, Uint8 Data) {
    return (*self) << Data;
}
inline Packet & opErAToRWrapper_Packet__opLeftShift(Packet * self, Int16 Data) {
    return (*self) << Data;
}
inline Packet & opErAToRWrapper_Packet__opLeftShift(Packet * self, Uint16 Data) {
    return (*self) << Data;
}
inline Packet & opErAToRWrapper_Packet__opLeftShift(Packet * self, Int32 Data) {
    return (*self) << Data;
}
inline Packet & opErAToRWrapper_Packet__opLeftShift(Packet * self, Uint32 Data) {
    return (*self) << Data;
}
inline Packet & opErAToRWrapper_Packet__opLeftShift(Packet * self, float Data) {
    return (*self) << Data;
}
inline Packet & opErAToRWrapper_Packet__opLeftShift(Packet * self, double Data) {
    return (*self) << Data;
}
inline Packet & opErAToRWrapper_Packet__opLeftShift(Packet * self, const char * Data) {
    return (*self) << Data;
}
inline Packet & opErAToRWrapper_Packet__opLeftShift(Packet * self, const std::string & Data) {
    return (*self) << Data;
}
inline Packet & opErAToRWrapper_Packet__opLeftShift(Packet * self, const wchar_t * Data) {
    return (*self) << Data;
}
inline Packet & opErAToRWrapper_Packet__opLeftShift(Packet * self, const std::wstring & Data) {
    return (*self) << Data;
}


template <typename D>
void buildMetaClass_Packet(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method("Append", &D::ClassType::Append);
    _d.CPGF_MD_TEMPLATE _method("Clear", &D::ClassType::Clear);
    _d.CPGF_MD_TEMPLATE _method("GetData", &D::ClassType::GetData);
    _d.CPGF_MD_TEMPLATE _method("GetDataSize", &D::ClassType::GetDataSize);
    _d.CPGF_MD_TEMPLATE _method("EndOfPacket", &D::ClassType::EndOfPacket);
    _d.CPGF_MD_TEMPLATE _operator< bool (cpgf::GMetaSelf)>(mopHolder());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, bool &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opRightShift", (Packet & (*) (Packet *, bool &))&opErAToRWrapper_Packet__opRightShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Int8 &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opRightShift", (Packet & (*) (Packet *, Int8 &))&opErAToRWrapper_Packet__opRightShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Uint8 &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opRightShift", (Packet & (*) (Packet *, Uint8 &))&opErAToRWrapper_Packet__opRightShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Int16 &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opRightShift", (Packet & (*) (Packet *, Int16 &))&opErAToRWrapper_Packet__opRightShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Uint16 &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opRightShift", (Packet & (*) (Packet *, Uint16 &))&opErAToRWrapper_Packet__opRightShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Int32 &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opRightShift", (Packet & (*) (Packet *, Int32 &))&opErAToRWrapper_Packet__opRightShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Uint32 &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opRightShift", (Packet & (*) (Packet *, Uint32 &))&opErAToRWrapper_Packet__opRightShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, float &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opRightShift", (Packet & (*) (Packet *, float &))&opErAToRWrapper_Packet__opRightShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, double &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opRightShift", (Packet & (*) (Packet *, double &))&opErAToRWrapper_Packet__opRightShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, char *)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opRightShift", (Packet & (*) (Packet *, char *))&opErAToRWrapper_Packet__opRightShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, std::string &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opRightShift", (Packet & (*) (Packet *, std::string &))&opErAToRWrapper_Packet__opRightShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, wchar_t *)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opRightShift", (Packet & (*) (Packet *, wchar_t *))&opErAToRWrapper_Packet__opRightShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, std::wstring &)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opRightShift", (Packet & (*) (Packet *, std::wstring &))&opErAToRWrapper_Packet__opRightShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, bool)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opLeftShift", (Packet & (*) (Packet *, bool))&opErAToRWrapper_Packet__opLeftShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Int8)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opLeftShift", (Packet & (*) (Packet *, Int8))&opErAToRWrapper_Packet__opLeftShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Uint8)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opLeftShift", (Packet & (*) (Packet *, Uint8))&opErAToRWrapper_Packet__opLeftShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Int16)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opLeftShift", (Packet & (*) (Packet *, Int16))&opErAToRWrapper_Packet__opLeftShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Uint16)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opLeftShift", (Packet & (*) (Packet *, Uint16))&opErAToRWrapper_Packet__opLeftShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Int32)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opLeftShift", (Packet & (*) (Packet *, Int32))&opErAToRWrapper_Packet__opLeftShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, Uint32)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opLeftShift", (Packet & (*) (Packet *, Uint32))&opErAToRWrapper_Packet__opLeftShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, float)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opLeftShift", (Packet & (*) (Packet *, float))&opErAToRWrapper_Packet__opLeftShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, double)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opLeftShift", (Packet & (*) (Packet *, double))&opErAToRWrapper_Packet__opLeftShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, const char *)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opLeftShift", (Packet & (*) (Packet *, const char *))&opErAToRWrapper_Packet__opLeftShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder << mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opLeftShift", (Packet & (*) (Packet *, const std::string &))&opErAToRWrapper_Packet__opLeftShift, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, const wchar_t *)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opLeftShift", (Packet & (*) (Packet *, const wchar_t *))&opErAToRWrapper_Packet__opLeftShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Packet & (*)(cpgf::GMetaSelf, const std::wstring &)>(mopHolder << mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opLeftShift", (Packet & (*) (Packet *, const std::wstring &))&opErAToRWrapper_Packet__opLeftShift, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
