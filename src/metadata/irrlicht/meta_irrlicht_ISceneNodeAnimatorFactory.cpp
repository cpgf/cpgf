// Auto generated file, don't modify.

#include "irrlicht.h"
#include "ISceneNodeAnimatorFactory.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_ISceneNodeAnimatorFactory.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_ISceneNodeAnimatorFactory()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::ISceneNodeAnimatorFactory, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::ISceneNodeAnimatorFactory, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ISceneNodeAnimatorFactory");
        buildMetaClass_ISceneNodeAnimatorFactory(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


