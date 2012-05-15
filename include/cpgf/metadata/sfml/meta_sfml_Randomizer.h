// Auto generated file, don't modify.

#ifndef __META_SFML_RANDOMIZER_H
#define __META_SFML_RANDOMIZER_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Randomizer(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetSeed", _r), &D::ClassType::SetSeed);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetSeed", _r), &D::ClassType::GetSeed);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Random", _r), (float (*) (float, float))&D::ClassType::Random);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Random", _r), (int (*) (int, int))&D::ClassType::Random);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
