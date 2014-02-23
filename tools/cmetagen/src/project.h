#ifndef CPGF_PROJECT_H
#define CPGF_PROJECT_H

#include "cpgf/gscopedptr.h"
#include "cpgf/gsharedinterface.h"
#include "cpgf/gclassutil.h"
#include "cpgf/gcallback.h"

#include <string>
#include <vector>

namespace metadata {
template <typename D_d >
void buildMetaClass_metagen_Project(D_d & _d);
}

namespace cpgf {
struct IScriptFunction;
struct IScriptObject;
struct IMetaField;
struct IMetaClass;
class GScriptValue;
}

namespace metagen {

typedef cpgf::GCallback<bool (const std::string &)> ProjectVisitorFilter;

typedef std::vector<std::string> StringArrayType;

class BuilderTemplateInstantiationRepository;
class BuilderItem;

class CppSourceFile;

class ProjectImplement;
class ProjectVisitor;

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
	std::string getSourceRootPath() const;

	const StringArrayType & getFiles() const;
	const std::string & getClangOptions() const;

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

	bool shouldStopOnCompileError() const;
	
	const BuilderTemplateInstantiationRepository * getTemplateInstantiationRepository() const;

#ifdef CPGF_METAGEN_PARSER
private:
#else
public:
#endif
	std::string getAbsoluteFileName(const std::string & fileName) const;
	const std::string & getProjectRootPath() const;

	void loadProject(const std::string & projectFileName, const ProjectVisitorFilter & filter);

	std::string getOutputHeaderFileName(const std::string & sourceFileName) const;
	std::string getOutputSourceFileName(const std::string & sourceFileName, int fileIndex) const;

	void processFileByScript(CppSourceFile * sourceFile) const;
	void processBuilderItemByScript(BuilderItem * builderItem) const;
	std::string replaceHeaderByScript(const std::string & fileName) const;

	cpgf::IMetaClass * getProjectClass() const;
	void visitProject(ProjectVisitor * visitor);

private:
	std::string doGetOutputFileName(const std::string & sourceFileName,
		int fileIndex, bool isSourceFile) const;

#ifdef CPGF_METAGEN_PARSER
public:
#else
private:
#endif
	std::string projectID;
	std::string sourceRootPath;

	StringArrayType files;
	std::string clangOptions;

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

	bool stopOnCompileError;

	// prototype: bool (filename); return false to skip the file
	cpgf::GSharedInterface<cpgf::IScriptFunction> fileCallback;

	// prototype: void (const T * cppItem, BuilderItem * data);
	// T can any class derived from CppItem, such as CppClass, CppEnum
	cpgf::GSharedInterface<cpgf::IScriptFunction> mainCallback;

	// prototype: string (string originalHeaderFullFileName);
	cpgf::GSharedInterface<cpgf::IScriptFunction> headerReplaceCallback;

	// dumb field to be used by script parser
	int templateInstantiations;

private:
	cpgf::GScopedPointer<BuilderTemplateInstantiationRepository> templateInstantiationRepository;
	std::string projectFileName;
	std::string projectRootPath;

	cpgf::GScopedPointer<ProjectImplement> implement;

private:
	friend class ProjectImplement;
	friend class ProjectParserVisitor;
	friend class CommandLineParser;
	friend void visitProject(cpgf::IMetaClass * projectClass, ProjectVisitor * visitor);

	template <typename D_d >
	friend void metadata::buildMetaClass_metagen_Project(D_d & _d);

	GMAKE_NONCOPYABLE(Project);
};


class ProjectVisitor
{
public:
	explicit ProjectVisitor(Project * project)
		: project(project), scriptObject(NULL) {
	}

	virtual ~ProjectVisitor() {}

	virtual void visitStringArray(cpgf::IMetaField * field, const std::string & fieldName, const cpgf::GScriptValue & fieldValue,
		StringArrayType * stringArray) = 0;
	virtual void visitTemplateInstantiations(cpgf::IMetaField * field, const std::string & fieldName, const cpgf::GScriptValue & fieldValue) = 0;
	virtual void visitScriptCallback(cpgf::IMetaField * field, const std::string & fieldName, const cpgf::GScriptValue & fieldValue,
		cpgf::GSharedInterface<cpgf::IScriptFunction> * callback) = 0;
	virtual void visitString(cpgf::IMetaField * field, const std::string & fieldName, const cpgf::GScriptValue & fieldValue) = 0;
	virtual void visitInteger(cpgf::IMetaField * field, const std::string & fieldName, const cpgf::GScriptValue & fieldValue) = 0;

	Project * getProject() const { return this->project; }
	cpgf::IScriptObject * getScriptObject() const { return this->scriptObject; }
	void setScriptObject(cpgf::IScriptObject * scriptObject) { this->scriptObject = scriptObject; }

private:
	Project * project;
	cpgf::IScriptObject * scriptObject;
};

void visitProject(cpgf::IMetaClass * projectClass, ProjectVisitor * visitor);



} // namespace metagen


#endif
