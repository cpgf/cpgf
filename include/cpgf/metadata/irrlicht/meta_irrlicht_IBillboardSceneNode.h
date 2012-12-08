// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IBILLBOARDSCENENODE_H
#define __META_IRRLICHT_IBILLBOARDSCENENODE_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IBillboardSceneNode(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("setSize", (void (D::ClassType::*) (const core::dimension2d< f32 > &))&D::ClassType::setSize);
    _d.CPGF_MD_TEMPLATE _method("setSize", (void (D::ClassType::*) (f32, f32, f32))&D::ClassType::setSize);
    _d.CPGF_MD_TEMPLATE _method("getSize", (const core::dimension2d< f32 > & (D::ClassType::*) () const)&D::ClassType::getSize);
    _d.CPGF_MD_TEMPLATE _method("getSize", (void (D::ClassType::*) (f32 &, f32 &, f32 &) const)&D::ClassType::getSize);
    _d.CPGF_MD_TEMPLATE _method("setColor", (void (D::ClassType::*) (const video::SColor &))&D::ClassType::setColor);
    _d.CPGF_MD_TEMPLATE _method("setColor", (void (D::ClassType::*) (const video::SColor &, const video::SColor &))&D::ClassType::setColor);
    _d.CPGF_MD_TEMPLATE _method("getColor", &D::ClassType::getColor);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
