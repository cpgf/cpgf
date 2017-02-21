/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_namespace_cppitem
createMetaClass_metagen_CppItem
createMetaClass_metagen_CppNamedItem
@@cpgf@creations@*/

#ifndef METADATA_META_CPPITEM_H
#define METADATA_META_CPPITEM_H

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif
#include "clang/AST/Decl.h"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include "project.h"
#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif
#include "clang/AST/Decl.h"
#include "clang/Basic/Specifiers.h"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include "model/cppcontainer.h"
#include "model/cppitem.h"
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
metagen::CppItem &oPeRat0rWrapPer_metagen_CppItem_opAssign_0(TsE1f * sE1F, const metagen::CppItem &pAr9_Am0)
{
    return (*sE1F) = pAr9_Am0;
}

template <typename TsE1f>
metagen::CppNamedItem &oPeRat0rWrapPer_metagen_CppNamedItem_opAssign_0(TsE1f * sE1F, const metagen::CppNamedItem &pAr9_Am0)
{
    return (*sE1F) = pAr9_Am0;
}

template <typename D_d >
void buildMetaClass_metagen_namespace_cppitem(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    
    _d.CPGF_MD_TEMPLATE _enum<metagen::ItemVisibility>("ItemVisibility")
        ._element("ivFirst", metagen::ivFirst)
        ._element("ivPublic", metagen::ivPublic)
        ._element("ivProtected", metagen::ivProtected)
        ._element("ivPrivate", metagen::ivPrivate)
        ._element("ivCount", metagen::ivCount)
    ;
    _d.CPGF_MD_TEMPLATE _enum<metagen::ItemCategory>("ItemCategory")
        ._element("icFirst", metagen::icFirst)
        ._element("icFile", metagen::icFile)
        ._element("icNamespace", metagen::icNamespace)
        ._element("icClass", metagen::icClass)
        ._element("icEnum", metagen::icEnum)
        ._element("icConstructor", metagen::icConstructor)
        ._element("icDestructor", metagen::icDestructor)
        ._element("icField", metagen::icField)
        ._element("icMethod", metagen::icMethod)
        ._element("icOperator", metagen::icOperator)
        ._element("icCount", metagen::icCount)
    ;
    _d.CPGF_MD_TEMPLATE _enum<metagen::ItemTextOption>("ItemTextOption")
        ._element("itoWithArgType", metagen::itoWithArgType)
        ._element("itoWithArgName", metagen::itoWithArgName)
        ._element("itoWithDefaultValue", metagen::itoWithDefaultValue)
        ._element("itoIncludeArg", metagen::itoIncludeArg)
        ._element("itoWithResult", metagen::itoWithResult)
        ._element("itoWithName", metagen::itoWithName)
        ._element("itoWithQualifiers", metagen::itoWithQualifiers)
        ._element("itoAsPointer", metagen::itoAsPointer)
        ._element("itoWithParentName", metagen::itoWithParentName)
    ;

    _d.CPGF_MD_TEMPLATE _field("ItemNames", &metagen::ItemNames);

    _d.CPGF_MD_TEMPLATE _method("isVisibilityAllowed", &metagen::isVisibilityAllowed);

}

template <typename D_d >
void buildMetaClass_metagen_CppNamedItem(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    
    _d.CPGF_MD_TEMPLATE _base<metagen::CppItem >();

    _d.CPGF_MD_TEMPLATE _method("getName", &D_d::ClassType::getName), cpgf::MakePolicy<GMetaRuleCopyConstReference<-1> >();
    _d.CPGF_MD_TEMPLATE _method("getQualifiedName", &D_d::ClassType::getQualifiedName), cpgf::MakePolicy<GMetaRuleCopyConstReference<-1> >();
    _d.CPGF_MD_TEMPLATE _method("getQualifiedNameWithoutNamespace", &D_d::ClassType::getQualifiedNameWithoutNamespace), cpgf::MakePolicy<GMetaRuleCopyConstReference<-1> >();
    _d.CPGF_MD_TEMPLATE _method("isNamed", &D_d::ClassType::isNamed);

    _d.CPGF_MD_TEMPLATE _operator<metagen::CppNamedItem &(*)(cpgf::GMetaSelf, const metagen::CppNamedItem &)>(mopHolder = mopHolder, cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_CppNamedItem_opAssign_0<typename D_d::ClassType >);

}

template <typename D_d >
void buildMetaClass_metagen_CppItem(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    

    _d.CPGF_MD_TEMPLATE _method("getCategory", &D_d::ClassType::getCategory);
    _d.CPGF_MD_TEMPLATE _method("dump", &D_d::ClassType::dump)
        ._default(0)
    ;
    _d.CPGF_MD_TEMPLATE _method("getParent", &D_d::ClassType::getParent);
    _d.CPGF_MD_TEMPLATE _method("getNamedParent", &D_d::ClassType::getNamedParent);
    _d.CPGF_MD_TEMPLATE _method("isInMainFile", &D_d::ClassType::isInMainFile);
    _d.CPGF_MD_TEMPLATE _method("getIndexInCategory", &D_d::ClassType::getIndexInCategory);
    _d.CPGF_MD_TEMPLATE _method("getVisibility", &D_d::ClassType::getVisibility);
    _d.CPGF_MD_TEMPLATE _method("getProject", &D_d::ClassType::getProject);
    _d.CPGF_MD_TEMPLATE _method("isFile", &D_d::ClassType::isFile);
    _d.CPGF_MD_TEMPLATE _method("isNamespace", &D_d::ClassType::isNamespace);
    _d.CPGF_MD_TEMPLATE _method("isClass", &D_d::ClassType::isClass);
    _d.CPGF_MD_TEMPLATE _method("isConstructor", &D_d::ClassType::isConstructor);
    _d.CPGF_MD_TEMPLATE _method("isDestructor", &D_d::ClassType::isDestructor);
    _d.CPGF_MD_TEMPLATE _method("isEnum", &D_d::ClassType::isEnum);
    _d.CPGF_MD_TEMPLATE _method("isField", &D_d::ClassType::isField);
    _d.CPGF_MD_TEMPLATE _method("isMethod", &D_d::ClassType::isMethod);
    _d.CPGF_MD_TEMPLATE _method("isOperator", &D_d::ClassType::isOperator);
    _d.CPGF_MD_TEMPLATE _method("isInvokable", &D_d::ClassType::isInvokable);
    _d.CPGF_MD_TEMPLATE _method("isGlobal", &D_d::ClassType::isGlobal);
    _d.CPGF_MD_TEMPLATE _method("isNestedClass", &D_d::ClassType::isNestedClass);
    _d.CPGF_MD_TEMPLATE _method("isContainer", &D_d::ClassType::isContainer);
    _d.CPGF_MD_TEMPLATE _method("isNamed", &D_d::ClassType::isNamed);

    _d.CPGF_MD_TEMPLATE _operator<metagen::CppItem &(*)(cpgf::GMetaSelf, const metagen::CppItem &)>(mopHolder = mopHolder, cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_CppItem_opAssign_0<typename D_d::ClassType >);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_namespace_cppitem();
cpgf::GDefineMetaInfo createMetaClass_metagen_CppNamedItem();
cpgf::GDefineMetaInfo createMetaClass_metagen_CppItem();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
