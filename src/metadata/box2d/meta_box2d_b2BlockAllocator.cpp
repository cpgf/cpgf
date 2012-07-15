// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Common/b2BlockAllocator.h"

#include "cpgf/metadata/box2d/meta_box2d_b2BlockAllocator.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_Global_b2blockallocator()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_b2blockallocator(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2BlockAllocator()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2BlockAllocator> _nd = GDefineMetaClass<b2BlockAllocator>::declare("b2BlockAllocator");
        buildMetaClass_B2BlockAllocator(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_box2d


