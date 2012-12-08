// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_EGUIELEMENTTYPES_H
#define __META_IRRLICHT_EGUIELEMENTTYPES_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_eguielementtypes(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("GUIElementTypeNames", &GUIElementTypeNames);
    _d.CPGF_MD_TEMPLATE _enum<EGUI_ELEMENT_TYPE>("EGUI_ELEMENT_TYPE")
        ._element("EGUIET_BUTTON", irr::gui::EGUIET_BUTTON)
        ._element("EGUIET_CHECK_BOX", irr::gui::EGUIET_CHECK_BOX)
        ._element("EGUIET_COMBO_BOX", irr::gui::EGUIET_COMBO_BOX)
        ._element("EGUIET_CONTEXT_MENU", irr::gui::EGUIET_CONTEXT_MENU)
        ._element("EGUIET_MENU", irr::gui::EGUIET_MENU)
        ._element("EGUIET_EDIT_BOX", irr::gui::EGUIET_EDIT_BOX)
        ._element("EGUIET_FILE_OPEN_DIALOG", irr::gui::EGUIET_FILE_OPEN_DIALOG)
        ._element("EGUIET_COLOR_SELECT_DIALOG", irr::gui::EGUIET_COLOR_SELECT_DIALOG)
        ._element("EGUIET_IN_OUT_FADER", irr::gui::EGUIET_IN_OUT_FADER)
        ._element("EGUIET_IMAGE", irr::gui::EGUIET_IMAGE)
        ._element("EGUIET_LIST_BOX", irr::gui::EGUIET_LIST_BOX)
        ._element("EGUIET_MESH_VIEWER", irr::gui::EGUIET_MESH_VIEWER)
        ._element("EGUIET_MESSAGE_BOX", irr::gui::EGUIET_MESSAGE_BOX)
        ._element("EGUIET_MODAL_SCREEN", irr::gui::EGUIET_MODAL_SCREEN)
        ._element("EGUIET_SCROLL_BAR", irr::gui::EGUIET_SCROLL_BAR)
        ._element("EGUIET_SPIN_BOX", irr::gui::EGUIET_SPIN_BOX)
        ._element("EGUIET_STATIC_TEXT", irr::gui::EGUIET_STATIC_TEXT)
        ._element("EGUIET_TAB", irr::gui::EGUIET_TAB)
        ._element("EGUIET_TAB_CONTROL", irr::gui::EGUIET_TAB_CONTROL)
        ._element("EGUIET_TABLE", irr::gui::EGUIET_TABLE)
        ._element("EGUIET_TOOL_BAR", irr::gui::EGUIET_TOOL_BAR)
        ._element("EGUIET_TREE_VIEW", irr::gui::EGUIET_TREE_VIEW)
        ._element("EGUIET_WINDOW", irr::gui::EGUIET_WINDOW)
        ._element("EGUIET_ELEMENT", irr::gui::EGUIET_ELEMENT)
        ._element("EGUIET_ROOT", irr::gui::EGUIET_ROOT)
        ._element("EGUIET_COUNT", irr::gui::EGUIET_COUNT)
        ._element("EGUIET_FORCE_32_BIT", irr::gui::EGUIET_FORCE_32_BIT)
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
