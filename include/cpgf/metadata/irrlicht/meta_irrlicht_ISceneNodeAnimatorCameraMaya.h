// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ISCENENODEANIMATORCAMERAMAYA_H
#define __META_IRRLICHT_ISCENENODEANIMATORCAMERAMAYA_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_ISceneNodeAnimatorCameraMaya(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getMoveSpeed", &D::ClassType::getMoveSpeed);
    _d.CPGF_MD_TEMPLATE _method("setMoveSpeed", &D::ClassType::setMoveSpeed);
    _d.CPGF_MD_TEMPLATE _method("getRotateSpeed", &D::ClassType::getRotateSpeed);
    _d.CPGF_MD_TEMPLATE _method("setRotateSpeed", &D::ClassType::setRotateSpeed);
    _d.CPGF_MD_TEMPLATE _method("getZoomSpeed", &D::ClassType::getZoomSpeed);
    _d.CPGF_MD_TEMPLATE _method("setZoomSpeed", &D::ClassType::setZoomSpeed);
    _d.CPGF_MD_TEMPLATE _method("getDistance", &D::ClassType::getDistance);
    _d.CPGF_MD_TEMPLATE _method("setDistance", &D::ClassType::setDistance);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
