// Auto generated file, don't modify.

#include "irrlicht.h"
#include "ISceneNodeAnimator.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_ISceneNodeAnimator.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_ISceneNodeAnimator()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::ISceneNodeAnimator, irr::io::IAttributeExchangingObject, irr::IEventReceiver> _nd = GDefineMetaClass<irr::scene::ISceneNodeAnimator, irr::io::IAttributeExchangingObject, irr::IEventReceiver>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ISceneNodeAnimator");
        buildMetaClass_ISceneNodeAnimator(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


