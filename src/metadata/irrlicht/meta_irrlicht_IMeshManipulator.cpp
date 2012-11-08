// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IMeshManipulator.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IMeshManipulator.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IMeshManipulator()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IMeshManipulator, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::IMeshManipulator, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IMeshManipulator");
        buildMetaClass_IMeshManipulator(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


