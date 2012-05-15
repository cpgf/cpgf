// Auto generated file, don't modify.

#ifndef __META_SFML_WINDOWSETTINGS_H
#define __META_SFML_WINDOWSETTINGS_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_WindowSettings(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (unsigned int, unsigned int, unsigned int)>()
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(8))
        ._default(copyVariantFromCopyable(24))
    ;
    _d.CPGF_MD_TEMPLATE _field(replaceName("DepthBits", _r), &D::ClassType::DepthBits);
    _d.CPGF_MD_TEMPLATE _field(replaceName("StencilBits", _r), &D::ClassType::StencilBits);
    _d.CPGF_MD_TEMPLATE _field(replaceName("AntialiasingLevel", _r), &D::ClassType::AntialiasingLevel);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
