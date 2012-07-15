// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/System/Vector3.hpp"

#include "cpgf/metadata/sfml/meta_sfml_Vector3.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_Global_vector3()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_vector3(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_Vector3()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<Vector3<int> > _nd = GDefineMetaClass<Vector3<int> >::declare("Vector3_int");
        buildMetaClass_Vector3<GDefineMetaClass<Vector3<int> >, int >(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<Vector3<float> > _nd = GDefineMetaClass<Vector3<float> >::declare("Vector3_float");
        buildMetaClass_Vector3<GDefineMetaClass<Vector3<float> >, float >(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


