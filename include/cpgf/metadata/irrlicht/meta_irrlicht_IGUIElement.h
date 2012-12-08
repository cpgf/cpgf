// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IGUIELEMENT_H
#define __META_IRRLICHT_IGUIELEMENT_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IGUIElement(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (EGUI_ELEMENT_TYPE, IGUIEnvironment *, IGUIElement *, s32, const core::rect< s32 > &)>();
    _d.CPGF_MD_TEMPLATE _method("getParent", &D::ClassType::getParent);
    _d.CPGF_MD_TEMPLATE _method("getRelativePosition", &D::ClassType::getRelativePosition);
    _d.CPGF_MD_TEMPLATE _method("setRelativePosition", (void (D::ClassType::*) (const core::rect< s32 > &))&D::ClassType::setRelativePosition);
    _d.CPGF_MD_TEMPLATE _method("setRelativePosition", (void (D::ClassType::*) (const core::position2di &))&D::ClassType::setRelativePosition);
    _d.CPGF_MD_TEMPLATE _method("setRelativePositionProportional", &D::ClassType::setRelativePositionProportional);
    _d.CPGF_MD_TEMPLATE _method("getAbsolutePosition", &D::ClassType::getAbsolutePosition);
    _d.CPGF_MD_TEMPLATE _method("getAbsoluteClippingRect", &D::ClassType::getAbsoluteClippingRect);
    _d.CPGF_MD_TEMPLATE _method("setNotClipped", &D::ClassType::setNotClipped);
    _d.CPGF_MD_TEMPLATE _method("isNotClipped", &D::ClassType::isNotClipped);
    _d.CPGF_MD_TEMPLATE _method("setMaxSize", &D::ClassType::setMaxSize);
    _d.CPGF_MD_TEMPLATE _method("setMinSize", &D::ClassType::setMinSize);
    _d.CPGF_MD_TEMPLATE _method("setAlignment", &D::ClassType::setAlignment);
    _d.CPGF_MD_TEMPLATE _method("updateAbsolutePosition", &D::ClassType::updateAbsolutePosition);
    _d.CPGF_MD_TEMPLATE _method("getElementFromPoint", &D::ClassType::getElementFromPoint);
    _d.CPGF_MD_TEMPLATE _method("isPointInside", &D::ClassType::isPointInside);
    _d.CPGF_MD_TEMPLATE _method("addChild", &D::ClassType::addChild);
    _d.CPGF_MD_TEMPLATE _method("removeChild", &D::ClassType::removeChild);
    _d.CPGF_MD_TEMPLATE _method("remove", &D::ClassType::remove);
    _d.CPGF_MD_TEMPLATE _method("draw", &D::ClassType::draw);
    _d.CPGF_MD_TEMPLATE _method("OnPostRender", &D::ClassType::OnPostRender);
    _d.CPGF_MD_TEMPLATE _method("move", &D::ClassType::move);
    _d.CPGF_MD_TEMPLATE _method("isVisible", &D::ClassType::isVisible);
    _d.CPGF_MD_TEMPLATE _method("setVisible", &D::ClassType::setVisible);
    _d.CPGF_MD_TEMPLATE _method("isSubElement", &D::ClassType::isSubElement);
    _d.CPGF_MD_TEMPLATE _method("setSubElement", &D::ClassType::setSubElement);
    _d.CPGF_MD_TEMPLATE _method("setTabStop", &D::ClassType::setTabStop);
    _d.CPGF_MD_TEMPLATE _method("isTabStop", &D::ClassType::isTabStop);
    _d.CPGF_MD_TEMPLATE _method("setTabOrder", &D::ClassType::setTabOrder);
    _d.CPGF_MD_TEMPLATE _method("getTabOrder", &D::ClassType::getTabOrder);
    _d.CPGF_MD_TEMPLATE _method("setTabGroup", &D::ClassType::setTabGroup);
    _d.CPGF_MD_TEMPLATE _method("isTabGroup", &D::ClassType::isTabGroup);
    _d.CPGF_MD_TEMPLATE _method("getTabGroup", &D::ClassType::getTabGroup);
    _d.CPGF_MD_TEMPLATE _method("isEnabled", &D::ClassType::isEnabled);
    _d.CPGF_MD_TEMPLATE _method("setEnabled", &D::ClassType::setEnabled);
    _d.CPGF_MD_TEMPLATE _method("setText", &D::ClassType::setText);
    _d.CPGF_MD_TEMPLATE _method("getText", &D::ClassType::getText);
    _d.CPGF_MD_TEMPLATE _method("setToolTipText", &D::ClassType::setToolTipText);
    _d.CPGF_MD_TEMPLATE _method("getToolTipText", &D::ClassType::getToolTipText, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
    _d.CPGF_MD_TEMPLATE _method("getID", &D::ClassType::getID);
    _d.CPGF_MD_TEMPLATE _method("setID", &D::ClassType::setID);
    _d.CPGF_MD_TEMPLATE _method("OnEvent", &D::ClassType::OnEvent);
    _d.CPGF_MD_TEMPLATE _method("bringToFront", &D::ClassType::bringToFront);
    _d.CPGF_MD_TEMPLATE _method("sendToBack", &D::ClassType::sendToBack);
    _d.CPGF_MD_TEMPLATE _method("getChildren", &D::ClassType::getChildren);
    _d.CPGF_MD_TEMPLATE _method("getElementFromId", &D::ClassType::getElementFromId)
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("isMyChild", &D::ClassType::isMyChild);
    _d.CPGF_MD_TEMPLATE _method("getNextElement", &D::ClassType::getNextElement)
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("getType", &D::ClassType::getType);
    _d.CPGF_MD_TEMPLATE _method("hasType", &D::ClassType::hasType);
    _d.CPGF_MD_TEMPLATE _method("getTypeName", &D::ClassType::getTypeName);
    _d.CPGF_MD_TEMPLATE _method("getName", &D::ClassType::getName);
    _d.CPGF_MD_TEMPLATE _method("setName", (void (D::ClassType::*) (const c8 *))&D::ClassType::setName);
    _d.CPGF_MD_TEMPLATE _method("setName", (void (D::ClassType::*) (const core::stringc &))&D::ClassType::setName, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("serializeAttributes", &D::ClassType::serializeAttributes)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("deserializeAttributes", &D::ClassType::deserializeAttributes)
        ._default(copyVariantFromCopyable(0))
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
