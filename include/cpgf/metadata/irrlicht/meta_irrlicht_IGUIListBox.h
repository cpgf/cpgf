// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IGUILISTBOX_H
#define __META_IRRLICHT_IGUILISTBOX_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_iguilistbox(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<EGUI_LISTBOX_COLOR>("EGUI_LISTBOX_COLOR")
        ._element("EGUI_LBC_TEXT", irr::gui::EGUI_LBC_TEXT)
        ._element("EGUI_LBC_TEXT_HIGHLIGHT", irr::gui::EGUI_LBC_TEXT_HIGHLIGHT)
        ._element("EGUI_LBC_ICON", irr::gui::EGUI_LBC_ICON)
        ._element("EGUI_LBC_ICON_HIGHLIGHT", irr::gui::EGUI_LBC_ICON_HIGHLIGHT)
        ._element("EGUI_LBC_COUNT", irr::gui::EGUI_LBC_COUNT)
    ;
}


template <typename D>
void buildMetaClass_IGUIListBox(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getItemCount", &D::ClassType::getItemCount);
    _d.CPGF_MD_TEMPLATE _method("getListItem", &D::ClassType::getListItem);
    _d.CPGF_MD_TEMPLATE _method("addItem", (u32 (D::ClassType::*) (const wchar_t *))&D::ClassType::addItem);
    _d.CPGF_MD_TEMPLATE _method("addItem", (u32 (D::ClassType::*) (const wchar_t *, s32))&D::ClassType::addItem);
    _d.CPGF_MD_TEMPLATE _method("removeItem", &D::ClassType::removeItem);
    _d.CPGF_MD_TEMPLATE _method("getItemAt", &D::ClassType::getItemAt);
    _d.CPGF_MD_TEMPLATE _method("getIcon", &D::ClassType::getIcon);
    _d.CPGF_MD_TEMPLATE _method("setSpriteBank", &D::ClassType::setSpriteBank);
    _d.CPGF_MD_TEMPLATE _method("clear", &D::ClassType::clear);
    _d.CPGF_MD_TEMPLATE _method("getSelected", &D::ClassType::getSelected);
    _d.CPGF_MD_TEMPLATE _method("setSelected", (void (D::ClassType::*) (s32))&D::ClassType::setSelected);
    _d.CPGF_MD_TEMPLATE _method("setSelected", (void (D::ClassType::*) (const wchar_t *))&D::ClassType::setSelected);
    _d.CPGF_MD_TEMPLATE _method("setAutoScrollEnabled", &D::ClassType::setAutoScrollEnabled);
    _d.CPGF_MD_TEMPLATE _method("isAutoScrollEnabled", &D::ClassType::isAutoScrollEnabled);
    _d.CPGF_MD_TEMPLATE _method("setItemOverrideColor", (void (D::ClassType::*) (u32, video::SColor))&D::ClassType::setItemOverrideColor);
    _d.CPGF_MD_TEMPLATE _method("setItemOverrideColor", (void (D::ClassType::*) (u32, EGUI_LISTBOX_COLOR, video::SColor))&D::ClassType::setItemOverrideColor);
    _d.CPGF_MD_TEMPLATE _method("clearItemOverrideColor", (void (D::ClassType::*) (u32))&D::ClassType::clearItemOverrideColor);
    _d.CPGF_MD_TEMPLATE _method("clearItemOverrideColor", (void (D::ClassType::*) (u32, EGUI_LISTBOX_COLOR))&D::ClassType::clearItemOverrideColor);
    _d.CPGF_MD_TEMPLATE _method("hasItemOverrideColor", &D::ClassType::hasItemOverrideColor);
    _d.CPGF_MD_TEMPLATE _method("getItemOverrideColor", &D::ClassType::getItemOverrideColor);
    _d.CPGF_MD_TEMPLATE _method("getItemDefaultColor", &D::ClassType::getItemDefaultColor);
    _d.CPGF_MD_TEMPLATE _method("setItem", &D::ClassType::setItem);
    _d.CPGF_MD_TEMPLATE _method("insertItem", &D::ClassType::insertItem);
    _d.CPGF_MD_TEMPLATE _method("swapItems", &D::ClassType::swapItems);
    _d.CPGF_MD_TEMPLATE _method("setItemHeight", &D::ClassType::setItemHeight);
    _d.CPGF_MD_TEMPLATE _method("setDrawBackground", &D::ClassType::setDrawBackground);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
