// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IMeshSceneNode.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IMeshSceneNode.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IMeshSceneNode()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IMeshSceneNode, irr::scene::ISceneNode> _nd = GDefineMetaClass<irr::scene::IMeshSceneNode, irr::scene::ISceneNode>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IMeshSceneNode");
        buildMetaClass_IMeshSceneNode(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


