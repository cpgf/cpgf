#include "project.h"

namespace metagen {


Project::Project()
	:	maxItemCountPerFile(0),
		reflectionFunctionPrefix("buildMetaClass_"),
		creationFunctionPrefix("createMetaClass_"),
		mainRegisterFunctionPrefix("registerMain_"),
		metaDefineParamName("D_d"),
		classWrapperPostfix("Wrapper"),
		classWrapperSuperPrefix("super_"),
		wrapOperator(true),
		wrapBitFields(true),
		allowPublic(true),
		allowProtected(false),
		allowPrivate(false)
{
maxItemCountPerFile = 5;
}

bool Project::shouldSplitFile() const
{
	return this->getMaxItemCountPerFile() > 0;
}

size_t Project::getMaxItemCountPerFile() const
{
	return this->maxItemCountPerFile;
}

const std::string & Project::getReflectionFunctionPrefix() const
{
	return this->reflectionFunctionPrefix;
}

const std::string & Project::getCreationFunctionPrefix() const
{
	return this->creationFunctionPrefix;
}

const std::string & Project::getMainRegisterFunctionPrefix() const
{
	return this->mainRegisterFunctionPrefix;
}

const std::string & Project::getMetaDefineParamName() const
{
	return this->metaDefineParamName;
}

const std::string & Project::getClassWrapperPostfix() const
{
	return this->classWrapperPostfix;
}

const std::string & Project::getClassWrapperSuperPrefix() const
{
	return this->classWrapperSuperPrefix;
}

bool Project::shouldWrapOperator() const
{
	return this->wrapOperator;
}

bool Project::shouldWrapBitFields() const
{
	return this->wrapBitFields;
}

bool Project::doesAllowPublic() const
{
	return this->allowPublic;
}

bool Project::doesAllowProtected() const
{
	return this->allowProtected;
}

bool Project::doesAllowPrivate() const
{
	return this->allowPrivate;
}


} // namespace metagen
