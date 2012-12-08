// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IGUIIMAGE_H
#define __META_IRRLICHT_IGUIIMAGE_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IGUIImage(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("setImage", &D::ClassType::setImage);
    _d.CPGF_MD_TEMPLATE _method("getImage", &D::ClassType::getImage);
    _d.CPGF_MD_TEMPLATE _method("setColor", &D::ClassType::setColor);
    _d.CPGF_MD_TEMPLATE _method("setScaleImage", &D::ClassType::setScaleImage);
    _d.CPGF_MD_TEMPLATE _method("setUseAlphaChannel", &D::ClassType::setUseAlphaChannel);
    _d.CPGF_MD_TEMPLATE _method("getColor", &D::ClassType::getColor);
    _d.CPGF_MD_TEMPLATE _method("isImageScaled", &D::ClassType::isImageScaled);
    _d.CPGF_MD_TEMPLATE _method("isAlphaChannelUsed", &D::ClassType::isAlphaChannelUsed);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
