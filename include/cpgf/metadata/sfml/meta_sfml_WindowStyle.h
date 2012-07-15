// Auto generated file, don't modify.

#ifndef __META_SFML_WINDOWSTYLE_H
#define __META_SFML_WINDOWSTYLE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;
using namespace sf::Style;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Global_windowstyle(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<long long>("GlobalEnum_sfml_3")
        ._element("None", sf::Style::None)
        ._element("Titlebar", sf::Style::Titlebar)
        ._element("Resize", sf::Style::Resize)
        ._element("Close", sf::Style::Close)
        ._element("Fullscreen", sf::Style::Fullscreen)
    ;
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
