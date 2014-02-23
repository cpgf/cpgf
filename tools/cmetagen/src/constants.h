#ifndef CPGF_CONSTANTS_H
#define CPGF_CONSTANTS_H

#include <string>

namespace metagen {

const std::string ParserPredefinedMacro = "CPGF_METAGEN_PARSER";

const std::string GeneratedFileMark = "/*@@cpgf@@*/";
const std::string GeneratedCreationFunctionBeginMark = "/*@cpgf@creations@@";
const std::string GeneratedCreationFunctionEndMark = "@@cpgf@creations@*/";

extern const char * metaHeaderIncludeList[];
const std::string includeMetaDataFooter("cpgf/metadata/private/gmetadata_footer.h");
const std::string includeMetaDefine("cpgf/gmetadefine.h");
const std::string includeOutmain("cpgf/goutmain.h");

const std::string metaTypeTypeDefName("MetaType");

const std::string scriptFieldFiles("files");
const std::string scriptFieldTemplateInstantiations("templateInstantiations");
const std::string scriptFieldFileCallback("fileCallback");
const std::string scriptFieldMainCallback("mainCallback");
const std::string scriptFieldHeaderReplaceCallback("headerReplaceCallback");

} // namespace metagen


#endif
