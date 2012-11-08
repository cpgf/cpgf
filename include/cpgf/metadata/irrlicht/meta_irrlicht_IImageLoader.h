// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IIMAGELOADER_H
#define __META_IRRLICHT_IIMAGELOADER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::io;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IImageLoader(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("isALoadableFileExtension", &D::ClassType::isALoadableFileExtension, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("isALoadableFileFormat", &D::ClassType::isALoadableFileFormat);
    _d.CPGF_MD_TEMPLATE _method("loadImage", &D::ClassType::loadImage);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
