// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IMeshCache.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IMeshCache.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IMeshCache()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IMeshCache, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::IMeshCache, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IMeshCache");
        buildMetaClass_IMeshCache(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


