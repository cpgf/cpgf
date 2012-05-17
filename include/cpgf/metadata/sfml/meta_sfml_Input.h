// Auto generated file, don't modify.

#ifndef __META_SFML_INPUT_H
#define __META_SFML_INPUT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindapi.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Input(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsKeyDown", _r), &D::ClassType::IsKeyDown);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsMouseButtonDown", _r), &D::ClassType::IsMouseButtonDown);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsJoystickButtonDown", _r), &D::ClassType::IsJoystickButtonDown);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetMouseX", _r), &D::ClassType::GetMouseX);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetMouseY", _r), &D::ClassType::GetMouseY);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetJoystickAxis", _r), &D::ClassType::GetJoystickAxis);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
