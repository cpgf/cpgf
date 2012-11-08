// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_EDRIVERTYPES_H
#define __META_IRRLICHT_EDRIVERTYPES_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_edrivertypes(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<E_DRIVER_TYPE>("E_DRIVER_TYPE")
        ._element("EDT_NULL", irr::video::EDT_NULL)
        ._element("EDT_SOFTWARE", irr::video::EDT_SOFTWARE)
        ._element("EDT_BURNINGSVIDEO", irr::video::EDT_BURNINGSVIDEO)
        ._element("EDT_DIRECT3D8", irr::video::EDT_DIRECT3D8)
        ._element("EDT_DIRECT3D9", irr::video::EDT_DIRECT3D9)
        ._element("EDT_OPENGL", irr::video::EDT_OPENGL)
        ._element("EDT_COUNT", irr::video::EDT_COUNT)
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
