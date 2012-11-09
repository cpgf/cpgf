// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_EHARDWAREBUFFERFLAGS_H
#define __META_IRRLICHT_EHARDWAREBUFFERFLAGS_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_ehardwarebufferflags(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<E_HARDWARE_MAPPING>("E_HARDWARE_MAPPING")
        ._element("EHM_NEVER", irr::scene::EHM_NEVER)
        ._element("EHM_STATIC", irr::scene::EHM_STATIC)
        ._element("EHM_DYNAMIC", irr::scene::EHM_DYNAMIC)
        ._element("EHM_STREAM", irr::scene::EHM_STREAM)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_BUFFER_TYPE>("E_BUFFER_TYPE")
        ._element("EBT_NONE", irr::scene::EBT_NONE)
        ._element("EBT_VERTEX", irr::scene::EBT_VERTEX)
        ._element("EBT_INDEX", irr::scene::EBT_INDEX)
        ._element("EBT_VERTEX_AND_INDEX", irr::scene::EBT_VERTEX_AND_INDEX)
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
