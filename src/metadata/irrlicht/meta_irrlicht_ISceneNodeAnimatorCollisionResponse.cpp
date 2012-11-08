// Auto generated file, don't modify.

#include "irrlicht.h"
#include "ISceneNodeAnimatorCollisionResponse.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_ISceneNodeAnimatorCollisionResponse.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_ICollisionCallback()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::ICollisionCallback, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::ICollisionCallback, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ICollisionCallback");
        buildMetaClass_ICollisionCallback(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_ISceneNodeAnimatorCollisionResponse()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::ISceneNodeAnimatorCollisionResponse, irr::scene::ISceneNodeAnimator> _nd = GDefineMetaClass<irr::scene::ISceneNodeAnimatorCollisionResponse, irr::scene::ISceneNodeAnimator>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ISceneNodeAnimatorCollisionResponse");
        buildMetaClass_ISceneNodeAnimatorCollisionResponse(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


