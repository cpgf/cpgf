#include "Box2D/Box2D.h"
#include "Box2D/Collision/b2TimeOfImpact.h"

#include "cpgf/metadata/box2d/meta_b2TimeOfImpact.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _d = GDefineMetaClass<void>::define("box2d");
    buildMetaClass_Global_b2timeofimpact(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2TOIInput> _d = GDefineMetaClass<b2TOIInput>::declare("b2TOIInput");
    _ns._class(_d);
    buildMetaClass_B2TOIInput(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2TOIOutput> _d = GDefineMetaClass<b2TOIOutput>::declare("b2TOIOutput");
    _ns._class(_d);
    buildMetaClass_B2TOIOutput(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

}

} // unnamed namespace

