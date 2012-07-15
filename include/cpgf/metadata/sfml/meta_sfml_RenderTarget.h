// Auto generated file, don't modify.

#ifndef __META_SFML_RENDERTARGET_H
#define __META_SFML_RENDERTARGET_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_RenderTarget(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("Clear", &D::ClassType::Clear)
        ._default(copyVariantFromCopyable(Color(0, 0, 0)))
    ;
    _d.CPGF_MD_TEMPLATE _method("Draw", &D::ClassType::Draw);
    _d.CPGF_MD_TEMPLATE _method("GetWidth", &D::ClassType::GetWidth);
    _d.CPGF_MD_TEMPLATE _method("GetHeight", &D::ClassType::GetHeight);
    _d.CPGF_MD_TEMPLATE _method("SetView", &D::ClassType::SetView);
    _d.CPGF_MD_TEMPLATE _method("GetView", &D::ClassType::GetView);
    _d.CPGF_MD_TEMPLATE _method("GetDefaultView", &D::ClassType::GetDefaultView);
    _d.CPGF_MD_TEMPLATE _method("PreserveOpenGLStates", &D::ClassType::PreserveOpenGLStates);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
