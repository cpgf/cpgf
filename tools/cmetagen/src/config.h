#ifndef __CONFIG_H
#define __CONFIG_H

#include <string>

namespace metagen {


class Config
{
public:
	Config();

	size_t getMaxItemCountPerFile() const;
	const std::string & getReflectionFunctionPrefix() const;
	const std::string & getCreationFunctionPrefix() const;
	const std::string & getMainRegisterFunctionPrefix() const;
	const std::string & getMetaDefineParamName() const;
	const std::string & getClassWrapperPostfix() const;
	const std::string & getClassWrapperSuperPrefix() const;

	bool shouldWrapOperator() const;
	bool shouldWrapBitFields() const;

	bool doesAllowPublic() const;
	bool doesAllowProtected() const;
	bool doesAllowPrivate() const;

private:
	size_t maxItemCountPerFile;
	std::string reflectionFunctionPrefix;
	std::string creationFunctionPrefix;
	std::string mainRegisterFunctionPrefix;
	std::string metaDefineParamName;
	std::string classWrapperPostfix;
	std::string classWrapperSuperPrefix;

	bool wrapOperator;
	bool wrapBitFields;

	bool allowPublic;
	bool allowProtected;
	bool allowPrivate;
};


} // namespace metagen


#endif
