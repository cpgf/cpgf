#include "builderenum.h"
#include "builderwriter.h"
#include "builderutil.h"
#include "codewriter/cppwriter.h"
#include "codewriter/codeblock.h"
#include "model/cppenum.h"

#include "Poco/Format.h"

#include <string>

using namespace std;

namespace metagen {


BuilderEnum::BuilderEnum(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icEnum);
}

BuilderEnum::~BuilderEnum()
{
}

const CppEnum * BuilderEnum::getCppEnum() const
{
	return static_cast<const CppEnum *>(this->getCppItem());
}

void BuilderEnum::doWriteMetaData(BuilderWriter * writer)
{
	const CppEnum * cppEnum = this->getCppEnum();
	CodeBlock * codeBlock = writer->getParentReflectionCodeBlock(cppEnum);

	string enumTypeName;
	string enumName;

	if(cppEnum->isAnonymous()) {
		enumName = Poco::format("AnonymousEnum_%d", cppEnum->getIndexInCategory());
		enumTypeName = "long long";
	}
	else {
		enumName = cppEnum->getName();
		enumTypeName = getReflectionScope(cppEnum, true) + enumName;
	}

	std::string s = Poco::format("%s<%s>(\"%s\")",
		writer->getReflectionAction("_enum"),
		enumTypeName,
		enumName
	);

	codeBlock->appendLine(s);

	string scope = getReflectionScope(cppEnum, false);
	CodeBlock * valueBlock = codeBlock->appendBlock(cbsIndent);
	const CppEnum::ValueListType * valueList = cppEnum->getValueList();
	for(CppEnum::ValueListType::const_iterator it = valueList->begin(); it != valueList->end(); ++it) {
		s = Poco::format("._element(\"%s\", %s%s)", it->getName(), scope, it->getName());
		valueBlock->appendLine(s);
	}
	codeBlock->appendLine(";");
}


} // namespace metagen
