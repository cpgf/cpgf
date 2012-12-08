// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IGUISKIN_H
#define __META_IRRLICHT_IGUISKIN_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_iguiskin(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("GUISkinTypeNames", &GUISkinTypeNames);
    _d.CPGF_MD_TEMPLATE _field("GUISkinColorNames", &GUISkinColorNames);
    _d.CPGF_MD_TEMPLATE _field("GUISkinSizeNames", &GUISkinSizeNames);
    _d.CPGF_MD_TEMPLATE _field("GUISkinTextNames", &GUISkinTextNames);
    _d.CPGF_MD_TEMPLATE _field("GUISkinIconNames", &GUISkinIconNames);
    _d.CPGF_MD_TEMPLATE _field("GUISkinFontNames", &GUISkinFontNames);
    _d.CPGF_MD_TEMPLATE _enum<EGUI_SKIN_TYPE>("EGUI_SKIN_TYPE")
        ._element("EGST_WINDOWS_CLASSIC", irr::gui::EGST_WINDOWS_CLASSIC)
        ._element("EGST_WINDOWS_METALLIC", irr::gui::EGST_WINDOWS_METALLIC)
        ._element("EGST_BURNING_SKIN", irr::gui::EGST_BURNING_SKIN)
        ._element("EGST_UNKNOWN", irr::gui::EGST_UNKNOWN)
        ._element("EGST_COUNT", irr::gui::EGST_COUNT)
    ;
    _d.CPGF_MD_TEMPLATE _enum<EGUI_DEFAULT_COLOR>("EGUI_DEFAULT_COLOR")
        ._element("EGDC_3D_DARK_SHADOW", irr::gui::EGDC_3D_DARK_SHADOW)
        ._element("EGDC_3D_SHADOW", irr::gui::EGDC_3D_SHADOW)
        ._element("EGDC_3D_FACE", irr::gui::EGDC_3D_FACE)
        ._element("EGDC_3D_HIGH_LIGHT", irr::gui::EGDC_3D_HIGH_LIGHT)
        ._element("EGDC_3D_LIGHT", irr::gui::EGDC_3D_LIGHT)
        ._element("EGDC_ACTIVE_BORDER", irr::gui::EGDC_ACTIVE_BORDER)
        ._element("EGDC_ACTIVE_CAPTION", irr::gui::EGDC_ACTIVE_CAPTION)
        ._element("EGDC_APP_WORKSPACE", irr::gui::EGDC_APP_WORKSPACE)
        ._element("EGDC_BUTTON_TEXT", irr::gui::EGDC_BUTTON_TEXT)
        ._element("EGDC_GRAY_TEXT", irr::gui::EGDC_GRAY_TEXT)
        ._element("EGDC_HIGH_LIGHT", irr::gui::EGDC_HIGH_LIGHT)
        ._element("EGDC_HIGH_LIGHT_TEXT", irr::gui::EGDC_HIGH_LIGHT_TEXT)
        ._element("EGDC_INACTIVE_BORDER", irr::gui::EGDC_INACTIVE_BORDER)
        ._element("EGDC_INACTIVE_CAPTION", irr::gui::EGDC_INACTIVE_CAPTION)
        ._element("EGDC_TOOLTIP", irr::gui::EGDC_TOOLTIP)
        ._element("EGDC_TOOLTIP_BACKGROUND", irr::gui::EGDC_TOOLTIP_BACKGROUND)
        ._element("EGDC_SCROLLBAR", irr::gui::EGDC_SCROLLBAR)
        ._element("EGDC_WINDOW", irr::gui::EGDC_WINDOW)
        ._element("EGDC_WINDOW_SYMBOL", irr::gui::EGDC_WINDOW_SYMBOL)
        ._element("EGDC_ICON", irr::gui::EGDC_ICON)
        ._element("EGDC_ICON_HIGH_LIGHT", irr::gui::EGDC_ICON_HIGH_LIGHT)
        ._element("EGDC_GRAY_WINDOW_SYMBOL", irr::gui::EGDC_GRAY_WINDOW_SYMBOL)
        ._element("EGDC_EDITABLE", irr::gui::EGDC_EDITABLE)
        ._element("EGDC_GRAY_EDITABLE", irr::gui::EGDC_GRAY_EDITABLE)
        ._element("EGDC_FOCUSED_EDITABLE", irr::gui::EGDC_FOCUSED_EDITABLE)
        ._element("EGDC_COUNT", irr::gui::EGDC_COUNT)
    ;
    _d.CPGF_MD_TEMPLATE _enum<EGUI_DEFAULT_SIZE>("EGUI_DEFAULT_SIZE")
        ._element("EGDS_SCROLLBAR_SIZE", irr::gui::EGDS_SCROLLBAR_SIZE)
        ._element("EGDS_MENU_HEIGHT", irr::gui::EGDS_MENU_HEIGHT)
        ._element("EGDS_WINDOW_BUTTON_WIDTH", irr::gui::EGDS_WINDOW_BUTTON_WIDTH)
        ._element("EGDS_CHECK_BOX_WIDTH", irr::gui::EGDS_CHECK_BOX_WIDTH)
        ._element("EGDS_MESSAGE_BOX_WIDTH", irr::gui::EGDS_MESSAGE_BOX_WIDTH)
        ._element("EGDS_MESSAGE_BOX_HEIGHT", irr::gui::EGDS_MESSAGE_BOX_HEIGHT)
        ._element("EGDS_BUTTON_WIDTH", irr::gui::EGDS_BUTTON_WIDTH)
        ._element("EGDS_BUTTON_HEIGHT", irr::gui::EGDS_BUTTON_HEIGHT)
        ._element("EGDS_TEXT_DISTANCE_X", irr::gui::EGDS_TEXT_DISTANCE_X)
        ._element("EGDS_TEXT_DISTANCE_Y", irr::gui::EGDS_TEXT_DISTANCE_Y)
        ._element("EGDS_TITLEBARTEXT_DISTANCE_X", irr::gui::EGDS_TITLEBARTEXT_DISTANCE_X)
        ._element("EGDS_TITLEBARTEXT_DISTANCE_Y", irr::gui::EGDS_TITLEBARTEXT_DISTANCE_Y)
        ._element("EGDS_MESSAGE_BOX_GAP_SPACE", irr::gui::EGDS_MESSAGE_BOX_GAP_SPACE)
        ._element("EGDS_MESSAGE_BOX_MIN_TEXT_WIDTH", irr::gui::EGDS_MESSAGE_BOX_MIN_TEXT_WIDTH)
        ._element("EGDS_MESSAGE_BOX_MAX_TEXT_WIDTH", irr::gui::EGDS_MESSAGE_BOX_MAX_TEXT_WIDTH)
        ._element("EGDS_MESSAGE_BOX_MIN_TEXT_HEIGHT", irr::gui::EGDS_MESSAGE_BOX_MIN_TEXT_HEIGHT)
        ._element("EGDS_MESSAGE_BOX_MAX_TEXT_HEIGHT", irr::gui::EGDS_MESSAGE_BOX_MAX_TEXT_HEIGHT)
        ._element("EGDS_BUTTON_PRESSED_IMAGE_OFFSET_X", irr::gui::EGDS_BUTTON_PRESSED_IMAGE_OFFSET_X)
        ._element("EGDS_BUTTON_PRESSED_IMAGE_OFFSET_Y", irr::gui::EGDS_BUTTON_PRESSED_IMAGE_OFFSET_Y)
        ._element("EGDS_BUTTON_PRESSED_TEXT_OFFSET_X", irr::gui::EGDS_BUTTON_PRESSED_TEXT_OFFSET_X)
        ._element("EGDS_BUTTON_PRESSED_TEXT_OFFSET_Y", irr::gui::EGDS_BUTTON_PRESSED_TEXT_OFFSET_Y)
        ._element("EGDS_COUNT", irr::gui::EGDS_COUNT)
    ;
    _d.CPGF_MD_TEMPLATE _enum<EGUI_DEFAULT_TEXT>("EGUI_DEFAULT_TEXT")
        ._element("EGDT_MSG_BOX_OK", irr::gui::EGDT_MSG_BOX_OK)
        ._element("EGDT_MSG_BOX_CANCEL", irr::gui::EGDT_MSG_BOX_CANCEL)
        ._element("EGDT_MSG_BOX_YES", irr::gui::EGDT_MSG_BOX_YES)
        ._element("EGDT_MSG_BOX_NO", irr::gui::EGDT_MSG_BOX_NO)
        ._element("EGDT_WINDOW_CLOSE", irr::gui::EGDT_WINDOW_CLOSE)
        ._element("EGDT_WINDOW_MAXIMIZE", irr::gui::EGDT_WINDOW_MAXIMIZE)
        ._element("EGDT_WINDOW_MINIMIZE", irr::gui::EGDT_WINDOW_MINIMIZE)
        ._element("EGDT_WINDOW_RESTORE", irr::gui::EGDT_WINDOW_RESTORE)
        ._element("EGDT_COUNT", irr::gui::EGDT_COUNT)
    ;
    _d.CPGF_MD_TEMPLATE _enum<EGUI_DEFAULT_ICON>("EGUI_DEFAULT_ICON")
        ._element("EGDI_WINDOW_MAXIMIZE", irr::gui::EGDI_WINDOW_MAXIMIZE)
        ._element("EGDI_WINDOW_RESTORE", irr::gui::EGDI_WINDOW_RESTORE)
        ._element("EGDI_WINDOW_CLOSE", irr::gui::EGDI_WINDOW_CLOSE)
        ._element("EGDI_WINDOW_MINIMIZE", irr::gui::EGDI_WINDOW_MINIMIZE)
        ._element("EGDI_WINDOW_RESIZE", irr::gui::EGDI_WINDOW_RESIZE)
        ._element("EGDI_CURSOR_UP", irr::gui::EGDI_CURSOR_UP)
        ._element("EGDI_CURSOR_DOWN", irr::gui::EGDI_CURSOR_DOWN)
        ._element("EGDI_CURSOR_LEFT", irr::gui::EGDI_CURSOR_LEFT)
        ._element("EGDI_CURSOR_RIGHT", irr::gui::EGDI_CURSOR_RIGHT)
        ._element("EGDI_MENU_MORE", irr::gui::EGDI_MENU_MORE)
        ._element("EGDI_CHECK_BOX_CHECKED", irr::gui::EGDI_CHECK_BOX_CHECKED)
        ._element("EGDI_DROP_DOWN", irr::gui::EGDI_DROP_DOWN)
        ._element("EGDI_SMALL_CURSOR_UP", irr::gui::EGDI_SMALL_CURSOR_UP)
        ._element("EGDI_SMALL_CURSOR_DOWN", irr::gui::EGDI_SMALL_CURSOR_DOWN)
        ._element("EGDI_RADIO_BUTTON_CHECKED", irr::gui::EGDI_RADIO_BUTTON_CHECKED)
        ._element("EGDI_MORE_LEFT", irr::gui::EGDI_MORE_LEFT)
        ._element("EGDI_MORE_RIGHT", irr::gui::EGDI_MORE_RIGHT)
        ._element("EGDI_MORE_UP", irr::gui::EGDI_MORE_UP)
        ._element("EGDI_MORE_DOWN", irr::gui::EGDI_MORE_DOWN)
        ._element("EGDI_EXPAND", irr::gui::EGDI_EXPAND)
        ._element("EGDI_COLLAPSE", irr::gui::EGDI_COLLAPSE)
        ._element("EGDI_FILE", irr::gui::EGDI_FILE)
        ._element("EGDI_DIRECTORY", irr::gui::EGDI_DIRECTORY)
        ._element("EGDI_COUNT", irr::gui::EGDI_COUNT)
    ;
    _d.CPGF_MD_TEMPLATE _enum<EGUI_DEFAULT_FONT>("EGUI_DEFAULT_FONT")
        ._element("EGDF_DEFAULT", irr::gui::EGDF_DEFAULT)
        ._element("EGDF_BUTTON", irr::gui::EGDF_BUTTON)
        ._element("EGDF_WINDOW", irr::gui::EGDF_WINDOW)
        ._element("EGDF_MENU", irr::gui::EGDF_MENU)
        ._element("EGDF_TOOLTIP", irr::gui::EGDF_TOOLTIP)
        ._element("EGDF_COUNT", irr::gui::EGDF_COUNT)
    ;
}


