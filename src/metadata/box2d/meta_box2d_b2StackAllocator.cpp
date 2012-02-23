// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Common/b2StackAllocator.h"

#include "cpgf/metadata/box2d/meta_box2d_b2StackAllocator.h"

#include "cpgf/gmetapolicy.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_Global_b2stackallocator()
{
    GDefineMetaDangle _d = GDefineMetaDangle::dangle();
    buildMetaClass_Global_b2stackallocator(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2StackAllocator()
{
    GDefineMetaClass<b2StackAllocator> _d = GDefineMetaClass<b2StackAllocator>::declare("b2StackAllocator");
    buildMetaClass_B2StackAllocator(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2StackEntry()
{
    GDefineMetaClass<b2StackEntry> _d = GDefineMetaClass<b2StackEntry>::declare("b2StackEntry");
    buildMetaClass_B2StackEntry(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


} // namespace meta_box2d 


