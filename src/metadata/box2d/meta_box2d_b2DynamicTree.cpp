// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Collision/b2DynamicTree.h"

#include "cpgf/metadata/box2d/meta_box2d_b2DynamicTree.h"

#include "cpgf/gmetapolicy.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_Global_b2dynamictree()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_b2dynamictree(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2DynamicTree()
{
    GDefineMetaClass<b2DynamicTree> _d = GDefineMetaClass<b2DynamicTree>::declare("b2DynamicTree");
    buildMetaClass_B2DynamicTree(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2DynamicTreeNode()
{
    GDefineMetaClass<b2DynamicTreeNode> _d = GDefineMetaClass<b2DynamicTreeNode>::declare("b2DynamicTreeNode");
    buildMetaClass_B2DynamicTreeNode(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


} // namespace meta_box2d 


