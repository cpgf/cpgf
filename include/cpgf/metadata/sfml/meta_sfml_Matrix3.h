// Auto generated file, don't modify.

#ifndef __META_SFML_MATRIX3_H
#define __META_SFML_MATRIX3_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Matrix3(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (float, float, float, float, float, float, float, float, float)>();
    _d.CPGF_MD_TEMPLATE _field(replaceName("Identity", _r), &D::ClassType::Identity);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetFromTransformations", _r), &D::ClassType::SetFromTransformations);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Transform", _r), &D::ClassType::Transform);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetInverse", _r), &D::ClassType::GetInverse);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Get4x4Elements", _r), &D::ClassType::Get4x4Elements);
    _d.CPGF_MD_TEMPLATE _operator<float (*)(unsigned int, unsigned int)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _operator<float & (*)(unsigned int, unsigned int)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _operator<Matrix3 (*)(const cpgf::GMetaSelf &, const Matrix3 &)>(mopHolder * mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Matrix3 & (*)(cpgf::GMetaSelf, const Matrix3 &)>(mopHolder *= mopHolder);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
