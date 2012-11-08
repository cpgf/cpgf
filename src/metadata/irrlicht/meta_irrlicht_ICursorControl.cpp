// Auto generated file, don't modify.

#include "irrlicht.h"
#include "ICursorControl.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_ICursorControl.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_ICursorControl()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::gui::ICursorControl, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::gui::ICursorControl, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ICursorControl");
        buildMetaClass_ICursorControl(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


