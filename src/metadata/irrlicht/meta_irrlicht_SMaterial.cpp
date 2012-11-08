// Auto generated file, don't modify.

#include "irrlicht.h"
#include "SMaterial.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_SMaterial.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_smaterial()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_smaterial(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SMaterial()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::video::SMaterial> _nd = GDefineMetaClass<irr::video::SMaterial>::declare("SMaterial");
        buildMetaClass_SMaterial(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


