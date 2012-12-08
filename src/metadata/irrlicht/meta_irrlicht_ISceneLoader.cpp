// Auto generated file, don't modify.

#include "irrlicht.h"
#include "ISceneLoader.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_ISceneLoader.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_ISceneLoader()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::ISceneLoader, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::ISceneLoader, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ISceneLoader");
        buildMetaClass_ISceneLoader(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


