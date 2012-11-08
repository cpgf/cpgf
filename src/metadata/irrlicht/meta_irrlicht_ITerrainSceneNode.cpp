// Auto generated file, don't modify.

#include "irrlicht.h"
#include "ITerrainSceneNode.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_ITerrainSceneNode.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_ITerrainSceneNode()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::ITerrainSceneNode, irr::scene::ISceneNode> _nd = GDefineMetaClass<irr::scene::ITerrainSceneNode, irr::scene::ISceneNode>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ITerrainSceneNode");
        buildMetaClass_ITerrainSceneNode(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


