// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IMeshLoader.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IMeshLoader.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IMeshLoader()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IMeshLoader, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::IMeshLoader, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IMeshLoader");
        buildMetaClass_IMeshLoader(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


