// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IGUICONTEXTMENU_H
#define __META_IRRLICHT_IGUICONTEXTMENU_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_iguicontextmenu(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<ECONTEXT_MENU_CLOSE>("ECONTEXT_MENU_CLOSE")
        ._element("ECMC_IGNORE", irr::gui::ECMC_IGNORE)
        ._element("ECMC_REMOVE", irr::gui::ECMC_REMOVE)
        ._element("ECMC_HIDE", irr::gui::ECMC_HIDE)
    ;
}


template <typename D>
void buildMetaClass_IGUIContextMenu(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("setCloseHandling", &D::ClassType::setCloseHandling);
    _d.CPGF_MD_TEMPLATE _method("getCloseHandling", &D::ClassType::getCloseHandling);
    _d.CPGF_MD_TEMPLATE _method("getItemCount", &D::ClassType::getItemCount);
    _d.CPGF_MD_TEMPLATE _method("addItem", &D::ClassType::addItem)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(-1))
    ;
    _d.CPGF_MD_TEMPLATE _method("insertItem", &D::ClassType::insertItem)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(-1))
    ;
    _d.CPGF_MD_TEMPLATE _method("findItemWithCommandId", &D::ClassType::findItemWithCommandId)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addSeparator", &D::ClassType::addSeparator);
    _d.CPGF_MD_TEMPLATE _method("getItemText", &D::ClassType::getItemText);
    _d.CPGF_MD_TEMPLATE _method("setItemText", &D::ClassType::setItemText);
    _d.CPGF_MD_TEMPLATE _method("isItemEnabled", &D::ClassType::isItemEnabled);
    _d.CPGF_MD_TEMPLATE _method("setItemEnabled", &D::ClassType::setItemEnabled);
    _d.CPGF_MD_TEMPLATE _method("setItemChecked", &D::ClassType::setItemChecked);
    _d.CPGF_MD_TEMPLATE _method("isItemChecked", &D::ClassType::isItemChecked);
    _d.CPGF_MD_TEMPLATE _method("removeItem", &D::ClassType::removeItem);
    _d.CPGF_MD_TEMPLATE _method("removeAllItems", &D::ClassType::removeAllItems);
    _d.CPGF_MD_TEMPLATE _method("getSelectedItem", &D::ClassType::getSelectedItem);
    _d.CPGF_MD_TEMPLATE _method("getItemCommandId", &D::ClassType::getItemCommandId);
    _d.CPGF_MD_TEMPLATE _method("setItemCommandId", &D::ClassType::setItemCommandId);
    _d.CPGF_MD_TEMPLATE _method("getSubMenu", &D::ClassType::getSubMenu);
    _d.CPGF_MD_TEMPLATE _method("setItemAutoChecking", &D::ClassType::setItemAutoChecking);
    _d.CPGF_MD_TEMPLATE _method("getItemAutoChecking", &D::ClassType::getItemAutoChecking);
    _d.CPGF_MD_TEMPLATE _method("setEventParent", &D::ClassType::setEventParent);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
