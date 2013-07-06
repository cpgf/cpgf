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

#include "model/cppfile.h"
#include "model/cppcontext.h"
#include "config.h"
#include "util.h"

#include "cpgf/gassert.h"


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


BuilderContext::BuilderContext(const Config * config)
	: config(config)
{
}

BuilderContext::~BuilderContext()
{
	clearPointerContainer(this->itemList);
}

BuilderItem * BuilderContext::createItem(const CppItem * cppItem)
{
	BuilderItem * builderItem = createBuilderItem(cppItem);
	builderItem->setConfig(this->config);
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
	file->setConfig(this->config);

	this->flatten(file);
	file->prepare();
	file->outputFiles();
}

void BuilderContext::flatten(BuilderFile * file)
{
	this->doFlatten(file, static_cast<const CppContainer *>(file->getCppItem()));
}

void BuilderContext::doFlatten(BuilderFile * file, const CppContainer * cppContainer)
{
	for(CppContainer::ItemListType::const_iterator it = cppContainer->getItemList()->begin(); it != cppContainer->getItemList()->end(); ++it) {
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
		this->itemList.push_back(item.take());
		if((*it)->isContainer()) {
			this->doFlatten(file, static_cast<const CppContainer *>(*it));
		}
	}
}

bool BuilderContext::shouldSkipItem(const CppItem * cppItem)
{
	ItemVisibility visibility = cppItem->getVisibility();
	if((visibility == ivPublic) != this->config->doesAllowPublic()
		|| (visibility == ivProtected) != this->config->doesAllowProtected()
		|| (visibility == ivPrivate) != this->config->doesAllowPrivate()
		) {
		return true;
	}

	return false;
}
