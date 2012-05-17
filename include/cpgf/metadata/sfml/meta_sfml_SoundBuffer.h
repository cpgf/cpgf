// Auto generated file, don't modify.

#ifndef __META_SFML_SOUNDBUFFER_H
#define __META_SFML_SOUNDBUFFER_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindapi.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_SoundBuffer(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const SoundBuffer &)>();
    _d.CPGF_MD_TEMPLATE _method(replaceName("LoadFromFile", _r), &D::ClassType::LoadFromFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("LoadFromMemory", _r), &D::ClassType::LoadFromMemory);
    _d.CPGF_MD_TEMPLATE _method(replaceName("LoadFromSamples", _r), &D::ClassType::LoadFromSamples);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SaveToFile", _r), &D::ClassType::SaveToFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetSamples", _r), &D::ClassType::GetSamples);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetSamplesCount", _r), &D::ClassType::GetSamplesCount);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetSampleRate", _r), &D::ClassType::GetSampleRate);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetChannelsCount", _r), &D::ClassType::GetChannelsCount);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetDuration", _r), &D::ClassType::GetDuration);
    _d.CPGF_MD_TEMPLATE _operator<SoundBuffer & (*)(cpgf::GMetaSelf, const SoundBuffer &)>(mopHolder = mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleParamNoncopyable<-1> >());
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
