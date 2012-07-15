// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/System/Vector2.hpp"

#include "cpgf/metadata/sfml/meta_sfml_Vector2.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_Global_vector2()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_vector2(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_Vector2()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<Vector2<int> > _nd = GDefineMetaClass<Vector2<int> >::declare("Vector2_int");
        buildMetaClass_Vector2<GDefineMetaClass<Vector2<int> >, int >(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<Vector2<float> > _nd = GDefineMetaClass<Vector2<float> >::declare("Vector2_float");
        buildMetaClass_Vector2<GDefineMetaClass<Vector2<float> >, float >(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


