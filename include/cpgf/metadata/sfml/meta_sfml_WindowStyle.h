// Auto generated file, don't modify.

#ifndef __META_SFML_WINDOWSTYLE_H
#define __META_SFML_WINDOWSTYLE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;
using namespace sf::Style;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Global_windowstyle(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<long long>(replaceName("GlobalEnum_sfml_2", _r))
        ._element(replaceName("None", _r), sf::Style::None)
        ._element(replaceName("Titlebar", _r), sf::Style::Titlebar)
        ._element(replaceName("Resize", _r), sf::Style::Resize)
        ._element(replaceName("Close", _r), sf::Style::Close)
        ._element(replaceName("Fullscreen", _r), sf::Style::Fullscreen)
    ;
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
