#ifndef __CONSTANTS_H
#define __CONSTANTS_H

#include <string>

namespace metagen {

const std::string ParserPredefinedMacro = "CPGF_METAGEN_PARSER";

const std::string includeMetaDefine("cpgf/gmetadefine.h");
const std::string includeMetaDataHeader("cpgf/metadata/private/gmetadata_header.h");
const std::string includeMetaDataFooter("cpgf/metadata/private/gmetadata_footer.h");
const std::string includeMetaPolicy("cpgf/gmetapolicy.h");
const std::string includeScriptBindUtil("cpgf/scriptbind/gscriptbindutil.h");
const std::string includeScriptWrapper("cpgf/scriptbind/gscriptwrapper.h");
const std::string includeScopedInterface("cpgf/gscopedinterface.h");
const std::string includeOutmain("cpgf/goutmain.h");
const std::string includeSelectFunctionByArity("cpgf/gselectFunctionByArity.h");

const std::string metaTypeTypeDefName("MetaType");

const std::string scriptFieldFiles("files");
const std::string scriptFieldTemplateInstantiations("templateInstantiations");

} // namespace metagen


#endif
