// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IGUIENVIRONMENT_H
#define __META_IRRLICHT_IGUIENVIRONMENT_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::io;
using namespace irr::video;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IGUIEnvironment(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("drawAll", &D::ClassType::drawAll);
    _d.CPGF_MD_TEMPLATE _method("setFocus", &D::ClassType::setFocus);
    _d.CPGF_MD_TEMPLATE _method("getFocus", &D::ClassType::getFocus);
    _d.CPGF_MD_TEMPLATE _method("getHovered", &D::ClassType::getHovered);
    _d.CPGF_MD_TEMPLATE _method("removeFocus", &D::ClassType::removeFocus);
    _d.CPGF_MD_TEMPLATE _method("hasFocus", &D::ClassType::hasFocus);
    _d.CPGF_MD_TEMPLATE _method("getVideoDriver", &D::ClassType::getVideoDriver);
    _d.CPGF_MD_TEMPLATE _method("getFileSystem", &D::ClassType::getFileSystem);
    _d.CPGF_MD_TEMPLATE _method("getOSOperator", &D::ClassType::getOSOperator);
    _d.CPGF_MD_TEMPLATE _method("clear", &D::ClassType::clear);
    _d.CPGF_MD_TEMPLATE _method("postEventFromUser", &D::ClassType::postEventFromUser);
    _d.CPGF_MD_TEMPLATE _method("setUserEventReceiver", &D::ClassType::setUserEventReceiver);
    _d.CPGF_MD_TEMPLATE _method("getSkin", &D::ClassType::getSkin);
    _d.CPGF_MD_TEMPLATE _method("setSkin", &D::ClassType::setSkin);
    _d.CPGF_MD_TEMPLATE _method("createSkin", &D::ClassType::createSkin);
    _d.CPGF_MD_TEMPLATE _method("createImageList", &D::ClassType::createImageList);
    _d.CPGF_MD_TEMPLATE _method("getFont", &D::ClassType::getFont, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("addFont", &D::ClassType::addFont, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("removeFont", &D::ClassType::removeFont);
    _d.CPGF_MD_TEMPLATE _method("getBuiltInFont", &D::ClassType::getBuiltInFont);
    _d.CPGF_MD_TEMPLATE _method("getSpriteBank", &D::ClassType::getSpriteBank, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("addEmptySpriteBank", &D::ClassType::addEmptySpriteBank, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getRootGUIElement", &D::ClassType::getRootGUIElement);
    _d.CPGF_MD_TEMPLATE _method("addButton", &D::ClassType::addButton)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addWindow", &D::ClassType::addWindow)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("addModalScreen", &D::ClassType::addModalScreen);
    _d.CPGF_MD_TEMPLATE _method("addMessageBox", &D::ClassType::addMessageBox)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(EMBF_OK))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addScrollBar", &D::ClassType::addScrollBar)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addImage", (IGUIImage * (D::ClassType::*) (video::ITexture *, core::position2d< s32 >, bool, IGUIElement *, s32, const wchar_t *))&D::ClassType::addImage)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("addImage", (IGUIImage * (D::ClassType::*) (const core::rect< s32 > &, IGUIElement *, s32, const wchar_t *, bool))&D::ClassType::addImage)
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addCheckBox", &D::ClassType::addCheckBox)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addListBox", &D::ClassType::addListBox)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addTreeView", &D::ClassType::addTreeView)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addMeshViewer", &D::ClassType::addMeshViewer)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addFileOpenDialog", &D::ClassType::addFileOpenDialog)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addColorSelectDialog", &D::ClassType::addColorSelectDialog)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addStaticText", &D::ClassType::addStaticText)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("addEditBox", &D::ClassType::addEditBox)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("addSpinBox", &D::ClassType::addSpinBox)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("addInOutFader", &D::ClassType::addInOutFader)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addTabControl", &D::ClassType::addTabControl)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addTab", &D::ClassType::addTab)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addContextMenu", &D::ClassType::addContextMenu)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addMenu", &D::ClassType::addMenu)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addToolBar", &D::ClassType::addToolBar)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addComboBox", &D::ClassType::addComboBox)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addTable", &D::ClassType::addTable)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getDefaultGUIElementFactory", &D::ClassType::getDefaultGUIElementFactory);
    _d.CPGF_MD_TEMPLATE _method("registerGUIElementFactory", &D::ClassType::registerGUIElementFactory);
    _d.CPGF_MD_TEMPLATE _method("getRegisteredGUIElementFactoryCount", &D::ClassType::getRegisteredGUIElementFactoryCount);
    _d.CPGF_MD_TEMPLATE _method("getGUIElementFactory", &D::ClassType::getGUIElementFactory);
    _d.CPGF_MD_TEMPLATE _method("addGUIElement", &D::ClassType::addGUIElement)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("saveGUI", (bool (D::ClassType::*) (const io::path &, IGUIElement *))&D::ClassType::saveGUI, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("saveGUI", (bool (D::ClassType::*) (io::IWriteFile *, IGUIElement *))&D::ClassType::saveGUI)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("loadGUI", (bool (D::ClassType::*) (const io::path &, IGUIElement *))&D::ClassType::loadGUI, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("loadGUI", (bool (D::ClassType::*) (io::IReadFile *, IGUIElement *))&D::ClassType::loadGUI)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("serializeAttributes", &D::ClassType::serializeAttributes)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("deserializeAttributes", &D::ClassType::deserializeAttributes)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("writeGUIElement", &D::ClassType::writeGUIElement);
    _d.CPGF_MD_TEMPLATE _method("readGUIElement", &D::ClassType::readGUIElement);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
