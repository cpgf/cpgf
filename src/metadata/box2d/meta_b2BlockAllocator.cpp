#include "Box2D/Box2D.h"
#include "Box2D/Common/b2BlockAllocator.h"

#include "cpgf/metadata/box2d/meta_b2BlockAllocator.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _d = GDefineMetaClass<void>::define("box2d");
    buildMetaClass_Global_b2blockallocator(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2BlockAllocator> _d = GDefineMetaClass<b2BlockAllocator>::Policy<MakePolicy<GMetaRuleDestructorAbsent> >::declare("b2BlockAllocator");
    _ns._class(_d);
    buildMetaClass_B2BlockAllocator(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

}

} // unnamed namespace

