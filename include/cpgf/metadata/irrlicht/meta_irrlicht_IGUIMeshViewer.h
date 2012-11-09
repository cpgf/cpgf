// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IGUIMESHVIEWER_H
#define __META_IRRLICHT_IGUIMESHVIEWER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;
using namespace irr::scene;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IGUIMeshViewer(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("setMesh", &D::ClassType::setMesh);
    _d.CPGF_MD_TEMPLATE _method("getMesh", &D::ClassType::getMesh);
    _d.CPGF_MD_TEMPLATE _method("setMaterial", &D::ClassType::setMaterial);
    _d.CPGF_MD_TEMPLATE _method("getMaterial", &D::ClassType::getMaterial);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
