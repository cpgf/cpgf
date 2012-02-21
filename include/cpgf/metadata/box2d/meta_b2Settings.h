// Auto generated file, don't modify.

#ifndef __META_B2SETTINGS_H
#define __META_B2SETTINGS_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


template <typename D, typename Policy>
void buildMetaClass_Global_b2settings(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("b2_version", _r), &b2_version, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Alloc", _r), (void * (*) (int32))&b2Alloc, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Free", _r), (void (*) (void *))&b2Free, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2MixFriction", _r), (float32 (*) (float32, float32))&b2MixFriction, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2MixRestitution", _r), (float32 (*) (float32, float32))&b2MixRestitution, _p);
    _d.CPGF_MD_TEMPLATE _enum<long long>(replaceName("GlobalDefine_box2d_3", _r))
        ._element(replaceName("b2_maxFloat", _r), b2_maxFloat)
        ._element(replaceName("b2_epsilon", _r), b2_epsilon)
        ._element(replaceName("b2_pi", _r), b2_pi)
        ._element(replaceName("b2_maxManifoldPoints", _r), b2_maxManifoldPoints)
        ._element(replaceName("b2_maxPolygonVertices", _r), b2_maxPolygonVertices)
        ._element(replaceName("b2_aabbExtension", _r), b2_aabbExtension)
        ._element(replaceName("b2_aabbMultiplier", _r), b2_aabbMultiplier)
        ._element(replaceName("b2_linearSlop", _r), b2_linearSlop)
        ._element(replaceName("b2_angularSlop", _r), b2_angularSlop)
        ._element(replaceName("b2_polygonRadius", _r), b2_polygonRadius)
        ._element(replaceName("b2_maxTOIContacts", _r), b2_maxTOIContacts)
        ._element(replaceName("b2_velocityThreshold", _r), b2_velocityThreshold)
        ._element(replaceName("b2_maxLinearCorrection", _r), b2_maxLinearCorrection)
        ._element(replaceName("b2_maxAngularCorrection", _r), b2_maxAngularCorrection)
        ._element(replaceName("b2_maxTranslation", _r), b2_maxTranslation)
        ._element(replaceName("b2_maxTranslationSquared", _r), b2_maxTranslationSquared)
        ._element(replaceName("b2_maxRotation", _r), b2_maxRotation)
        ._element(replaceName("b2_maxRotationSquared", _r), b2_maxRotationSquared)
        ._element(replaceName("b2_contactBaumgarte", _r), b2_contactBaumgarte)
        ._element(replaceName("b2_timeToSleep", _r), b2_timeToSleep)
        ._element(replaceName("b2_linearSleepTolerance", _r), b2_linearSleepTolerance)
        ._element(replaceName("b2_angularSleepTolerance", _r), b2_angularSleepTolerance)
    ;
}


template <typename D, typename Policy>
void buildMetaClass_B2Version(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("major", _r), &D::ClassType::major, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("minor", _r), &D::ClassType::minor, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("revision", _r), &D::ClassType::revision, _p);
}


template <typename D, typename Policy>
void buildMetaFile_B2Settings(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    buildMetaClass_Global_b2settings(config, _d, _r, _p);
    buildMetaClass_B2Version(config, _d, _r, _p);
}




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif