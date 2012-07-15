// Auto generated file, don't modify.

#ifndef __META_SFML_MATRIX3_H
#define __META_SFML_MATRIX3_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Matrix3(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (float, float, float, float, float, float, float, float, float)>();
    _d.CPGF_MD_TEMPLATE _field("Identity", &D::ClassType::Identity);
    _d.CPGF_MD_TEMPLATE _method("SetFromTransformations", &D::ClassType::SetFromTransformations);
    _d.CPGF_MD_TEMPLATE _method("Transform", &D::ClassType::Transform);
    _d.CPGF_MD_TEMPLATE _method("GetInverse", &D::ClassType::GetInverse);
    _d.CPGF_MD_TEMPLATE _method("Get4x4Elements", &D::ClassType::Get4x4Elements);
    _d.CPGF_MD_TEMPLATE _operator<float (*)(unsigned int, unsigned int)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _operator<float & (*)(unsigned int, unsigned int)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _operator<Matrix3 (*)(const cpgf::GMetaSelf &, const Matrix3 &)>(mopHolder * mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Matrix3 & (*)(cpgf::GMetaSelf, const Matrix3 &)>(mopHolder *= mopHolder);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
