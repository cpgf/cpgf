// Auto generated file, don't modify.

#ifndef __META_SFML_RENDERTARGET_H
#define __META_SFML_RENDERTARGET_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindapi.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_RenderTarget(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("Clear", _r), &D::ClassType::Clear)
        ._default(copyVariantFromCopyable(Color(0, 0, 0)))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("Draw", _r), &D::ClassType::Draw);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetWidth", _r), &D::ClassType::GetWidth);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetHeight", _r), &D::ClassType::GetHeight);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetView", _r), &D::ClassType::SetView);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetView", _r), &D::ClassType::GetView);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetDefaultView", _r), &D::ClassType::GetDefaultView);
    _d.CPGF_MD_TEMPLATE _method(replaceName("PreserveOpenGLStates", _r), &D::ClassType::PreserveOpenGLStates);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
