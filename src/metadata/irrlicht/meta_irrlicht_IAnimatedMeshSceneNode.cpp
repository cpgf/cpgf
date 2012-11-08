// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IAnimatedMeshSceneNode.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IAnimatedMeshSceneNode.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_ianimatedmeshscenenode()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_ianimatedmeshscenenode(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IAnimatedMeshSceneNode()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IAnimatedMeshSceneNode, irr::scene::ISceneNode> _nd = GDefineMetaClass<irr::scene::IAnimatedMeshSceneNode, irr::scene::ISceneNode>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IAnimatedMeshSceneNode");
        buildMetaClass_IAnimatedMeshSceneNode(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IAnimationEndCallBack()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IAnimationEndCallBack, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::IAnimationEndCallBack, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IAnimationEndCallBack");
        buildMetaClass_IAnimationEndCallBack(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


