// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IBoneSceneNode.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IBoneSceneNode.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_ibonescenenode()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_ibonescenenode(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IBoneSceneNode()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IBoneSceneNode, irr::scene::ISceneNode> _nd = GDefineMetaClass<irr::scene::IBoneSceneNode, irr::scene::ISceneNode>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IBoneSceneNode");
        buildMetaClass_IBoneSceneNode(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


