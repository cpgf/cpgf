// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IGUIListBox.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IGUIListBox.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_iguilistbox()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_iguilistbox(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IGUIListBox()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::gui::IGUIListBox, irr::gui::IGUIElement> _nd = GDefineMetaClass<irr::gui::IGUIListBox, irr::gui::IGUIElement>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IGUIListBox");
        buildMetaClass_IGUIListBox(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


