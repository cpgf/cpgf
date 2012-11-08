// Auto generated file, don't modify.

#include "irrlicht.h"
#include "ICameraSceneNode.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_ICameraSceneNode.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_ICameraSceneNode()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::ICameraSceneNode, irr::scene::ISceneNode, irr::IEventReceiver> _nd = GDefineMetaClass<irr::scene::ICameraSceneNode, irr::scene::ISceneNode, irr::IEventReceiver>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ICameraSceneNode");
        buildMetaClass_ICameraSceneNode(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


