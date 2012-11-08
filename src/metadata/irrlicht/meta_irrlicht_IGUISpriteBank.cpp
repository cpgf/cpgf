// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IGUISpriteBank.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IGUISpriteBank.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IGUISpriteBank()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::gui::IGUISpriteBank, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::gui::IGUISpriteBank, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IGUISpriteBank");
        buildMetaClass_IGUISpriteBank(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SGUISprite()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::gui::SGUISprite> _nd = GDefineMetaClass<irr::gui::SGUISprite>::declare("SGUISprite");
        buildMetaClass_SGUISprite(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SGUISpriteFrame()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::gui::SGUISpriteFrame> _nd = GDefineMetaClass<irr::gui::SGUISpriteFrame>::declare("SGUISpriteFrame");
        buildMetaClass_SGUISpriteFrame(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


