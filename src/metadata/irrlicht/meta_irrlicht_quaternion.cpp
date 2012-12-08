// Auto generated file, don't modify.

#include "irrlicht.h"
#include "quaternion.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_quaternion.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_quaternion()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_quaternion(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_Quaternion()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::core::quaternion> _nd = GDefineMetaClass<irr::core::quaternion>::declare("quaternion");
        buildMetaClass_Quaternion(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


