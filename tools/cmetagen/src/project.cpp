#include "config.h"
#include "project.h"
#include "buildermodel/buildertemplateinstantiation.h"
#include "buildermodel/builderitem.h"
#include "exception.h"
#include "util.h"
#include "constants.h"

#include "model/cppnamespace.h"
#include "model/cppclass.h"
#include "model/cppfield.h"
#include "model/cppmethod.h"
#include "model/cppconstructor.h"
#include "model/cppdestructor.h"
#include "model/cppenum.h"
#include "model/cppoperator.h"
#include "model/cppcontext.h"
#include "model/cppfile.h"
#include "model/cpputil.h"
#include "model/cppsourcefile.h"

#include "buildermodel/buildernamespace.h"
#include "buildermodel/builderclass.h"
#include "buildermodel/builderfield.h"
#include "buildermodel/buildermethod.h"
#include "buildermodel/builderconstructor.h"
#include "buildermodel/builderdestructor.h"
#include "buildermodel/builderenum.h"
#include "buildermodel/builderoperator.h"
#include "buildermodel/buildercontext.h"
#include "buildermodel/builderfile.h"

#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/gscopedinterface.h"
#include "cpgf/scriptbind/gscriptbindutil.h"

#include "cpgf/scriptbind/gscriptrunner.h"
#if ENABLE_LUA
	#include "cpgf/scriptbind/gluarunner.h"
#endif
#if ENABLE_V8
	#include "cpgf/scriptbind/gv8runner.h"
#endif
#if ENABLE_PYTHON
	#include "cpgf/scriptbind/gpythonrunner.h"
#endif
#if ENABLE_SPIDERMONKEY
	#include "cpgf/scriptbind/gspidermonkeyrunner.h"
#endif

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4244)
#endif

#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/Format.h"
#include "Poco/String.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#if defined(_MSC_VER)
#pragma warning(disable: 4127)
#endif


using namespace std;
using namespace cpgf;

namespace metagen {

void projectParseFatalError(const string & message)
{
	fatalError(message);
}

class ProjectParserVisitor : public ProjectVisitor
{
public:
	ProjectParserVisitor(Project * project, const ProjectVisitorFilter & filter)
		: ProjectVisitor(project), filter(filter) {
	}

	virtual void visitStringArray(IMetaField * /*field*/, const std::string & fieldName, const GScriptValue & /*fieldValue*/,
		StringArrayType * stringArray) {
		if(! this->canVisit(fieldName)) {
			return;
		}

		if(! this->getScriptObject()->maybeIsScriptArray(fieldName.c_str())) {
			projectParseFatalError(Poco::format("Config \"%s\" must be array.", fieldName));
		}

		bool isFieldFiles = (fieldName == scriptFieldFiles);
		
		GScopedInterface<IScriptArray> files(scriptGetAsScriptArray(this->getScriptObject(), fieldName.c_str()).toScriptArray());
		uint32_t count = files->getLength();
		for(uint32_t i = 0; i < count; ++i) {
			GScriptValue element(scriptGetScriptArrayValue(files.get(), i));
			if(! element.isString()) {
				projectParseFatalError(Poco::format("Elements in config \"%s\" must be string.", fieldName));
			}
			string text = element.toString();
			if(isFieldFiles && Poco::Path(text).isAbsolute()) {
				projectParseFatalError(Poco::format("File %s in config must NOT be absolute path.", text));
			}
//			stringArray->push_back(text);
			appendFileNames(stringArray, text);
		}
	}

