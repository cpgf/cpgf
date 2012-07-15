// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Collision/b2DynamicTree.h"

#include "cpgf/metadata/box2d/meta_box2d_b2DynamicTree.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_Global_b2dynamictree()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_b2dynamictree(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2DynamicTree()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2DynamicTree> _nd = GDefineMetaClass<b2DynamicTree>::declare("b2DynamicTree");
        buildMetaClass_B2DynamicTree(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2DynamicTreeNode()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2DynamicTreeNode> _nd = GDefineMetaClass<b2DynamicTreeNode>::declare("b2DynamicTreeNode");
        buildMetaClass_B2DynamicTreeNode(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_box2d


