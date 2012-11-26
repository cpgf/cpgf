// Auto generated file, don't modify.

#include "irrlicht.h"
#include "irrArray.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_irrArray.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Array_TemplateInstance_array_ISceneNodePointer()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<array<scene::ISceneNode *, irrAllocator<scene::ISceneNode *> > > _nd = GDefineMetaClass<array<scene::ISceneNode *, irrAllocator<scene::ISceneNode *> > >::declare("array_ISceneNodePointer");
        buildMetaClass_Array<GDefineMetaClass<array<scene::ISceneNode *, irrAllocator<scene::ISceneNode *> > >, scene::ISceneNode *, irrAllocator<scene::ISceneNode *>  >(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}
} // namespace meta_irrlicht


