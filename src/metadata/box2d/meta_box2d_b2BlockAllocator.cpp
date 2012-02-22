// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Common/b2BlockAllocator.h"

#include "cpgf/metadata/box2d/meta_box2d_b2BlockAllocator.h"

#include "cpgf/gmetapolicy.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_Global_b2blockallocator()
{
    GDefineMetaNamespace _d = GDefineMetaNamespace::dangle("");
    buildMetaClass_Global_b2blockallocator(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2BlockAllocator()
{
    GDefineMetaClass<b2BlockAllocator> _d = GDefineMetaClass<b2BlockAllocator>::declare("b2BlockAllocator");
    buildMetaClass_B2BlockAllocator(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


} // namespace meta_box2d 


