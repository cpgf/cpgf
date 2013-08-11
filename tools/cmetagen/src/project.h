#ifndef __PROJECT_H
#define __PROJECT_H

#include "cpgf/gscopedptr.h"
#include "cpgf/gclassutil.h"

#include <string>
#include <vector>

namespace metadata {
template <typename D_d >
void buildMetaClass_metagen_Project(D_d & _d);
}

namespace metagen {

class BuilderTemplateInstantiationRepository;

typedef std::vector<std::string> StringArrayType;

class ProjectImplement;

class Project
{
public:
	Project();
	~Project();

#ifdef CPGF_METAGEN_PARSER
private:
#else
public:
#endif
	std::string getProjectID() const;

	const StringArrayType & getFiles() const;
	const std::string & getCppNamespace() const;

	size_t getMaxItemCountPerFile() const;

	const std::string & getHeaderIncludePrefix() const;
	const std::string & getHeaderFileExtension() const;
	const std::string & getSourceFileExtension() const;
	const std::string & getHeaderOutputPath() const;
	const std::string & getSourceOutputPath() const;
	const std::string & getTargetFilePrefix() const;
	
	bool shouldIncludeExtensionInFileName() const;
	
	const std::string & getReflectionFunctionPrefix() const;
	const std::string & getCreationFunctionPrefix() const;
	const std::string & getMetaDefineParamName() const;
	const std::string & getClassWrapperPostfix() const;
	const std::string & getClassWrapperSuperPrefix() const;

	const std::string & getMainRegisterFunctionName() const;
	const std::string & getMainRegisterFileName() const;
	bool shouldAutoRegisterToGlobal() const;
	const std::string & getMetaNamespace() const;
	
	bool shouldWrapOperator() const;
	bool shouldWrapBitFields() const;

	bool doesAllowPublic() const;
	bool doesAllowProtected() const;
	bool doesAllowPrivate() const;

	bool doesForce() const;
	
	const BuilderTemplateInstantiationRepository * getTemplateInstantiationRepository() const;

#ifdef CPGF_METAGEN_PARSER
private:
#else
public:
#endif
	std::string getAbsoluteFileName(const std::string & fileName) const;
	const std::string & getProjectRootPath() const;

	void loadProject(const std::string & projectFileName);

	std::string getOutputHeaderFileName(const std::string & sourceFileName) const;
	std::string getOutputSourceFileName(const std::string & sourceFileName, int fileIndex) const;

private:
	std::string doGetOutputFileName(const std::string & sourceFileName,
		int fileIndex, bool isSourceFile) const;

#ifdef CPGF_METAGEN_PARSER
public:
#else
private:
#endif
	std::string projectID;

	StringArrayType files;
	std::string cppNamespace;
	
	size_t maxItemCountPerFile;

	std::string headerIncludePrefix;
	std::string headerFileExtension;
	std::string sourceFileExtension;
	std::string headerOutputPath;
	std::string sourceOutputPath;
	std::string targetFilePrefix;
	
	bool includeExtensionInFileName;

	std::string reflectionFunctionPrefix;
	std::string creationFunctionPrefix;
	std::string metaDefineParamName;
	std::string classWrapperPostfix;
	std::string classWrapperSuperPrefix;

	std::string mainRegisterFunctionName;
	std::string mainRegisterFileName;
	bool autoRegisterToGlobal;
	std::string metaNamespace;

	bool wrapOperator;
	bool wrapBitFields;

	bool allowPublic;
	bool allowProtected;
	bool allowPrivate;

	bool force;

private:
	cpgf::GScopedPointer<BuilderTemplateInstantiationRepository> templateInstantiationRepository;
	std::string projectFileName;
	std::string projectRootPath;

	cpgf::GScopedPointer<ProjectImplement> implement;

private:
	friend class ProjectImplement;

	template <typename D_d >
	friend void metadata::buildMetaClass_metagen_Project(D_d & _d);

	GMAKE_NONCOPYABLE(Project);
};


} // namespace metagen


#endif
