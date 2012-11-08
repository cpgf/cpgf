// Auto generated file, don't modify.

#include "irrlicht.h"
#include "ISceneNodeFactory.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_ISceneNodeFactory.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_ISceneNodeFactory()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::ISceneNodeFactory, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::ISceneNodeFactory, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ISceneNodeFactory");
        buildMetaClass_ISceneNodeFactory(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


