// Auto generated file, don't modify.

#ifndef __META_SFML_SOUND_H
#define __META_SFML_SOUND_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Sound(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const SoundBuffer &, bool, float, float, const Vector3f &)>()
        ._default(copyVariantFromCopyable(Vector3f(0, 0, 0)))
        ._default(copyVariantFromCopyable(100.f))
        ._default(copyVariantFromCopyable(1.f))
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _constructor<void * (const Sound &)>();
    _d.CPGF_MD_TEMPLATE _method(replaceName("Play", _r), &D::ClassType::Play);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Pause", _r), &D::ClassType::Pause);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Stop", _r), &D::ClassType::Stop);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetBuffer", _r), &D::ClassType::SetBuffer);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetLoop", _r), &D::ClassType::SetLoop);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetPitch", _r), &D::ClassType::SetPitch);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetVolume", _r), &D::ClassType::SetVolume);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetPosition", _r), (void (D::ClassType::*) (float, float, float))&D::ClassType::SetPosition);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetPosition", _r), (void (D::ClassType::*) (const Vector3f &))&D::ClassType::SetPosition);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetRelativeToListener", _r), &D::ClassType::SetRelativeToListener);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetMinDistance", _r), &D::ClassType::SetMinDistance);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetAttenuation", _r), &D::ClassType::SetAttenuation);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetPlayingOffset", _r), &D::ClassType::SetPlayingOffset);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetBuffer", _r), &D::ClassType::GetBuffer);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetLoop", _r), &D::ClassType::GetLoop);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetPitch", _r), &D::ClassType::GetPitch);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetVolume", _r), &D::ClassType::GetVolume);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetPosition", _r), &D::ClassType::GetPosition);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsRelativeToListener", _r), &D::ClassType::IsRelativeToListener);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetMinDistance", _r), &D::ClassType::GetMinDistance);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAttenuation", _r), &D::ClassType::GetAttenuation);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetStatus", _r), &D::ClassType::GetStatus);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetPlayingOffset", _r), &D::ClassType::GetPlayingOffset);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ResetBuffer", _r), &D::ClassType::ResetBuffer);
    _d.CPGF_MD_TEMPLATE _enum<typename D::ClassType::Status>(replaceName("Status", _r))
        ._element(replaceName("Stopped", _r), D::ClassType::Stopped)
        ._element(replaceName("Paused", _r), D::ClassType::Paused)
        ._element(replaceName("Playing", _r), D::ClassType::Playing)
    ;
    _d.CPGF_MD_TEMPLATE _operator<Sound & (*)(cpgf::GMetaSelf, const Sound &)>(mopHolder = mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleParamNoncopyable<-1> >());
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