	virtual void visitTemplateInstantiations(IMetaField * /*field*/, const std::string & fieldName, const GScriptValue & /*fieldValue*/) {
		if(! this->canVisit(fieldName)) {
			return;
		}
		
		if(! this->getScriptObject()->maybeIsScriptArray(fieldName.c_str())) {
			projectParseFatalError(Poco::format("Config \"%s\" must be array.", fieldName));
		}
		
		GScopedInterface<IScriptArray> templateInstantiations(scriptGetAsScriptArray(this->getScriptObject(), fieldName.c_str()).toScriptArray());
		uint32_t count = templateInstantiations->getLength();
		for(uint32_t i = 0; i < count; ++i) {
			if(templateInstantiations->maybeIsScriptArray(i)) {
				GScopedInterface<IScriptArray> instantiation(scriptGetAsScriptArray(templateInstantiations.get(), i).toScriptArray());
				uint32_t nameCount = instantiation->getLength();
				if(nameCount < 2) {
					projectParseFatalError("Template instantiation must have at least two strings.");
				}
				if(nameCount > 3) {
					projectParseFatalError("Template instantiation must have at most three strings.");
				}
				string templateExpression;
				string reflectionName;
				string classWrapperReflectionName;
				for(uint32_t k = 0; k < nameCount; ++k) {
					GScriptValue element(scriptGetScriptArrayValue(instantiation.get(), k));
					if(! element.isString()) {
						projectParseFatalError("Elements in template instantiation must be string.");
					}

					switch(k) {
					case 0:
						templateExpression = element.toString();
						break;

					case 1:
						reflectionName = element.toString();
						break;

					case 2:
						classWrapperReflectionName = element.toString();
						break;
					}
				}
				this->getProject()->templateInstantiationRepository->add(templateExpression, reflectionName, classWrapperReflectionName);
			}
			else {
				projectParseFatalError(Poco::format("Config elements in \"%s\" must be array.", fieldName));
			}
		}
	}

	virtual void visitScriptCallback(IMetaField * /*field*/, const std::string & fieldName, const GScriptValue & fieldValue,
		cpgf::GSharedInterface<cpgf::IScriptFunction> * callback) {
		if(! this->canVisit(fieldName)) {
			return;
		}
		
		if(! fieldValue.isScriptFunction()) {
			projectParseFatalError(Poco::format("Config \"%s\" must be script function.", fieldName));
		}
		GScopedInterface<IScriptFunction> scriptFunction(fieldValue.toScriptFunction());
		callback->reset(scriptFunction.get());
	}

	virtual void visitString(IMetaField * field, const std::string & fieldName, const GScriptValue & fieldValue) {
		if(! this->canVisit(fieldName)) {
			return;
		}
		
		if(! fieldValue.isString()) {
			projectParseFatalError(Poco::format("Config \"%s\" must be string.", fieldName));
		}
		metaSetValue(field, this->getProject(), fieldValue.toString());
	}

	virtual void visitInteger(IMetaField * field, const std::string & fieldName, const GScriptValue & fieldValue) {
		if(! this->canVisit(fieldName)) {
			return;
		}
		
		if(! fieldValue.isFundamental()) {
			projectParseFatalError(Poco::format("Config \"%s\" must be primary type.", fieldName));
		}
		metaSetValue(field, this->getProject(), fieldValue.toFundamental());
	}

private:
	bool canVisit(const std::string & fieldName) {
		return this->filter(fieldName);
	}

private:
	ProjectVisitorFilter filter;
};

class ProjectImplement
{
public:
	ProjectImplement();
	~ProjectImplement();

	void loadScriptFile(const string & fileName);
	void loadProject(Project * project, const ProjectVisitorFilter & filter);

	IMetaClass * getProjectClass() const;

private:
	void initialize(const string & fileName);

