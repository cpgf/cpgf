/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_CppType
@@cpgf@creations@*/

#ifndef METADATA_META_CPPTYPE_H
#define METADATA_META_CPPTYPE_H

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif
#include "clang/AST/Decl.h"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif
#include "clang/AST/DeclTemplate.h"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include "model/cpptype.h"
#include "cpgf/metatraits/gmetaconverter_string.h"
#include "cpgf/metatraits/gmetaconverter_widestring.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/scriptbind/gscriptwrapper.h"
#include "cpgf/gscopedinterface.h"
#include "cpgf/gselectfunctionbyarity.h"
#include "cpgf/metadata/private/gmetadata_header.h"

namespace metadata {

template <typename TsE1f>
metagen::CppType &oPeRat0rWrapPer_metagen_CppType_opAssign_0(TsE1f * sE1F, const metagen::CppType &other)
{
    return (*sE1F) = other;
}

template <typename D_d >
void buildMetaClass_metagen_CppType(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    

    _d.CPGF_MD_TEMPLATE _constructor<void * (const clang::QualType &)>(cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _constructor<void * (const metagen::CppType &)>(cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());

    _d.CPGF_MD_TEMPLATE _operator<metagen::CppType &(*)(cpgf::GMetaSelf, const metagen::CppType &)>(mopHolder = mopHolder, cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_CppType_opAssign_0<typename D_d::ClassType >);

    _d.CPGF_MD_TEMPLATE _method("getQualifiedName", cpgf::selectFunctionByArity1(&D_d::ClassType::getQualifiedName), cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getQualifiedName", cpgf::selectFunctionByArity0(&D_d::ClassType::getQualifiedName));
    _d.CPGF_MD_TEMPLATE _method("isConst", &D_d::ClassType::isConst);
    _d.CPGF_MD_TEMPLATE _method("isVolatile", &D_d::ClassType::isVolatile);
    _d.CPGF_MD_TEMPLATE _method("isArray", &D_d::ClassType::isArray);
    _d.CPGF_MD_TEMPLATE _method("isPointer", &D_d::ClassType::isPointer);
    _d.CPGF_MD_TEMPLATE _method("isMultiplePointer", &D_d::ClassType::isMultiplePointer);
    _d.CPGF_MD_TEMPLATE _method("isPointerToConst", &D_d::ClassType::isPointerToConst);
    _d.CPGF_MD_TEMPLATE _method("isPointerToVolatile", &D_d::ClassType::isPointerToVolatile);
    _d.CPGF_MD_TEMPLATE _method("isReference", &D_d::ClassType::isReference);
    _d.CPGF_MD_TEMPLATE _method("isReferenceToConst", &D_d::ClassType::isReferenceToConst);
    _d.CPGF_MD_TEMPLATE _method("isReferenceToVolatile", &D_d::ClassType::isReferenceToVolatile);
    _d.CPGF_MD_TEMPLATE _method("isReferenceToValue", &D_d::ClassType::isReferenceToValue);
    _d.CPGF_MD_TEMPLATE _method("isFunction", &D_d::ClassType::isFunction);
    _d.CPGF_MD_TEMPLATE _method("isFunctionPointer", &D_d::ClassType::isFunctionPointer);
    _d.CPGF_MD_TEMPLATE _method("isVoid", &D_d::ClassType::isVoid);
    _d.CPGF_MD_TEMPLATE _method("isFundamental", &D_d::ClassType::isFundamental);
    _d.CPGF_MD_TEMPLATE _method("isTemplateDependent", &D_d::ClassType::isTemplateDependent);
    _d.CPGF_MD_TEMPLATE _method("getNonReferenceType", &D_d::ClassType::getNonReferenceType);
    _d.CPGF_MD_TEMPLATE _method("getBaseType", &D_d::ClassType::getBaseType);
    _d.CPGF_MD_TEMPLATE _method("getCXXRecordDecl", &D_d::ClassType::getCXXRecordDecl);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_CppType();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
