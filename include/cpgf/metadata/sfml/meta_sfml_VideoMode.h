// Auto generated file, don't modify.

#ifndef __META_SFML_VIDEOMODE_H
#define __META_SFML_VIDEOMODE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindapi.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_VideoMode(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (unsigned int, unsigned int, unsigned int)>()
        ._default(copyVariantFromCopyable(32))
    ;
    _d.CPGF_MD_TEMPLATE _field(replaceName("Width", _r), &D::ClassType::Width);
    _d.CPGF_MD_TEMPLATE _field(replaceName("Height", _r), &D::ClassType::Height);
    _d.CPGF_MD_TEMPLATE _field(replaceName("BitsPerPixel", _r), &D::ClassType::BitsPerPixel);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsValid", _r), &D::ClassType::IsValid);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetDesktopMode", _r), &D::ClassType::GetDesktopMode);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetMode", _r), &D::ClassType::GetMode);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetModesCount", _r), &D::ClassType::GetModesCount);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const VideoMode &)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const VideoMode &)>(mopHolder != mopHolder);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
