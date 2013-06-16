#include "builderclass.h"
#include "builderfilewriter.h"
#include "model/cppclass.h"
#include "codewriter/cppwriter.h"
#include "util.h"

#include "Poco/Format.h"

#include <string>

using namespace std;


BuilderClass::BuilderClass(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icClass);
}

BuilderClass::~BuilderClass()
{
}

void BuilderClass::doWriteMetaData(BuilderFileWriter * writer)
{
	const CppClass * cppClass = static_cast<const CppClass *>(this->getCppItem());
	writer->requireItemConainerFunction(cppClass);
}

