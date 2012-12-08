// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IRRXML_H
#define __META_IRRLICHT_IRRXML_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::io;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_irrxml(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("createIrrXMLReader", (IrrXMLReader* (*) (const char *))&createIrrXMLReader);
    _d.CPGF_MD_TEMPLATE _method("createIrrXMLReader", (IrrXMLReader* (*) (FILE *))&createIrrXMLReader);
    _d.CPGF_MD_TEMPLATE _method("createIrrXMLReader", (IrrXMLReader* (*) (IFileReadCallBack *, bool))&createIrrXMLReader)
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("createIrrXMLReaderUTF16", (IrrXMLReaderUTF16* (*) (const char *))&createIrrXMLReaderUTF16);
    _d.CPGF_MD_TEMPLATE _method("createIrrXMLReaderUTF16", (IrrXMLReaderUTF16* (*) (FILE *))&createIrrXMLReaderUTF16);
    _d.CPGF_MD_TEMPLATE _method("createIrrXMLReaderUTF16", (IrrXMLReaderUTF16* (*) (IFileReadCallBack *, bool))&createIrrXMLReaderUTF16)
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("createIrrXMLReaderUTF32", (IrrXMLReaderUTF32* (*) (const char *))&createIrrXMLReaderUTF32);
    _d.CPGF_MD_TEMPLATE _method("createIrrXMLReaderUTF32", (IrrXMLReaderUTF32* (*) (FILE *))&createIrrXMLReaderUTF32);
    _d.CPGF_MD_TEMPLATE _method("createIrrXMLReaderUTF32", (IrrXMLReaderUTF32* (*) (IFileReadCallBack *, bool))&createIrrXMLReaderUTF32)
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _enum<ETEXT_FORMAT>("ETEXT_FORMAT")
        ._element("ETF_ASCII", irr::io::ETF_ASCII)
        ._element("ETF_UTF8", irr::io::ETF_UTF8)
        ._element("ETF_UTF16_BE", irr::io::ETF_UTF16_BE)
        ._element("ETF_UTF16_LE", irr::io::ETF_UTF16_LE)
        ._element("ETF_UTF32_BE", irr::io::ETF_UTF32_BE)
        ._element("ETF_UTF32_LE", irr::io::ETF_UTF32_LE)
    ;
    _d.CPGF_MD_TEMPLATE _enum<EXML_NODE>("EXML_NODE")
        ._element("EXN_NONE", irr::io::EXN_NONE)
        ._element("EXN_ELEMENT", irr::io::EXN_ELEMENT)
        ._element("EXN_ELEMENT_END", irr::io::EXN_ELEMENT_END)
        ._element("EXN_TEXT", irr::io::EXN_TEXT)
        ._element("EXN_COMMENT", irr::io::EXN_COMMENT)
        ._element("EXN_CDATA", irr::io::EXN_CDATA)
        ._element("EXN_UNKNOWN", irr::io::EXN_UNKNOWN)
    ;
}


template <typename D>
void buildMetaClass_IFileReadCallBack(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("read", &D::ClassType::read);
    _d.CPGF_MD_TEMPLATE _method("getSize", &D::ClassType::getSize);
}


template <typename D, class char_type, class super_class>
void buildMetaClass_IIrrXMLReader(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("read", &D::ClassType::read);
    _d.CPGF_MD_TEMPLATE _method("getNodeType", &D::ClassType::getNodeType);
    _d.CPGF_MD_TEMPLATE _method("getAttributeCount", &D::ClassType::getAttributeCount);
    _d.CPGF_MD_TEMPLATE _method("getAttributeName", &D::ClassType::getAttributeName);
    _d.CPGF_MD_TEMPLATE _method("getAttributeValue", (const char_type * (D::ClassType::*) (int) const)&D::ClassType::getAttributeValue);
    _d.CPGF_MD_TEMPLATE _method("getAttributeValue", (const char_type * (D::ClassType::*) (const char_type *) const)&D::ClassType::getAttributeValue);
    _d.CPGF_MD_TEMPLATE _method("getAttributeValueSafe", &D::ClassType::getAttributeValueSafe);
    _d.CPGF_MD_TEMPLATE _method("getAttributeValueAsInt", (int (D::ClassType::*) (const char_type *) const)&D::ClassType::getAttributeValueAsInt);
    _d.CPGF_MD_TEMPLATE _method("getAttributeValueAsInt", (int (D::ClassType::*) (int) const)&D::ClassType::getAttributeValueAsInt);
    _d.CPGF_MD_TEMPLATE _method("getAttributeValueAsFloat", (float (D::ClassType::*) (const char_type *) const)&D::ClassType::getAttributeValueAsFloat);
    _d.CPGF_MD_TEMPLATE _method("getAttributeValueAsFloat", (float (D::ClassType::*) (int) const)&D::ClassType::getAttributeValueAsFloat);
    _d.CPGF_MD_TEMPLATE _method("getNodeName", &D::ClassType::getNodeName);
    _d.CPGF_MD_TEMPLATE _method("getNodeData", &D::ClassType::getNodeData);
    _d.CPGF_MD_TEMPLATE _method("isEmptyElement", &D::ClassType::isEmptyElement);
    _d.CPGF_MD_TEMPLATE _method("getSourceFormat", &D::ClassType::getSourceFormat);
    _d.CPGF_MD_TEMPLATE _method("getParserFormat", &D::ClassType::getParserFormat);
}


template <typename D>
void buildMetaClass_IXMLBase(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
}


template <typename T>
inline void opErAToRWrapper_xmlChar__opAssign(xmlChar<T> * self, int t) {
    (*self) = t;
}


template <typename D, typename T>
void buildMetaClass_XmlChar(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (char)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (wchar_t)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (unsigned char)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (unsigned short)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (unsigned int)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (unsigned long)>();
    _d.CPGF_MD_TEMPLATE _field("c", &D::ClassType::c);
    _d.CPGF_MD_TEMPLATE _operator< T (cpgf::GMetaSelf)>(mopHolder());
    _d.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf, int)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAssign", (void (*) (xmlChar<T> *, int))&opErAToRWrapper_xmlChar__opAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
