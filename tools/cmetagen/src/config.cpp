#include "config.h"


Config::Config()
	:	maxItemCountPerFile(0),
		reflectionFunctionPrefix("buildMetaClass_"),
		creationFunctionPrefix("createMetaClass_"),
		mainRegisterFunctionPrefix("registerMain_"),
		metaDefineParamName("D_d"),
		wrapOperator(true),
		wrapBitFields(true),
		allowPublic(true),
		allowProtected(false),
		allowPrivate(false)
{
maxItemCountPerFile = 5;
}

size_t Config::getMaxItemCountPerFile() const
{
	return this->maxItemCountPerFile;
}

const std::string & Config::getReflectionFunctionPrefix() const
{
	return this->reflectionFunctionPrefix;
}

const std::string & Config::getCreationFunctionPrefix() const
{
	return this->creationFunctionPrefix;
}

const std::string & Config::getMainRegisterFunctionPrefix() const
{
	return this->mainRegisterFunctionPrefix;
}

const std::string & Config::getMetaDefineParamName() const
{
	return this->metaDefineParamName;
}

bool Config::shouldWrapOperator() const
{
	return this->wrapOperator;
}

bool Config::shouldWrapBitFields() const
{
	return this->wrapBitFields;
}

bool Config::doesAllowPublic() const
{
	return this->allowPublic;
}

bool Config::doesAllowProtected() const
{
	return this->allowProtected;
}

bool Config::doesAllowPrivate() const
{
	return this->allowPrivate;
}
