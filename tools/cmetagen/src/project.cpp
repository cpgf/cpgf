#include "project.h"
#include "buildermodel/buildertemplateinstantiation.h"
#include "exception.h"
#include "util.h"

#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/Format.h"

using namespace std;


namespace metagen {

Project::Project()
	:	maxItemCountPerFile(0),
		
		cppNamespace("metadata"),
		
		headerIncludePrefix(""),
		
		headerFileExtension(".h"),
		sourceFileExtension(".cpp"),
		headerOutputPath("output/"),
		sourceOutputPath("output/"),
		targetFilePrefix("meta_"),
		
		includeExtensionInFileName(false),
		
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
		allowPrivate(false),

		force(false),

		templateInstantiationRepository(new BuilderTemplateInstantiationRepository)
{
//maxItemCountPerFile = 5;
this->templateInstantiationRepository->add("ns1::TemplateA<int, 18>", "TemplateA_int", "TemplateA_wrapper_int");
this->files.push_back("z.h");
this->headerOutputPath = "";
this->sourceOutputPath = headerOutputPath;
this->force = true;
}

Project::~Project()
{
}

const StringArrayType & Project::getFiles() const
{
	return this->files;
}

size_t Project::getMaxItemCountPerFile() const
{
	return this->maxItemCountPerFile;
}

const std::string & Project::getCppNamespace() const
{
	return this->cppNamespace;
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
