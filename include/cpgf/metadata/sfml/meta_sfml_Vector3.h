// Auto generated file, don't modify.

#ifndef __META_SFML_VECTOR3_H
#define __META_SFML_VECTOR3_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Global_vector3(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
}


template <typename D, typename T>
void buildMetaClass_Vector3(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (T, T, T)>();
    _d.CPGF_MD_TEMPLATE _field("x", &D::ClassType::x);
    _d.CPGF_MD_TEMPLATE _field("y", &D::ClassType::y);
    _d.CPGF_MD_TEMPLATE _field("z", &D::ClassType::z);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
