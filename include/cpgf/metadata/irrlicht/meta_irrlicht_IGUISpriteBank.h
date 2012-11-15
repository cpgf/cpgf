// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IGUISPRITEBANK_H
#define __META_IRRLICHT_IGUISPRITEBANK_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IGUISpriteBank(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getPositions", &D::ClassType::getPositions);
    _d.CPGF_MD_TEMPLATE _method("getSprites", &D::ClassType::getSprites);
    _d.CPGF_MD_TEMPLATE _method("getTextureCount", &D::ClassType::getTextureCount);
    _d.CPGF_MD_TEMPLATE _method("getTexture", &D::ClassType::getTexture);
    _d.CPGF_MD_TEMPLATE _method("addTexture", &D::ClassType::addTexture);
    _d.CPGF_MD_TEMPLATE _method("setTexture", &D::ClassType::setTexture);
    _d.CPGF_MD_TEMPLATE _method("addTextureAsSprite", &D::ClassType::addTextureAsSprite);
    _d.CPGF_MD_TEMPLATE _method("clear", &D::ClassType::clear);
    _d.CPGF_MD_TEMPLATE _method("draw2DSprite", &D::ClassType::draw2DSprite)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(video::SColor(255, 255, 255, 255)))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw2DSpriteBatch", &D::ClassType::draw2DSpriteBatch)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(video::SColor(255, 255, 255, 255)))
        ._default(copyVariantFromCopyable(0))
    ;
}


template <typename D>
void buildMetaClass_SGUISprite(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("Frames", &D::ClassType::Frames);
    _d.CPGF_MD_TEMPLATE _field("frameTime", &D::ClassType::frameTime);
}


template <typename D>
void buildMetaClass_SGUISpriteFrame(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("textureNumber", &D::ClassType::textureNumber);
    _d.CPGF_MD_TEMPLATE _field("rectNumber", &D::ClassType::rectNumber);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
