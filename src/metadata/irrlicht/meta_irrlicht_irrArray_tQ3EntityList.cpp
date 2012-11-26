// Auto generated file, don't modify.

#include "irrlicht.h"
#include "irrArray.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_irrArray.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Array_TemplateInstance_tQ3EntityList()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<array<irr::scene::quake3::IEntity, irrAllocator<irr::scene::quake3::IEntity> > > _nd = GDefineMetaClass<array<irr::scene::quake3::IEntity, irrAllocator<irr::scene::quake3::IEntity> > >::declare("tQ3EntityList");
        buildMetaClass_Array<GDefineMetaClass<array<irr::scene::quake3::IEntity, irrAllocator<irr::scene::quake3::IEntity> > >, irr::scene::quake3::IEntity, irrAllocator<irr::scene::quake3::IEntity>  >(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}
} // namespace meta_irrlicht