	void visitProject(ProjectVisitor * visitor);

private:
	GScopedPointer<GScriptRunner> runner;
	GScopedInterface<IMetaService> service;
	GScopedInterface<IScriptObject> scriptObject;
	GScopedInterface<IMetaClass> rootClass;
};

ProjectImplement::ProjectImplement()
	: service(createDefaultMetaService())
{
	this->rootClass.reset(this->service->findClassByName("metadata"));
}

ProjectImplement::~ProjectImplement()
{
}

GScriptRunner * createScriptRunner(IMetaService * metaService, const string & fileName)
{
	(void)metaService;

	string extension = Poco::Path(fileName).getExtension();
	Poco::toLowerInPlace(extension);

#if ENABLE_LUA
	if(extension == "lua") {
		return createLuaScriptRunner(metaService);
	}
#endif

#if ENABLE_PYTHON
	if(extension == "py") {
		return createPythonScriptRunner(metaService);
	}
#endif

//	if(extension == "js")
	{
#if ENABLE_V8
		return createV8ScriptRunner(metaService);
#else
#if ENABLE_SPIDERMONKEY
		return createSpiderMonkeyScriptRunner(metaService);
#endif
#endif
	}
#if !HAS_SCRIPT_ENGINE
	return NULL;
#endif
}

void ProjectImplement::initialize(const string & fileName)
{
	if(! HAS_SCRIPT_ENGINE) {
		return;
	}

	this->runner.reset(createScriptRunner(this->service.get(), fileName));
	this->scriptObject.reset(runner->getScripeObject());

	scriptSetValue(this->scriptObject.get(), "metadata", GScriptValue::fromClass(rootClass.get()));

	this->scriptObject->bindCoreService("cpgf", NULL);
}

void ProjectImplement::loadScriptFile(const string & fileName)
{
	this->initialize(fileName);
	try {
		bool success = this->runner->executeFile(fileName.c_str());
		if(! success) {
			projectParseFatalError("Error in script file.");
		}
	}
	catch(const std::runtime_error & e) {
		projectParseFatalError(string("Error in script file. Message: ") + e.what());
	}
}

void ProjectImplement::visitProject(ProjectVisitor * visitor)
{
	GScriptValue configValue(scriptGetValue(this->scriptObject.get(), "config"));
	if(configValue.isNull()) {
		projectParseFatalError("\"config\" is required in project script.");
	}
	if(! configValue.isScriptObject()) {
		projectParseFatalError("\"config\" must be script object.");
	}

	GScopedInterface<IScriptObject> scriptObject(configValue.toScriptObject());
	visitor->setScriptObject(scriptObject.get());
	GScopedInterface<IMetaClass> projectClass(this->getProjectClass());
	metagen::visitProject(projectClass.get(), visitor);
}

IMetaClass * ProjectImplement::getProjectClass() const
{
	return this->rootClass->getClass("Project");
}

void visitProject(IMetaClass * projectClass, ProjectVisitor * visitor)
{
	GScriptValue value;
	uint32_t fieldCount = projectClass->getFieldCount();
	for(uint32_t i = 0; i < fieldCount; ++i) {
		GScopedInterface<IMetaField> field(projectClass->getFieldAt(i));
		string fieldName(field->getName());
		if(visitor->getScriptObject() != NULL) {
			value = scriptGetValue(visitor->getScriptObject(), fieldName.c_str());
			if(value.isNull()) {
				continue;
			}
		}

		if(fieldName == scriptFieldFiles) {
			visitor->visitStringArray(field.get(), fieldName, value, &visitor->getProject()->files);
			continue;
		}

		if(fieldName == scriptFieldTemplateInstantiations) {
			visitor->visitTemplateInstantiations(field.get(), fieldName, value);
			continue;
		}

		if(fieldName == scriptFieldFileCallback) {
			visitor->visitScriptCallback(field.get(), fieldName, value, &visitor->getProject()->fileCallback);
			continue;
		}

		if(fieldName == scriptFieldMainCallback) {
			visitor->visitScriptCallback(field.get(), fieldName, value, &visitor->getProject()->mainCallback);
			continue;
		}

		if(fieldName == scriptFieldHeaderReplaceCallback) {
			visitor->visitScriptCallback(field.get(), fieldName, value, &visitor->getProject()->headerReplaceCallback);
			continue;
		}

		GMetaType metaType(metaGetItemType(field.get()));
		if(! metaType.isPointer()) {
			if(metaType.baseIsStdString()) {
				visitor->visitString(field.get(), fieldName, value);
				continue;
			}
			if(vtIsInteger(metaType.getVariantType())) {
				visitor->visitInteger(field.get(), fieldName, value);
				continue;
			}
		}
		projectParseFatalError(Poco::format("Internal error. Unhandled property \"%s\".", fieldName));
	}
}

void ProjectImplement::loadProject(Project * project, const ProjectVisitorFilter & filter)
{
	if(! HAS_SCRIPT_ENGINE) {
		return;
	}
	ProjectParserVisitor visitor(project, filter);
	this->visitProject(&visitor);
}


Project::Project()
	:	projectID(""),
		sourceRootPath(""),

