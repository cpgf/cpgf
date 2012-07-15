// Auto generated file, don't modify.

#ifndef __META_SFML_LISTENER_H
#define __META_SFML_LISTENER_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Listener(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("SetGlobalVolume", &D::ClassType::SetGlobalVolume);
    _d.CPGF_MD_TEMPLATE _method("GetGlobalVolume", &D::ClassType::GetGlobalVolume);
    _d.CPGF_MD_TEMPLATE _method("SetPosition", (void (*) (float, float, float))&D::ClassType::SetPosition);
    _d.CPGF_MD_TEMPLATE _method("SetPosition", (void (*) (const Vector3f &))&D::ClassType::SetPosition);
    _d.CPGF_MD_TEMPLATE _method("GetPosition", &D::ClassType::GetPosition);
    _d.CPGF_MD_TEMPLATE _method("SetTarget", (void (*) (float, float, float))&D::ClassType::SetTarget);
    _d.CPGF_MD_TEMPLATE _method("SetTarget", (void (*) (const Vector3f &))&D::ClassType::SetTarget);
    _d.CPGF_MD_TEMPLATE _method("GetTarget", &D::ClassType::GetTarget);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
