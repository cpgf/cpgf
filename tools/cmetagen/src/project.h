#ifndef __PROJECT_H
#define __PROJECT_H

#include "cpgf/gscopedptr.h"

#include <string>

namespace metagen {

class BuilderTemplateInstantiationRepository;

class Project
{
public:
	Project();
	~Project();

	size_t getMaxItemCountPerFile() const;
	
	const std::string & getCppNamespace() const;

	const std::string & getHeaderFileExtension() const;
	const std::string & getSourceFileExtension() const;
	const std::string & getHeaderOutputPath() const;
	const std::string & getSourceOutputPath() const;
	const std::string & getTargetFilePrefix() const;
	
	bool shouldIncludeExtensionInFileName() const;
	
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
	
	const BuilderTemplateInstantiationRepository * getTemplateInstantiationRepository() const;

public:	
	std::string getAbsoluteFileName(const std::string & fileName) const;
	const std::string & getProjectRootPath() const;

	void loadProject(const std::string & projectFileName);

private:
	size_t maxItemCountPerFile;

	std::string cppNamespace;
	
	std::string headerFileExtension;
	std::string sourceFileExtension;
	std::string headerOutputPath;
	std::string sourceOutputPath;
	std::string targetFilePrefix;
	
	bool includeExtensionInFileName;

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
	
	cpgf::GScopedPointer<BuilderTemplateInstantiationRepository> templateInstantiationRepository;
	std::string projectFileName;
	std::string projectRootPath;
};


} // namespace metagen


#endif
