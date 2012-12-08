// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IIMAGE_H
#define __META_IRRLICHT_IIMAGE_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IImage(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("lock", &D::ClassType::lock);
    _d.CPGF_MD_TEMPLATE _method("unlock", &D::ClassType::unlock);
    _d.CPGF_MD_TEMPLATE _method("getDimension", &D::ClassType::getDimension);
    _d.CPGF_MD_TEMPLATE _method("getBitsPerPixel", &D::ClassType::getBitsPerPixel);
    _d.CPGF_MD_TEMPLATE _method("getBytesPerPixel", &D::ClassType::getBytesPerPixel);
    _d.CPGF_MD_TEMPLATE _method("getImageDataSizeInBytes", &D::ClassType::getImageDataSizeInBytes);
    _d.CPGF_MD_TEMPLATE _method("getImageDataSizeInPixels", &D::ClassType::getImageDataSizeInPixels);
    _d.CPGF_MD_TEMPLATE _method("getPixel", &D::ClassType::getPixel);
    _d.CPGF_MD_TEMPLATE _method("setPixel", &D::ClassType::setPixel, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<2> >())
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("getColorFormat", &D::ClassType::getColorFormat);
    _d.CPGF_MD_TEMPLATE _method("getRedMask", &D::ClassType::getRedMask);
    _d.CPGF_MD_TEMPLATE _method("getGreenMask", &D::ClassType::getGreenMask);
    _d.CPGF_MD_TEMPLATE _method("getBlueMask", &D::ClassType::getBlueMask);
    _d.CPGF_MD_TEMPLATE _method("getAlphaMask", &D::ClassType::getAlphaMask);
    _d.CPGF_MD_TEMPLATE _method("getPitch", &D::ClassType::getPitch);
    _d.CPGF_MD_TEMPLATE _method("copyToScaling", (void (D::ClassType::*) (void *, u32, u32, ECOLOR_FORMAT, u32))&D::ClassType::copyToScaling)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(ECF_A8R8G8B8))
    ;
    _d.CPGF_MD_TEMPLATE _method("copyToScaling", (void (D::ClassType::*) (IImage *))&D::ClassType::copyToScaling);
    _d.CPGF_MD_TEMPLATE _method("copyTo", (void (D::ClassType::*) (IImage *, const core::position2d< s32 > &))&D::ClassType::copyTo)
        ._default(copyVariantFromCopyable(core::position2d< s32 >(0, 0)))
    ;
    _d.CPGF_MD_TEMPLATE _method("copyTo", (void (D::ClassType::*) (IImage *, const core::position2d< s32 > &, const core::rect< s32 > &, const core::rect< s32 > *))&D::ClassType::copyTo)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("copyToWithAlpha", &D::ClassType::copyToWithAlpha, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<3> >())
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("copyToScalingBoxFilter", &D::ClassType::copyToScalingBoxFilter)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("fill", &D::ClassType::fill, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getBitsPerPixelFromFormat", &D::ClassType::getBitsPerPixelFromFormat);
    _d.CPGF_MD_TEMPLATE _method("isRenderTargetOnlyFormat", &D::ClassType::isRenderTargetOnlyFormat);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
