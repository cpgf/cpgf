// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ETERRAINELEMENTS_H
#define __META_IRRLICHT_ETERRAINELEMENTS_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_eterrainelements(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<E_TERRAIN_PATCH_SIZE>("E_TERRAIN_PATCH_SIZE")
        ._element("ETPS_9", irr::scene::ETPS_9)
        ._element("ETPS_17", irr::scene::ETPS_17)
        ._element("ETPS_33", irr::scene::ETPS_33)
        ._element("ETPS_65", irr::scene::ETPS_65)
        ._element("ETPS_129", irr::scene::ETPS_129)
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
