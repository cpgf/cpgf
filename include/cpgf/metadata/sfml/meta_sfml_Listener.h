// Auto generated file, don't modify.

#ifndef __META_SFML_LISTENER_H
#define __META_SFML_LISTENER_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindapi.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Listener(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetGlobalVolume", _r), &D::ClassType::SetGlobalVolume);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetGlobalVolume", _r), &D::ClassType::GetGlobalVolume);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetPosition", _r), (void (*) (float, float, float))&D::ClassType::SetPosition);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetPosition", _r), (void (*) (const Vector3f &))&D::ClassType::SetPosition);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetPosition", _r), &D::ClassType::GetPosition);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetTarget", _r), (void (*) (float, float, float))&D::ClassType::SetTarget);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetTarget", _r), (void (*) (const Vector3f &))&D::ClassType::SetTarget);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetTarget", _r), &D::ClassType::GetTarget);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
