// Auto generated file, don't modify.

#include "irrlicht.h"
#include "ILightSceneNode.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_ILightSceneNode.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_ILightSceneNode()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::ILightSceneNode, irr::scene::ISceneNode> _nd = GDefineMetaClass<irr::scene::ILightSceneNode, irr::scene::ISceneNode>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ILightSceneNode");
        buildMetaClass_ILightSceneNode(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


