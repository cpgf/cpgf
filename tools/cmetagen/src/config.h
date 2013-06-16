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
			mainRegisterFunctionPrefix("registerMain_"),
			allowPublic(true),
			allowProtected(false),
			allowPrivate(false)
	{
maxItemCountPerFile = 3;
	}
	
	size_t getMaxItemCountPerFile() const { return this->maxItemCountPerFile; }
	const std::string & getReflectionFunctionPrefix() const { return this->reflectionFunctionPrefix; }
	const std::string & getCreationFunctionPrefix() const { return this->creationFunctionPrefix; }
	const std::string & getMainRegisterFunctionPrefix() const { return this->mainRegisterFunctionPrefix; }
	
	bool isAllowPublic() const { return this->allowPublic; }
	bool isAllowProtected() const { return this->allowProtected; }
	bool isAllowPrivate() const { return this->allowPrivate; }

private:
	size_t maxItemCountPerFile;
	std::string reflectionFunctionPrefix;
	std::string creationFunctionPrefix;
	std::string mainRegisterFunctionPrefix;
	bool allowPublic;
	bool allowProtected;
	bool allowPrivate;
};


#endif
