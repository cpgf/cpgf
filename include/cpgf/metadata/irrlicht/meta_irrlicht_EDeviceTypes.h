// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_EDEVICETYPES_H
#define __META_IRRLICHT_EDEVICETYPES_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_edevicetypes(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<E_DEVICE_TYPE>("E_DEVICE_TYPE")
        ._element("EIDT_WIN32", irr::EIDT_WIN32)
        ._element("EIDT_WINCE", irr::EIDT_WINCE)
        ._element("EIDT_X11", irr::EIDT_X11)
        ._element("EIDT_OSX", irr::EIDT_OSX)
        ._element("EIDT_SDL", irr::EIDT_SDL)
        ._element("EIDT_FRAMEBUFFER", irr::EIDT_FRAMEBUFFER)
        ._element("EIDT_CONSOLE", irr::EIDT_CONSOLE)
        ._element("EIDT_BEST", irr::EIDT_BEST)
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
