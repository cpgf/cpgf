// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IGUIELEMENTFACTORY_H
#define __META_IRRLICHT_IGUIELEMENTFACTORY_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IGUIElementFactory(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("addGUIElement", (IGUIElement * (D::ClassType::*) (EGUI_ELEMENT_TYPE, IGUIElement *))&D::ClassType::addGUIElement)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addGUIElement", (IGUIElement * (D::ClassType::*) (const c8 *, IGUIElement *))&D::ClassType::addGUIElement)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getCreatableGUIElementTypeCount", &D::ClassType::getCreatableGUIElementTypeCount);
    _d.CPGF_MD_TEMPLATE _method("getCreateableGUIElementType", &D::ClassType::getCreateableGUIElementType);
    _d.CPGF_MD_TEMPLATE _method("getCreateableGUIElementTypeName", (const c8 * (D::ClassType::*) (s32) const)&D::ClassType::getCreateableGUIElementTypeName);
    _d.CPGF_MD_TEMPLATE _method("getCreateableGUIElementTypeName", (const c8 * (D::ClassType::*) (EGUI_ELEMENT_TYPE) const)&D::ClassType::getCreateableGUIElementTypeName);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
