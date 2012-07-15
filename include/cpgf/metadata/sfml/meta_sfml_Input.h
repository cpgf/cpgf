// Auto generated file, don't modify.

#ifndef __META_SFML_INPUT_H
#define __META_SFML_INPUT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Input(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method("IsKeyDown", &D::ClassType::IsKeyDown);
    _d.CPGF_MD_TEMPLATE _method("IsMouseButtonDown", &D::ClassType::IsMouseButtonDown);
    _d.CPGF_MD_TEMPLATE _method("IsJoystickButtonDown", &D::ClassType::IsJoystickButtonDown);
    _d.CPGF_MD_TEMPLATE _method("GetMouseX", &D::ClassType::GetMouseX);
    _d.CPGF_MD_TEMPLATE _method("GetMouseY", &D::ClassType::GetMouseY);
    _d.CPGF_MD_TEMPLATE _method("GetJoystickAxis", &D::ClassType::GetJoystickAxis);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
