// Auto generated file, don't modify.

#include "irrlicht.h"
#include "ISceneCollisionManager.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_ISceneCollisionManager.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_ISceneCollisionManager()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::ISceneCollisionManager, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::ISceneCollisionManager, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ISceneCollisionManager");
        buildMetaClass_ISceneCollisionManager(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


