#include "buildertemplateinstantiation.h"
#include "model/cppclass.h"
#include "util.h"
#include "exception.h"

#include "cpgf/gassert.h"

#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/StringTokenizer.h"

using namespace std;

namespace metagen {


BuilderTemplateInstantiation::BuilderTemplateInstantiation(const std::string & templateExpression,
	const std::string & reflectionName, const std::string & classWrapperReflectionName)
	: reflectionName(reflectionName), classWrapperReflectionName(classWrapperReflectionName)
{
	this->parse(templateExpression);
}
	
BuilderTemplateInstantiation::~BuilderTemplateInstantiation()
{
}

const std::string & BuilderTemplateInstantiation::getReflectionName() const
{
	return this->reflectionName;
}

const std::string & BuilderTemplateInstantiation::getClassWrapperReflectionName() const
{
	return this->classWrapperReflectionName;
}

const std::string & BuilderTemplateInstantiation::getTemplateParams() const
{
	return this->templateParams;
}

bool BuilderTemplateInstantiation::match(const CppItem * cppItem) const
{
	if(! cppItem->isContainer()) {
		GASSERT(false);
		return false;
	}

	if(! cppItem->isClass()) {
		return false;
	}

	if(! static_cast<const CppClass *>(cppItem)->isTemplate()) {
		return false;
	}

	const CppContainer * container = static_cast<const CppContainer *>(cppItem);
	for(StringListType::const_reverse_iterator it = this->classNameList.rbegin();
		it != this->classNameList.rend();
		++it, container != NULL) {
		if(*it != container->getName()) {
			return false;
		}
		container = container->getParent();
	}

	return true;
}

void BuilderTemplateInstantiation::parse(const std::string & templateExpression)
{
	string className;
	size_t pos = templateExpression.find_first_of('<');
	size_t lastPos = templateExpression.find_last_of('>');

	if(pos != string::npos && pos > 0) {
		className = templateExpression.substr(0, pos);
		if(lastPos != string::npos && lastPos > pos) {
			this->templateParams = templateExpression.substr(pos + 1, lastPos - (pos + 1));
		}
	}

	Poco::trimInPlace(className);
	Poco::trimInPlace(this->templateParams);

	Poco::StringTokenizer tokenizer(className, "::", Poco::StringTokenizer::TOK_IGNORE_EMPTY | Poco::StringTokenizer::TOK_TRIM);
	std::copy(tokenizer.begin(), tokenizer.end(), std::back_inserter(this->classNameList));

	if(this->classNameList.empty()) {
		fatalError(Poco::format("Wrong template instantiation '%s', no class name is found.", templateExpression));
	}

	if(this->templateParams.empty()) {
		fatalError(Poco::format("Wrong template instantiation '%s', no template parameters are found.", templateExpression));
	}
}


BuilderTemplateInstantiationRepository::BuilderTemplateInstantiationRepository()
{
}

BuilderTemplateInstantiationRepository::~BuilderTemplateInstantiationRepository()
{
	clearPointerContainer(this->instantiationList);
}

void BuilderTemplateInstantiationRepository::add(const std::string & templateExpression,
	const std::string & reflectionName, const std::string & classWrapperReflectionName)
{
	BuilderTemplateInstantiation * instantiation = new BuilderTemplateInstantiation(
		templateExpression, reflectionName, classWrapperReflectionName);
		
	this->instantiationList.push_back(instantiation);
}

void BuilderTemplateInstantiationRepository::loadInstantiations(const CppItem * cppItem,
	TemplateInstantiationListType * outList) const
{
	for(TemplateInstantiationListType::const_iterator it = this->instantiationList.begin();
		it != this->instantiationList.end();
		++it) {
		if((*it)->match(cppItem)) {
			outList->push_back(*it);
		}
	}
}


} // namespace metagen

