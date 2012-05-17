// Auto generated file, don't modify.

#ifndef __META_SFML_VECTOR2_H
#define __META_SFML_VECTOR2_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindapi.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Global_vector2(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
}


template <typename D, typename T>
void buildMetaClass_Vector2(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (T, T)>();
    _d.CPGF_MD_TEMPLATE _field(replaceName("x", _r), &D::ClassType::x);
    _d.CPGF_MD_TEMPLATE _field(replaceName("y", _r), &D::ClassType::y);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
