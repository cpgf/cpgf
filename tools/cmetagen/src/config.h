#ifndef __CONFIG_H
#define __CONFIG_H

#include <string>


class Config
{
public:
	Config()
		:	maxItemCountPerFile(0),
			reflectionFunctionPrefix("buildMetaClass_"),
			creationFunctionPrefix("createMetaClass_"),
			mainRegisterFunctionPrefix("registerMain_")
	{}
	
	size_t getMaxItemCountPerFile() const { return this->maxItemCountPerFile; }
	const std::string & getReflectionFunctionPrefix() const { return this->reflectionFunctionPrefix; }
	const std::string & getCreationFunctionPrefix() const { return this->creationFunctionPrefix; }
	const std::string & getMainRegisterFunctionPrefix() const { return this->mainRegisterFunctionPrefix; }

private:
	size_t maxItemCountPerFile;
	std::string reflectionFunctionPrefix;
	std::string creationFunctionPrefix;
	std::string mainRegisterFunctionPrefix;
};


#endif
