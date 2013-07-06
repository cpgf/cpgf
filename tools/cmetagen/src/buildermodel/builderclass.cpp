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

const CppClass * BuilderClass::getCppClass() const
{
	return static_cast<const CppClass *>(this->getCppItem());
}

void BuilderClass::doWriteMetaData(BuilderFileWriter * writer)
{
}

