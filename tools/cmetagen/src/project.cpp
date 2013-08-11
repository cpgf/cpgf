#include "project.h"
#include "buildermodel/buildertemplateinstantiation.h"
#include "exception.h"
#include "util.h"

#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/gscopedinterface.h"

#include "cpgf/scriptbind/gv8runner.h"

#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/Format.h"

using namespace std;
using namespace cpgf;

namespace metagen {

class ProjectImplement
{
public:
	ProjectImplement();

	void loadScriptFile(const string & fileName);
	void loadProject(Project * project);

private:
	void initialize();

private:
	GScopedPointer<GScriptRunner> runner;
	GScopedInterface<IMetaService> service;
	GScopedInterface<IScriptObject> scriptObject;
	GScopedInterface<IMetaClass> rootClass;
};

ProjectImplement::ProjectImplement()
{
	this->initialize();
}

void ProjectImplement::initialize()
{
	this->service.reset(createDefaultMetaService());
	this->runner.reset(createV8ScriptRunner(this->service.get()));
	this->scriptObject.reset(runner->getScripeObject());

	this->scriptObject->bindCoreService("cpgf", NULL);

	this->rootClass.reset(this->service->findClassByName("metadata"));
	scriptSetValue(this->scriptObject.get(), "metadata", GScriptValue::fromClass(rootClass.get()));
}

void ProjectImplement::loadScriptFile(const string & fileName)
{
	try {
		bool success = this->runner->executeFile(fileName.c_str());
		if(! success) {
			fatalError("Error in script file.");
		}
	}
	catch(const std::runtime_error & e) {
		fatalError(string("Error in script file. Message: ") + e.what());
	}
}

void ProjectImplement::loadProject(Project * project)
{
	GScriptValue configValue(scriptGetValue(this->scriptObject.get(), "config"));
	if(configValue.isNull()) {
		fatalError("\"config\" is required in project script.");
	}
	if(! configValue.isScriptObject()) {
		fatalError("\"config\" must be script object.");
	}

	GScopedInterface<IScriptObject> configObject(configValue.toScriptObject());

	GScopedInterface<IMetaClass> projectClass(this->rootClass->getClass("Project"));
	uint32_t fieldCount = projectClass->getFieldCount();
	for(uint32_t i = 0; i < fieldCount; ++i) {
		GScopedInterface<IMetaField> field(projectClass->getFieldAt(i));
		string fieldName(field->getName());
		GScriptValue value(scriptGetValue(configObject.get(), fieldName.c_str()));
		if(value.isNull()) {
			continue;
		}

		if(fieldName == "files") {
			if(! configObject->maybeIsScriptArray(fieldName.c_str())) {
				fatalError(Poco::format("Config \"%s\" must be array.", fieldName));
			}
			GScopedInterface<IScriptArray> files(scriptGetAsScriptArray(configObject.get(), fieldName.c_str()).toScriptArray());
			uint32_t count = files->getLength();
			for(uint32_t k = 0; k < count; ++k) {
				GScriptValue element(scriptGetScriptArrayValue(files.get(), k));
				if(! element.isString()) {
					fatalError(Poco::format("Elements in config \"%s\" must be string.", fieldName));
				}
				project->files.push_back(element.toString());
			}
			continue;
		}

		GMetaType metaType(metaGetItemType(field.get()));
		if(! metaType.isPointer()) {
			if(metaType.baseIsStdString()) {
				if(! value.isString()) {
					fatalError(Poco::format("Config \"%s\" must be string.", fieldName));
				}
				metaSetValue(field.get(), project, value.toString());
				continue;
			}
			if(vtIsInteger(metaType.getVariantType())) {
				if(! value.isFundamental()) {
					fatalError(Poco::format("Config \"%s\" must be primary type.", fieldName));
				}
				metaSetValue(field.get(), project, value.toFundamental());
				continue;
			}
		}
		fatalError(Poco::format("Internal error. Unhandled property \"%s\".", fieldName));
	}
}


Project::Project()
	:	projectID(""),
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
		mainRegisterFileName("registerMain_"),
		autoRegisterToGlobal(true),
		metaNamespace("metadata"),
		
		wrapOperator(true),
		wrapBitFields(true),

		allowPublic(true),
		allowProtected(false),
		allowPrivate(false),

		force(false),

		templateInstantiationRepository(new BuilderTemplateInstantiationRepository),

		implement(new ProjectImplement)
{
//maxItemCountPerFile = 5;
this->templateInstantiationRepository->add("ns1::TemplateA<int, 18>", "TemplateA_int", "TemplateA_wrapper_int");
this->headerOutputPath = "";
this->sourceOutputPath = headerOutputPath;
}

Project::~Project()
{
}

std::string Project::getProjectID() const
{
	return this->projectID;
}
const StringArrayType & Project::getFiles() const
{
	return this->files;
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

void Project::loadProject(const std::string & projectFileName)
{
	Poco::Path projectPath(projectFileName);
	projectPath.makeAbsolute();

	if(projectPath.isDirectory()) {
		fatalError(Poco::format("Project %s is directory. File needed.", projectFileName));
	}

	Poco::File projectFile(projectPath);
	if(! projectFile.exists()) {
		fatalError(Poco::format("Project %s doesn't exist.", projectFileName));
	}

	this->projectFileName = normalizeFile(projectPath.toString());
	this->projectRootPath = normalizePath(projectPath.parent().toString());

	this->implement->loadScriptFile(projectFileName);
	this->implement->loadProject(this);
}

std::string Project::getOutputHeaderFileName(const std::string & sourceFileName) const
{
	return this->doGetOutputFileName(sourceFileName, -1, false);
}

std::string Project::getOutputSourceFileName(const std::string & sourceFileName, int fileIndex) const
{
	return this->doGetOutputFileName(sourceFileName, fileIndex, true);
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
