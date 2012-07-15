// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/System/NonCopyable.hpp"

#include "cpgf/metadata/sfml/meta_sfml_NonCopyable.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_NonCopyable()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::NonCopyable> _nd = GDefineMetaClass<sf::NonCopyable>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("NonCopyable");
        buildMetaClass_NonCopyable(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


