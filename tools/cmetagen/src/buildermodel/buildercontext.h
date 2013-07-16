#ifndef __BUILDERCONTEXT_H
#define __BUILDERCONTEXT_H

#include "builderitem.h"

#include "cpgf/gscopedptr.h"

#include <vector>

namespace metagen {


class CppContext;
class CppContanier;
class CppFile;
class BuilderFile;
class BuilderContainer;
class BuilderSectionList;
class Project;

class BuilderContext
{
public:
	typedef std::vector<BuilderItem *> ItemListType;

public:
	explicit BuilderContext(const Project * project);
	~BuilderContext();

	void process(const CppContext * cppContext);
	
	const Project * getProject() const { return this->project; }
	ItemListType * getItemList() { return &this->itemList; }
	BuilderSectionList * getSectionList() { return this->sectionList.get(); }

protected:
	void doProcessFile(const CppFile * cppFile);

	void flatten(BuilderFile * file);
	void doFlatten(BuilderFile * file, BuilderContainer * builderContainer);

	bool shouldSkipItem(const CppItem * cppItem);
	
	BuilderItem * createItem(const CppItem * cppItem);

private:
	const Project * project;
	ItemListType itemList;
	cpgf::GScopedPointer<BuilderSectionList> sectionList;
};


} // namespace metagen


#endif
