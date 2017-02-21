#ifndef CPGF_BUILDERTEMPLATEINSTANTIATION_H
#define CPGF_BUILDERTEMPLATEINSTANTIATION_H

#include <string>
#include <vector>

namespace metagen {

class CppItem;

class BuilderTemplateInstantiation
{
private:
	typedef std::vector<std::string> StringListType;

private:
	BuilderTemplateInstantiation(const std::string & templateExpression,
		const std::string & reflectionName, const std::string & classWrapperReflectionName);
	
public:
	~BuilderTemplateInstantiation();
	
	const std::string & getReflectionName() const;
	const std::string & getClassWrapperReflectionName() const;
	const std::string & getTemplateParams() const;

	bool match(const CppItem * cppItem) const;

private:
	void parse(const std::string & templateExpression);

private:
	StringListType classNameList;
	std::string templateParams;
	std::string reflectionName;
	std::string classWrapperReflectionName;

private:
	friend class BuilderTemplateInstantiationRepository;
};


typedef std::vector<BuilderTemplateInstantiation *> TemplateInstantiationListType;

class BuilderTemplateInstantiationRepository
{
public:
	BuilderTemplateInstantiationRepository();
	~BuilderTemplateInstantiationRepository();
	
	void add(const std::string & templateExpression,
		const std::string & reflectionName, const std::string & classWrapperReflectionName);

	void loadInstantiations(const CppItem * cppItem, TemplateInstantiationListType * outList) const;

private:
	TemplateInstantiationListType instantiationList;
};


} // namespace metagen


#endif
