#include "Box2D/Box2D.h"
#include "Box2D/Collision/b2DynamicTree.h"

#include "cpgf/metadata/box2d/meta_b2DynamicTree.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _d = GDefineMetaClass<void>::define("box2d");
    buildMetaClass_Global_b2dynamictree(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2DynamicTree> _d = GDefineMetaClass<b2DynamicTree>::Policy<MakePolicy<GMetaRuleDestructorAbsent> >::declare("b2DynamicTree");
    _ns._class(_d);
    buildMetaClass_B2DynamicTree(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2DynamicTreeNode> _d = GDefineMetaClass<b2DynamicTreeNode>::declare("b2DynamicTreeNode");
    _ns._class(_d);
    buildMetaClass_B2DynamicTreeNode(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

}

} // unnamed namespace

