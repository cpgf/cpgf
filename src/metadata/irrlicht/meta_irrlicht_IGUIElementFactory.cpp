// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IGUIElementFactory.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IGUIElementFactory.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IGUIElementFactory()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::gui::IGUIElementFactory, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::gui::IGUIElementFactory, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IGUIElementFactory");
        buildMetaClass_IGUIElementFactory(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


