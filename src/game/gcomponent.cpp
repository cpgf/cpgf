#include <map>
#include <string>

#include "cpgf/game/gcomponent.h"
#include "cpgf/game/gsubsystem.h"
#include "cpgf/game/gentity.h"
#include "cpgf/game/gdebug.h"


using namespace std;


namespace cpgf {


GComponent::GComponent(int type) : type(type), ownerEntity(NULL), enabled(true), parentComponent(NULL), updateOrder(0)
{
    debugAssert(type >= 0, "Invalid component type, maybe the component was not registered?");
}

GComponent::~GComponent()
{
}

void GComponent::attachToEntity(GEntity *entity)
{
	this->ownerEntity = entity;

	GSubSystemManager * subSystemManager = this->ownerEntity->getSubSystemManager();
	if(subSystemManager != NULL) {
		subSystemManager->addComponent(this);
	}
}

void GComponent::detachFromEntity(GEntity *entity)
{
	GSubSystemManager * subSystemManager = this->ownerEntity->getSubSystemManager();
	if(subSystemManager != NULL) {
		subSystemManager->removeComponent(this);
	}

	this->ownerEntity = NULL;
}

void GComponent::setUpdater(ComponentUpdaterTypePtr updater)
{
	this->updater.reset(updater);
}

void GComponent::doUpdate(GUpdateContext * context)
{
}

int GComponent::loadUpdateOrder()
{
	return 0;
}

int GComponent::getParentedLevel() const
{
	int level = 0;

	GComponent * parent = this->parentComponent;

	while(parent != NULL) {
		++level;

		parent = parent->parentComponent;
	}

	return level;
}



GComponentRegister::GComponentRegister() : availableType(0), updateCount(0)
{
}

GComponentRegister::~GComponentRegister()
{
}

void GComponentRegister::beginRegister()
{
	if(this->updateCount < 0) {
		this->updateCount = 0;
	}

	this->updateCount++;

	this->componentInitList.reset(new ComponentInitListType);
}

void GComponentRegister::endRegister()
{
	this->updateCount--;

	if(this->updateCount < 0) {
		this->updateCount = 0;
	}

	if(this->updateCount == 0) {
		for(ComponentInitListType::iterator it = this->componentInitList->begin(); it != this->componentInitList->end(); ++it) {
			(*it)();
		}
	}

	this->componentInitList.reset(NULL);
}

int GComponentRegister::registerComponent(const std::string & name, const std::string & className, const ComponentCreatorType & creator, ComponentInitType init)
{
	debugAssert(this->updateCount > 0, "Register must be in begin..end");

	this->componentInitList->push_back(init);

	GComponentRegisterItem * item = this->getRegisterItemFromName(name);

	if(item == NULL) {
		int type = this->availableType;

		this->availableType++;
		GComponentRegisterItem newItem = GComponentRegisterItem(name, type);
		this->componentMap[type] = newItem;
		newItem.childItemmap[className].setCreator(creator);

		return type;
	}
	else {
		item->childItemmap[className].setCreator(creator);

		return item->getType();
	}
}

int GComponentRegister::getComponentTypeFromName(const std::string & name)
{
	GComponentRegisterItem * item = this->getRegisterItemFromName(name);

	if(item == NULL) {
		return invalidComponentType;
	}
	else {
		return item->getType();
	}
}

GComponentRegister::GComponentRegisterItem * GComponentRegister::getRegisterItemFromType(int type)
{
	MapType::iterator it = this->componentMap.find(type);

	if(it == this->componentMap.end()) {
		return NULL;
	}
	else {
		return & it->second;
	}
}

GComponentRegister::GComponentRegisterItem * GComponentRegister::getRegisterItemFromName(const std::string & name)
{
	for(MapType::iterator it = this->componentMap.begin(); it != this->componentMap.end(); ++it) {
		if(it->second.getName() == name) {
			return & it->second;
		}
	}

	return NULL;
}


GComponentRegister componentRegister;
GComponentRegister * getComponentRegister()
{
	return &componentRegister;
}


} // namespace cpgf