template <typename D>
void buildMetaClass_IGUISkin(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getColor", &D::ClassType::getColor);
    _d.CPGF_MD_TEMPLATE _method("setColor", &D::ClassType::setColor);
    _d.CPGF_MD_TEMPLATE _method("getSize", &D::ClassType::getSize);
    _d.CPGF_MD_TEMPLATE _method("getDefaultText", &D::ClassType::getDefaultText);
    _d.CPGF_MD_TEMPLATE _method("setDefaultText", &D::ClassType::setDefaultText);
    _d.CPGF_MD_TEMPLATE _method("setSize", &D::ClassType::setSize);
    _d.CPGF_MD_TEMPLATE _method("getFont", &D::ClassType::getFont)
        ._default(copyVariantFromCopyable(EGDF_DEFAULT))
    ;
    _d.CPGF_MD_TEMPLATE _method("setFont", &D::ClassType::setFont)
        ._default(copyVariantFromCopyable(EGDF_DEFAULT))
    ;
    _d.CPGF_MD_TEMPLATE _method("getSpriteBank", &D::ClassType::getSpriteBank);
    _d.CPGF_MD_TEMPLATE _method("setSpriteBank", &D::ClassType::setSpriteBank);
    _d.CPGF_MD_TEMPLATE _method("getIcon", &D::ClassType::getIcon);
    _d.CPGF_MD_TEMPLATE _method("setIcon", &D::ClassType::setIcon);
    _d.CPGF_MD_TEMPLATE _method("draw3DButtonPaneStandard", &D::ClassType::draw3DButtonPaneStandard)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw3DButtonPanePressed", &D::ClassType::draw3DButtonPanePressed)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw3DSunkenPane", &D::ClassType::draw3DSunkenPane)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw3DWindowBackground", &D::ClassType::draw3DWindowBackground)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw3DMenuPane", &D::ClassType::draw3DMenuPane)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw3DToolBar", &D::ClassType::draw3DToolBar)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw3DTabButton", &D::ClassType::draw3DTabButton)
        ._default(copyVariantFromCopyable(EGUIA_UPPERLEFT))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw3DTabBody", &D::ClassType::draw3DTabBody)
        ._default(copyVariantFromCopyable(EGUIA_UPPERLEFT))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("drawIcon", &D::ClassType::drawIcon)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw2DRectangle", &D::ClassType::draw2DRectangle)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getType", &D::ClassType::getType);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
