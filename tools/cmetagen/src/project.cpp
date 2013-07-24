#include "project.h"
#include "buildermodel/buildertemplateinstantiation.h"

namespace metagen {


Project::Project()
	:	maxItemCountPerFile(0),
		
		headerFileExtension(".h"),
		sourceFileExtension(".cpp"),
		headerOutputPath("output/"),
		sourceOutputPath("output/"),
		
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

		templateInstantiationRepository(new BuilderTemplateInstantiationRepository)
{
maxItemCountPerFile = 5;
this->templateInstantiationRepository->add("ns1::TemplateA<int, 18>", "TemplateA_int", "TemplateA_wrapper_int");
}

Project::~Project()
{
}

bool Project::shouldSplitFile() const
{
	return this->getMaxItemCountPerFile() > 0;
}

size_t Project::getMaxItemCountPerFile() const
{
	return this->maxItemCountPerFile;
}

const std::string & Project::getHeaderFileExtension()
{
	return this->headerFileExtension;
}

const std::string & Project::getSourceFileExtension()
{
	return this->sourceFileExtension;
}

const std::string & Project::getHeaderOutputPath()
{
	return this->headerOutputPath;
}

const std::string & Project::getSourceOutputPath()
{
	return this->sourceOutputPath;
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

const BuilderTemplateInstantiationRepository * Project::getTemplateInstantiationRepository() const
{
	return this->templateInstantiationRepository.get();
}


} // namespace metagen
