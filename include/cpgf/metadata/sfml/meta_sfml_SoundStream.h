// Auto generated file, don't modify.

#ifndef __META_SFML_SOUNDSTREAM_H
#define __META_SFML_SOUNDSTREAM_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindapi.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_SoundStream(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("Play", _r), &D::ClassType::Play);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Stop", _r), &D::ClassType::Stop);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetChannelsCount", _r), &D::ClassType::GetChannelsCount);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetSampleRate", _r), &D::ClassType::GetSampleRate);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetStatus", _r), &D::ClassType::GetStatus);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetPlayingOffset", _r), &D::ClassType::GetPlayingOffset);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetLoop", _r), &D::ClassType::SetLoop);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetLoop", _r), &D::ClassType::GetLoop);
    _d.CPGF_MD_TEMPLATE _enum<typename D::ClassType::Status>(replaceName("Status", _r))
    ;
    {
        GDefineMetaClass<SoundStream::Chunk> _nd = GDefineMetaClass<SoundStream::Chunk>::declare("Chunk");
        _nd.CPGF_MD_TEMPLATE _field(replaceName("Samples", _r), &sf::SoundStream::Chunk::Samples);
        _nd.CPGF_MD_TEMPLATE _field(replaceName("NbSamples", _r), &sf::SoundStream::Chunk::NbSamples);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
