#include "buildercontext.h"
#include "builderclass.h"
#include "builderconstructor.h"
#include "buildercontainer.h"
#include "builderdestructor.h"
#include "builderenum.h"
#include "builderfield.h"
#include "builderfile.h "
#include "builderitem.h"
#include "buildermethod.h"
#include "buildernamespace.h"
#include "builderoperator.h"
#include "builderutil.h"

#include "model/cppfile.h"
#include "model/cppcontext.h"
#include "project.h"
#include "util.h"

#include "cpgf/gassert.h"


namespace metagen {


BuilderItem * createBuilderItem(const CppItem * cppItem)
{
	switch(cppItem->getCategory()) {
		case icFile:
			return new BuilderFile(cppItem);

		case icNamespace:
			return new BuilderNamespace(cppItem);

		case icClass:
			return new BuilderClass(cppItem);

		case icField:
			return new BuilderField(cppItem);

		case icMethod:
			return new BuilderMethod(cppItem);

		case icEnum:
			return new BuilderEnum(cppItem);

		case icOperator:
			return new BuilderOperator(cppItem);

		case icConstructor:
			return new BuilderConstructor(cppItem);

		case icDestructor:
			return new BuilderDestructor(cppItem);

		default:
			GASSERT(false);
			break;
	}
	
	return NULL;
}


BuilderContext::BuilderContext(const Project * project)
	: project(project)
{
}

BuilderContext::~BuilderContext()
{
	clearPointerContainer(this->itemList);
}

BuilderItem * BuilderContext::createItem(const CppItem * cppItem)
{
	BuilderItem * builderItem = createBuilderItem(cppItem);
	builderItem->setConfig(this->project);
	return builderItem;
}

void BuilderContext::process(const CppContext * cppContext)
{
	this->doProcessFile(cppContext->getCppFile());
}

void BuilderContext::doProcessFile(const CppFile * cppFile)
{
	BuilderFile * file = static_cast<BuilderFile *>(this->createItem(cppFile));
	this->itemList.push_back(file);
	file->setConfig(this->project);

	this->flatten(file);
	file->outputFiles();
}

void BuilderContext::flatten(BuilderFile * file)
{
	this->doFlatten(file, file);
}

void BuilderContext::doFlatten(BuilderFile * file, BuilderContainer * builderContainer)
{
	for(CppContainer::ItemListType::const_iterator it = builderContainer->getCppContainer()->getItemList()->begin();
		it != builderContainer->getCppContainer()->getItemList()->end(); ++it) {
		if(! (*it)->isInMainFile()) {
			continue;
		}
		if(this->shouldSkipItem(*it)) {
			continue;
		}

		cpgf::GScopedPointer<BuilderItem> item(this->createItem(*it));
		if(! item->canBind()) {
			continue;
		}
		if(item->shouldSkipBind()) {
			continue;
		}

		file->getItemList()->push_back(item.get());
		BuilderItem * itemPointer = item.get();
		this->itemList.push_back(item.take());
		builderContainer->addItem(itemPointer);
		if((*it)->isContainer()) {
			this->doFlatten(file, static_cast<BuilderContainer *>(itemPointer));
		}
	}
}

bool BuilderContext::shouldSkipItem(const CppItem * cppItem)
{
	return ! isVisibilityAllowed(cppItem->getVisibility(), this->project);
}


} // namespace metagen

