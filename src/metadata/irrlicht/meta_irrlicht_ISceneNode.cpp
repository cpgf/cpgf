// Auto generated file, don't modify.

#include "irrlicht.h"
#include "ISceneNode.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_ISceneNode.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_ISceneNode()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::ISceneNode, irr::io::IAttributeExchangingObject> _nd = GDefineMetaClass<irr::scene::ISceneNode, irr::io::IAttributeExchangingObject>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ISceneNode");
        buildMetaClass_ISceneNode(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<ISceneNodeWrapper, irr::scene::ISceneNode> _nd = GDefineMetaClass<ISceneNodeWrapper, irr::scene::ISceneNode>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("ISceneNodeWrapper");
        buildMetaClass_ISceneNodeWrapper(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


