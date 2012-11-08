// Auto generated file, don't modify.

#include "irrlicht.h"
#include "SMesh.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_SMesh.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_SMesh()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::SMesh, irr::scene::IMesh> _nd = GDefineMetaClass<irr::scene::SMesh, irr::scene::IMesh>::declare("SMesh");
        buildMetaClass_SMesh(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


