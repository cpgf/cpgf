// Auto generated file, don't modify.

#ifndef __META_SFML_SOUNDSTREAM_H
#define __META_SFML_SOUNDSTREAM_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_SoundStream(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("Play", &D::ClassType::Play);
    _d.CPGF_MD_TEMPLATE _method("Stop", &D::ClassType::Stop);
    _d.CPGF_MD_TEMPLATE _method("GetChannelsCount", &D::ClassType::GetChannelsCount);
    _d.CPGF_MD_TEMPLATE _method("GetSampleRate", &D::ClassType::GetSampleRate);
    _d.CPGF_MD_TEMPLATE _method("GetStatus", &D::ClassType::GetStatus);
    _d.CPGF_MD_TEMPLATE _method("GetPlayingOffset", &D::ClassType::GetPlayingOffset);
    _d.CPGF_MD_TEMPLATE _method("SetLoop", &D::ClassType::SetLoop);
    _d.CPGF_MD_TEMPLATE _method("GetLoop", &D::ClassType::GetLoop);
    _d.CPGF_MD_TEMPLATE _enum<typename D::ClassType::Status>("Status")
    ;
    {
        GDefineMetaClass<SoundStream::Chunk> _nd = GDefineMetaClass<SoundStream::Chunk>::declare("Chunk");
        _nd.CPGF_MD_TEMPLATE _field("Samples", &SoundStream::Chunk::Samples);
        _nd.CPGF_MD_TEMPLATE _field("NbSamples", &SoundStream::Chunk::NbSamples);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
