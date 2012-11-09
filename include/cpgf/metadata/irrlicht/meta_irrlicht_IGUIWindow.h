// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IGUIWINDOW_H
#define __META_IRRLICHT_IGUIWINDOW_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IGUIWindow(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getCloseButton", &D::ClassType::getCloseButton);
    _d.CPGF_MD_TEMPLATE _method("getMinimizeButton", &D::ClassType::getMinimizeButton);
    _d.CPGF_MD_TEMPLATE _method("getMaximizeButton", &D::ClassType::getMaximizeButton);
    _d.CPGF_MD_TEMPLATE _method("isDraggable", &D::ClassType::isDraggable);
    _d.CPGF_MD_TEMPLATE _method("setDraggable", &D::ClassType::setDraggable);
    _d.CPGF_MD_TEMPLATE _method("setDrawBackground", &D::ClassType::setDrawBackground);
    _d.CPGF_MD_TEMPLATE _method("getDrawBackground", &D::ClassType::getDrawBackground);
    _d.CPGF_MD_TEMPLATE _method("setDrawTitlebar", &D::ClassType::setDrawTitlebar);
    _d.CPGF_MD_TEMPLATE _method("getDrawTitlebar", &D::ClassType::getDrawTitlebar);
    _d.CPGF_MD_TEMPLATE _method("getClientRect", &D::ClassType::getClientRect);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
