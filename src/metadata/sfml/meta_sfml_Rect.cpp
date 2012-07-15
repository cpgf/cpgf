// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics/Rect.hpp"

#include "cpgf/metadata/sfml/meta_sfml_Rect.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_Rect()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<Rect<int> > _nd = GDefineMetaClass<Rect<int> >::declare("Rect_int");
        buildMetaClass_Rect<GDefineMetaClass<Rect<int> >, int >(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<Rect<float> > _nd = GDefineMetaClass<Rect<float> >::declare("Rect_float");
        buildMetaClass_Rect<GDefineMetaClass<Rect<float> >, float >(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


