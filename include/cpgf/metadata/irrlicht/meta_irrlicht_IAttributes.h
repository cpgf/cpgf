// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IATTRIBUTES_H
#define __META_IRRLICHT_IATTRIBUTES_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;
using namespace irr::io;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IAttributes(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getAttributeCount", &D::ClassType::getAttributeCount);
    _d.CPGF_MD_TEMPLATE _method("getAttributeName", &D::ClassType::getAttributeName);
    _d.CPGF_MD_TEMPLATE _method("getAttributeType", (E_ATTRIBUTE_TYPE (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeType);
    _d.CPGF_MD_TEMPLATE _method("getAttributeType", (E_ATTRIBUTE_TYPE (D::ClassType::*) (s32))&D::ClassType::getAttributeType);
    _d.CPGF_MD_TEMPLATE _method("getAttributeTypeString", (const wchar_t * (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeTypeString);
    _d.CPGF_MD_TEMPLATE _method("getAttributeTypeString", (const wchar_t * (D::ClassType::*) (s32))&D::ClassType::getAttributeTypeString);
    _d.CPGF_MD_TEMPLATE _method("existsAttribute", &D::ClassType::existsAttribute);
    _d.CPGF_MD_TEMPLATE _method("findAttribute", &D::ClassType::findAttribute);
    _d.CPGF_MD_TEMPLATE _method("clear", &D::ClassType::clear);
    _d.CPGF_MD_TEMPLATE _method("read", &D::ClassType::read)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("write", &D::ClassType::write)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("addInt", &D::ClassType::addInt);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, s32))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsInt", (s32 (D::ClassType::*) (const c8 *) const)&D::ClassType::getAttributeAsInt);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsInt", (s32 (D::ClassType::*) (s32) const)&D::ClassType::getAttributeAsInt);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, s32))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addFloat", &D::ClassType::addFloat);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, f32))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsFloat", (f32 (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsFloat);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsFloat", (f32 (D::ClassType::*) (s32))&D::ClassType::getAttributeAsFloat);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, f32))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addString", (void (D::ClassType::*) (const c8 *, const c8 *))&D::ClassType::addString);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, const c8 *))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsString", (core::stringc (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsString);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsString", (void (D::ClassType::*) (const c8 *, c8 *))&D::ClassType::getAttributeAsString);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsString", (core::stringc (D::ClassType::*) (s32))&D::ClassType::getAttributeAsString);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, const c8 *))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addString", (void (D::ClassType::*) (const c8 *, const wchar_t *))&D::ClassType::addString);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, const wchar_t *))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsStringW", (core::stringw (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsStringW);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsStringW", (void (D::ClassType::*) (const c8 *, wchar_t *))&D::ClassType::getAttributeAsStringW);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsStringW", (core::stringw (D::ClassType::*) (s32))&D::ClassType::getAttributeAsStringW);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, const wchar_t *))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addBinary", &D::ClassType::addBinary);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, void *, s32))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsBinaryData", (void (D::ClassType::*) (const c8 *, void *, s32))&D::ClassType::getAttributeAsBinaryData);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsBinaryData", (void (D::ClassType::*) (s32, void *, s32))&D::ClassType::getAttributeAsBinaryData);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, void *, s32))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addArray", &D::ClassType::addArray);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, const core::array< core::stringw > &))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsArray", (core::array< core::stringw > (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsArray);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsArray", (core::array< core::stringw > (D::ClassType::*) (s32))&D::ClassType::getAttributeAsArray);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, const core::array< core::stringw > &))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addBool", &D::ClassType::addBool);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, bool))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsBool", (bool (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsBool);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsBool", (bool (D::ClassType::*) (s32))&D::ClassType::getAttributeAsBool);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, bool))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addEnum", (void (D::ClassType::*) (const c8 *, const c8 *, const c8 *const *))&D::ClassType::addEnum);
    _d.CPGF_MD_TEMPLATE _method("addEnum", (void (D::ClassType::*) (const c8 *, s32, const c8 *const *))&D::ClassType::addEnum);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, const c8 *, const c8 *const *))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsEnumeration", (const c8 * (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsEnumeration);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsEnumeration", (s32 (D::ClassType::*) (const c8 *, const c8 *const *))&D::ClassType::getAttributeAsEnumeration);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsEnumeration", (s32 (D::ClassType::*) (s32, const c8 *const *))&D::ClassType::getAttributeAsEnumeration);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsEnumeration", (const c8 * (D::ClassType::*) (s32))&D::ClassType::getAttributeAsEnumeration);
    _d.CPGF_MD_TEMPLATE _method("getAttributeEnumerationLiteralsOfEnumeration", (void (D::ClassType::*) (const c8 *, core::array< core::stringc > &))&D::ClassType::getAttributeEnumerationLiteralsOfEnumeration);
    _d.CPGF_MD_TEMPLATE _method("getAttributeEnumerationLiteralsOfEnumeration", (void (D::ClassType::*) (s32, core::array< core::stringc > &))&D::ClassType::getAttributeEnumerationLiteralsOfEnumeration);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, const c8 *, const c8 *const *))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addColor", &D::ClassType::addColor);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, video::SColor))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsColor", (video::SColor (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsColor);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsColor", (video::SColor (D::ClassType::*) (s32))&D::ClassType::getAttributeAsColor);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, video::SColor))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addColorf", &D::ClassType::addColorf);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, video::SColorf))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsColorf", (video::SColorf (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsColorf);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsColorf", (video::SColorf (D::ClassType::*) (s32))&D::ClassType::getAttributeAsColorf);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, video::SColorf))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addVector3d", &D::ClassType::addVector3d);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, core::vector3df))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsVector3d", (core::vector3df (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsVector3d);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsVector3d", (core::vector3df (D::ClassType::*) (s32))&D::ClassType::getAttributeAsVector3d);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, core::vector3df))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addVector2d", &D::ClassType::addVector2d);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, core::vector2df))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsVector2d", (core::vector2df (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsVector2d);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsVector2d", (core::vector2df (D::ClassType::*) (s32))&D::ClassType::getAttributeAsVector2d);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, core::vector2df))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addPosition2d", &D::ClassType::addPosition2d);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, core::position2di))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsPosition2d", (core::position2di (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsPosition2d);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsPosition2d", (core::position2di (D::ClassType::*) (s32))&D::ClassType::getAttributeAsPosition2d);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, core::position2di))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addRect", &D::ClassType::addRect);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, core::rect< s32 >))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsRect", (core::rect< s32 > (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsRect);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsRect", (core::rect< s32 > (D::ClassType::*) (s32))&D::ClassType::getAttributeAsRect);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, core::rect< s32 >))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addDimension2d", &D::ClassType::addDimension2d);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, core::dimension2d< u32 >))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsDimension2d", (core::dimension2d< u32 > (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsDimension2d);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsDimension2d", (core::dimension2d< u32 > (D::ClassType::*) (s32))&D::ClassType::getAttributeAsDimension2d);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, core::dimension2d< u32 >))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addMatrix", &D::ClassType::addMatrix);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, const core::matrix4 &))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsMatrix", (core::matrix4 (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsMatrix);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsMatrix", (core::matrix4 (D::ClassType::*) (s32))&D::ClassType::getAttributeAsMatrix);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, const core::matrix4 &))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addQuaternion", &D::ClassType::addQuaternion);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, core::quaternion))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsQuaternion", (core::quaternion (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsQuaternion);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsQuaternion", (core::quaternion (D::ClassType::*) (s32))&D::ClassType::getAttributeAsQuaternion);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, core::quaternion))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addBox3d", &D::ClassType::addBox3d);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, core::aabbox3df))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsBox3d", (core::aabbox3df (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsBox3d);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsBox3d", (core::aabbox3df (D::ClassType::*) (s32))&D::ClassType::getAttributeAsBox3d);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, core::aabbox3df))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addPlane3d", &D::ClassType::addPlane3d);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, core::plane3df))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsPlane3d", (core::plane3df (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsPlane3d);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsPlane3d", (core::plane3df (D::ClassType::*) (s32))&D::ClassType::getAttributeAsPlane3d);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, core::plane3df))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addTriangle3d", &D::ClassType::addTriangle3d);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, core::triangle3df))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsTriangle3d", (core::triangle3df (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsTriangle3d);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsTriangle3d", (core::triangle3df (D::ClassType::*) (s32))&D::ClassType::getAttributeAsTriangle3d);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, core::triangle3df))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addLine2d", &D::ClassType::addLine2d);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, core::line2df))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsLine2d", (core::line2df (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsLine2d);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsLine2d", (core::line2df (D::ClassType::*) (s32))&D::ClassType::getAttributeAsLine2d);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, core::line2df))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addLine3d", &D::ClassType::addLine3d);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, core::line3df))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsLine3d", (core::line3df (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsLine3d);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsLine3d", (core::line3df (D::ClassType::*) (s32))&D::ClassType::getAttributeAsLine3d);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, core::line3df))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("addTexture", &D::ClassType::addTexture, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<2> >())
        ._default(copyVariantFromCopyable(""))
    ;
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, video::ITexture *, const io::path &))&D::ClassType::setAttribute, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<2> >())
        ._default(copyVariantFromCopyable(""))
    ;
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsTexture", (video::ITexture * (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsTexture);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsTexture", (video::ITexture * (D::ClassType::*) (s32))&D::ClassType::getAttributeAsTexture);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, video::ITexture *, const io::path &))&D::ClassType::setAttribute, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<2> >())
        ._default(copyVariantFromCopyable(""))
    ;
    _d.CPGF_MD_TEMPLATE _method("addUserPointer", &D::ClassType::addUserPointer);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (const c8 *, void *))&D::ClassType::setAttribute);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsUserPointer", (void * (D::ClassType::*) (const c8 *))&D::ClassType::getAttributeAsUserPointer);
    _d.CPGF_MD_TEMPLATE _method("getAttributeAsUserPointer", (void * (D::ClassType::*) (s32))&D::ClassType::getAttributeAsUserPointer);
    _d.CPGF_MD_TEMPLATE _method("setAttribute", (void (D::ClassType::*) (s32, void *))&D::ClassType::setAttribute);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
