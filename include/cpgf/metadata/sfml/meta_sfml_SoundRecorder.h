// Auto generated file, don't modify.

#ifndef __META_SFML_SOUNDRECORDER_H
#define __META_SFML_SOUNDRECORDER_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindapi.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_SoundRecorder(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("Start", _r), &D::ClassType::Start)
        ._default(copyVariantFromCopyable(44100))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("Stop", _r), &D::ClassType::Stop);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetSampleRate", _r), &D::ClassType::GetSampleRate);
    _d.CPGF_MD_TEMPLATE _method(replaceName("CanCapture", _r), &D::ClassType::CanCapture);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
