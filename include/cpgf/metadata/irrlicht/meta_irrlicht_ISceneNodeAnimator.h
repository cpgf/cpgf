// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ISCENENODEANIMATOR_H
#define __META_IRRLICHT_ISCENENODEANIMATOR_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::io;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_ISceneNodeAnimator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("animateNode", &D::ClassType::animateNode);
    _d.CPGF_MD_TEMPLATE _method("createClone", &D::ClassType::createClone)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("isEventReceiverEnabled", &D::ClassType::isEventReceiverEnabled);
    _d.CPGF_MD_TEMPLATE _method("OnEvent", &D::ClassType::OnEvent);
    _d.CPGF_MD_TEMPLATE _method("getType", &D::ClassType::getType);
    _d.CPGF_MD_TEMPLATE _method("hasFinished", &D::ClassType::hasFinished);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