		clangOptions(),

		cppNamespace("metadata"),
		
		maxItemCountPerFile(0),
		
		headerIncludePrefix(""),
		
		headerFileExtension(".h"),
		sourceFileExtension(".cpp"),
		headerOutputPath("output/"),
		sourceOutputPath("output/"),
		targetFilePrefix("meta_"),
		
		includeExtensionInFileName(false),
		
		reflectionFunctionPrefix("buildMetaClass_"),
		creationFunctionPrefix("createMetaClass_"),
		metaDefineParamName("D_d"),
		classWrapperPostfix("Wrapper"),
		classWrapperSuperPrefix("super_"),

		mainRegisterFunctionName("registerMain_"),
		mainRegisterFileName("registerMain"),
		autoRegisterToGlobal(true),
		metaNamespace("metadata"),
		
		wrapOperator(true),
		wrapBitFields(true),

		allowPublic(true),
		allowProtected(false),
		allowPrivate(false),

		force(false),

		stopOnCompileError(false),

		templateInstantiationRepository(new BuilderTemplateInstantiationRepository),

		implement(new ProjectImplement)
{
}

Project::~Project()
{
}

std::string Project::getProjectID() const
{
	return this->projectID;
}

std::string Project::getSourceRootPath() const
{
	return this->sourceRootPath;
}

const StringArrayType & Project::getFiles() const
{
	return this->files;
}

const std::string & Project::getClangOptions() const
{
	return this->clangOptions;
}

const std::string & Project::getCppNamespace() const
{
	return this->cppNamespace;
}

size_t Project::getMaxItemCountPerFile() const
{
	return this->maxItemCountPerFile;
}

const std::string & Project::getHeaderIncludePrefix() const
{
	return this->headerIncludePrefix;
}
const std::string & Project::getHeaderFileExtension() const
{
	return this->headerFileExtension;
}

const std::string & Project::getSourceFileExtension() const
{
	return this->sourceFileExtension;
}

const std::string & Project::getHeaderOutputPath() const
{
	return this->headerOutputPath;
}

const std::string & Project::getSourceOutputPath() const
{
	return this->sourceOutputPath;
}

const std::string & Project::getTargetFilePrefix() const
{
	return this->targetFilePrefix;
}

bool Project::shouldIncludeExtensionInFileName() const
{
	return this->includeExtensionInFileName;
}

const std::string & Project::getReflectionFunctionPrefix() const
{
	return this->reflectionFunctionPrefix;
}

const std::string & Project::getCreationFunctionPrefix() const
{
	return this->creationFunctionPrefix;
}

const std::string & Project::getMainRegisterFunctionName() const
{
	return this->mainRegisterFunctionName;
}

const std::string & Project::getClassWrapperPostfix() const
{
	return this->classWrapperPostfix;
}

const std::string & Project::getClassWrapperSuperPrefix() const
{
	return this->classWrapperSuperPrefix;
}

const std::string & Project::getMainRegisterFileName() const
{
	return this->mainRegisterFileName;
}

const std::string & Project::getMetaDefineParamName() const
{
	return this->metaDefineParamName;
}

bool Project::shouldAutoRegisterToGlobal() const
{
	return this->autoRegisterToGlobal;
}

const std::string & Project::getMetaNamespace() const
{
	return this->metaNamespace;
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

bool Project::doesForce() const
{
	return this->force;
}

bool Project::shouldStopOnCompileError() const
{
	return this->stopOnCompileError;
}

const BuilderTemplateInstantiationRepository * Project::getTemplateInstantiationRepository() const
{
	return this->templateInstantiationRepository.get();
}

std::string Project::getAbsoluteFileName(const std::string & fileName) const
{
	return normalizeFile(Poco::Path(fileName).makeAbsolute(this->getProjectRootPath()).toString());
}

const std::string & Project::getProjectRootPath() const
{
	return this->projectRootPath;
}

void Project::loadProject(const std::string & projectFileName, const ProjectVisitorFilter & filter)
{
	Poco::Path projectPath(projectFileName);
	projectPath.makeAbsolute();

	if(! HAS_SCRIPT_ENGINE) {
		if(! projectFileName.empty()) {
			projectParseFatalError("The tool was not compiled with any script engine. --project can't be set.");
		}
		return;
	}

	if(projectPath.isDirectory()) {
		projectParseFatalError(Poco::format("Project %s is directory. File needed.", projectFileName));
	}

	Poco::File projectFile(projectPath);
	if(! projectFile.exists()) {
		projectParseFatalError(Poco::format("Project %s doesn't exist.", projectFileName));
	}

	this->projectFileName = normalizeFile(projectPath.toString());
	this->projectRootPath = normalizePath(projectPath.parent().toString());

	this->implement->loadScriptFile(projectPath.toString());
	this->implement->loadProject(this, filter);

	this->sourceRootPath = this->getAbsoluteFileName(this->sourceRootPath);
}

std::string Project::getOutputHeaderFileName(const std::string & sourceFileName) const
{
	return this->doGetOutputFileName(sourceFileName, -1, false);
}

std::string Project::getOutputSourceFileName(const std::string & sourceFileName, int fileIndex) const
{
	return this->doGetOutputFileName(sourceFileName, fileIndex, true);
}

void Project::processFileByScript(CppSourceFile * sourceFile) const
{
	if(this->fileCallback) {
		invokeScriptFunction(this->fileCallback.get(), sourceFile);
	}
}

void Project::processBuilderItemByScript(BuilderItem * builderItem) const
{
	if(! this->mainCallback) {
		return;
	}

	const CppItem * cppItem = builderItem->getCppItem();

#define INVOKE(category, type) \
		case category: \
		invokeScriptFunction(this->mainCallback.get(), static_cast<const Cpp ## type *>(cppItem), static_cast<Builder ## type *>(builderItem)); \
		break;

	switch(cppItem->getCategory()) {
		INVOKE(icFile, File);
		INVOKE(icNamespace, Namespace);
		INVOKE(icClass, Class);
		INVOKE(icEnum, Enum);
		INVOKE(icConstructor, Constructor);
		INVOKE(icDestructor, Destructor);
		INVOKE(icField, Field);
		INVOKE(icMethod, Method);
		INVOKE(icOperator, Operator);
	}

#undef INVOKE
}

std::string Project::replaceHeaderByScript(const std::string & fileName) const
{
	if(! this->headerReplaceCallback) {
		return fileName;
	}

	GVariant result = invokeScriptFunction(this->headerReplaceCallback.get(), fileName.c_str());
	return fromVariant<char *>(result);
}

IMetaClass * Project::getProjectClass() const
{
	return this->implement->getProjectClass();
}

void Project::visitProject(ProjectVisitor * visitor)
{
	GScopedInterface<IMetaClass> projectClass(this->implement->getProjectClass());
	metagen::visitProject(projectClass.get(), visitor);
}

std::string Project::doGetOutputFileName(const std::string & sourceFileName,
	int fileIndex, bool isSourceFile) const
{
	string postfix = "";
	if(fileIndex > 0) {
		postfix = Poco::format("_%d", fileIndex);
	}
	if(this->shouldIncludeExtensionInFileName()) {
		postfix = Poco::format("%s%s", Poco::Path(sourceFileName).getExtension(), postfix);
	}
	string extension;
	if(isSourceFile) {
		extension = this->getSourceFileExtension();
	}
	else {
		extension = this->getHeaderFileExtension();
	}
	if(! extension.empty() && extension[0] != '.') {
		extension = "." + extension;
	}

	string fileName = Poco::format("%s%s%s%s",
		this->getTargetFilePrefix(),
		Poco::Path(sourceFileName).getBaseName(),
		postfix,
		extension
	);
	string outputPath;
	if(isSourceFile) {
		outputPath = this->getSourceOutputPath();
	}
	else {
		outputPath = this->getHeaderOutputPath();
	}
	return this->getAbsoluteFileName(normalizePath(outputPath) + fileName);
}


} // namespace metagen
