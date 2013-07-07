#include "buildermethod.h"
#include "builderfilewriter.h"
#include "codewriter/cppwriter.h"
#include "model/cppmethod.h"
#include "builderutil.h"

#include "Poco/Format.h"

using namespace std;


BuilderMethod::BuilderMethod(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icMethod);
}

BuilderMethod::~BuilderMethod()
{
}

const CppMethod * BuilderMethod::getCppMethod() const
{
	return static_cast<const CppMethod *>(this->getCppItem());
}

void BuilderMethod::doWriteMetaData(BuilderFileWriter * writer)
{
	this->doWriterReflection(writer);
}

void BuilderMethod::doWriterReflection(BuilderFileWriter * writer)
{
	const CppMethod * cppMethod = this->getCppMethod();
	CodeBlock * codeBlock = writer->getReflectionCodeBlock(cppMethod);

	size_t arity = cppMethod->getArity();
	bool hasDefaultValue = (arity > 0 && cppMethod->paramHasDefaultValue(arity - 1));

	std::string s;

	if(cppMethod->isOverloaded()) {
		s = Poco::format("%s(\"%s\", (%s)(&%s%s))",
			writer->getReflectionAction("_method"),
			cppMethod->getName(),
			cppMethod->getTextOfPointeredType(),
			getReflectionScope(cppMethod),
			cppMethod->getName(),
			string(hasDefaultValue ? "" : ";")
		);
	}
	else {
		s = Poco::format("%s(\"%s\", &%s%s)",
			writer->getReflectionAction("_method"),
			cppMethod->getName(),
			getReflectionScope(cppMethod),
			cppMethod->getName(),
			string(hasDefaultValue ? "" : ";")
		);
	}
	codeBlock->appendLine(s);

	if(hasDefaultValue) {
		CodeBlock * defaultValueBlock = codeBlock->appendBlock(cbsIndent);
		while(arity != 0) {
			--arity;
			if(! cppMethod->paramHasDefaultValue(arity)) {
				break;
			}

			string defaultValue = cppMethod->getTextOfParamDeafultValue(arity);
			bool shouldSafeCopy = false;
			CppType type = cppMethod->getParamType(arity).getNonReferenceType();
			if(! type.isPointer()) {
				if(! type.isFundamental()) {
					shouldSafeCopy = true;
				}
			}
			if(shouldSafeCopy) {
				s = Poco::format("._default(copyVariantFromCopyable(%s))", defaultValue);
			}
			else {
				s = Poco::format("._default(%s)", defaultValue);
			}
			defaultValueBlock->appendLine(s);
		}
		
		codeBlock->appendLine(";");
	}
}

