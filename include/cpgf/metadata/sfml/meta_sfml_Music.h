// Auto generated file, don't modify.

#ifndef __META_SFML_MUSIC_H
#define __META_SFML_MUSIC_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;
using namespace sf::priv;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Music(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (std::size_t)>()
        ._default(copyVariantFromCopyable(44100))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("OpenFromFile", _r), &D::ClassType::OpenFromFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("OpenFromMemory", _r), &D::ClassType::OpenFromMemory);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetDuration", _r), &D::ClassType::GetDuration);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
