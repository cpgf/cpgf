// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IGUITreeView.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IGUITreeView.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IGUITreeView()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::gui::IGUITreeView, irr::gui::IGUIElement> _nd = GDefineMetaClass<irr::gui::IGUITreeView, irr::gui::IGUIElement>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IGUITreeView");
        buildMetaClass_IGUITreeView(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IGUITreeViewNode()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::gui::IGUITreeViewNode, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::gui::IGUITreeViewNode, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IGUITreeViewNode");
        buildMetaClass_IGUITreeViewNode(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


