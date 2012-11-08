// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_EDEBUGSCENETYPES_H
#define __META_IRRLICHT_EDEBUGSCENETYPES_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_edebugscenetypes(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<E_DEBUG_SCENE_TYPE>("E_DEBUG_SCENE_TYPE")
        ._element("EDS_OFF", irr::scene::EDS_OFF)
        ._element("EDS_BBOX", irr::scene::EDS_BBOX)
        ._element("EDS_NORMALS", irr::scene::EDS_NORMALS)
        ._element("EDS_SKELETON", irr::scene::EDS_SKELETON)
        ._element("EDS_MESH_WIRE_OVERLAY", irr::scene::EDS_MESH_WIRE_OVERLAY)
        ._element("EDS_HALF_TRANSPARENCY", irr::scene::EDS_HALF_TRANSPARENCY)
        ._element("EDS_BBOX_BUFFERS", irr::scene::EDS_BBOX_BUFFERS)
        ._element("EDS_BBOX_ALL", irr::scene::EDS_BBOX_ALL)
        ._element("EDS_FULL", irr::scene::EDS_FULL)
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
