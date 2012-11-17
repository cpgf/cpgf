// Auto generated file, don't modify.

#ifndef __META_SFML_SOUND_H
#define __META_SFML_SOUND_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


inline Sound & opErAToRWrapper_Sound__opAssign(Sound * self, const Sound & Other) {
    return (*self) = Other;
}


template <typename D>
void buildMetaClass_Sound(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const SoundBuffer &, bool, float, float, const Vector3f &)>()
        ._default(copyVariantFromCopyable(Vector3f(0, 0, 0)))
        ._default(copyVariantFromCopyable(100.f))
        ._default(copyVariantFromCopyable(1.f))
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _constructor<void * (const Sound &)>();
    _d.CPGF_MD_TEMPLATE _method("Play", &D::ClassType::Play);
    _d.CPGF_MD_TEMPLATE _method("Pause", &D::ClassType::Pause);
    _d.CPGF_MD_TEMPLATE _method("Stop", &D::ClassType::Stop);
    _d.CPGF_MD_TEMPLATE _method("SetBuffer", &D::ClassType::SetBuffer);
    _d.CPGF_MD_TEMPLATE _method("SetLoop", &D::ClassType::SetLoop);
    _d.CPGF_MD_TEMPLATE _method("SetPitch", &D::ClassType::SetPitch);
    _d.CPGF_MD_TEMPLATE _method("SetVolume", &D::ClassType::SetVolume);
    _d.CPGF_MD_TEMPLATE _method("SetPosition", (void (D::ClassType::*) (float, float, float))&D::ClassType::SetPosition);
    _d.CPGF_MD_TEMPLATE _method("SetPosition", (void (D::ClassType::*) (const Vector3f &))&D::ClassType::SetPosition);
    _d.CPGF_MD_TEMPLATE _method("SetRelativeToListener", &D::ClassType::SetRelativeToListener);
    _d.CPGF_MD_TEMPLATE _method("SetMinDistance", &D::ClassType::SetMinDistance);
    _d.CPGF_MD_TEMPLATE _method("SetAttenuation", &D::ClassType::SetAttenuation);
    _d.CPGF_MD_TEMPLATE _method("SetPlayingOffset", &D::ClassType::SetPlayingOffset);
    _d.CPGF_MD_TEMPLATE _method("GetBuffer", &D::ClassType::GetBuffer);
    _d.CPGF_MD_TEMPLATE _method("GetLoop", &D::ClassType::GetLoop);
    _d.CPGF_MD_TEMPLATE _method("GetPitch", &D::ClassType::GetPitch);
    _d.CPGF_MD_TEMPLATE _method("GetVolume", &D::ClassType::GetVolume);
    _d.CPGF_MD_TEMPLATE _method("GetPosition", &D::ClassType::GetPosition);
    _d.CPGF_MD_TEMPLATE _method("IsRelativeToListener", &D::ClassType::IsRelativeToListener);
    _d.CPGF_MD_TEMPLATE _method("GetMinDistance", &D::ClassType::GetMinDistance);
    _d.CPGF_MD_TEMPLATE _method("GetAttenuation", &D::ClassType::GetAttenuation);
    _d.CPGF_MD_TEMPLATE _method("GetStatus", &D::ClassType::GetStatus);
    _d.CPGF_MD_TEMPLATE _method("GetPlayingOffset", &D::ClassType::GetPlayingOffset);
    _d.CPGF_MD_TEMPLATE _method("ResetBuffer", &D::ClassType::ResetBuffer);
    _d.CPGF_MD_TEMPLATE _enum<typename D::ClassType::Status>("Status")
        ._element("Stopped", D::ClassType::Stopped)
        ._element("Paused", D::ClassType::Paused)
        ._element("Playing", D::ClassType::Playing)
    ;
    _d.CPGF_MD_TEMPLATE _operator<Sound & (*)(cpgf::GMetaSelf, const Sound &)>(mopHolder = mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleParamNoncopyable<-1> >());
    _d.CPGF_MD_TEMPLATE _method("_opAssign", (Sound & (*) (Sound *, const Sound &))&opErAToRWrapper_Sound__opAssign, cpgf::MakePolicy<cpgf::GMetaRuleParamNoncopyable<-1>, cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
