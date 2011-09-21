#include "cpgf/game/gsubsystem.h"
#include "cpgf/game/gcomponent.h"
#include "cpgf/game/gutility.h"
#include "cpgf/game/gdebug.h"


namespace cpgf {


GSubSystem::GSubSystem(int type, int updateOrder, CompareType compare)
	: type(type), updateOrder(updateOrder), componentCompare(compare)
{
	this->componentList.reset(new ListType(GComponentSortComparer(this)));
}

GSubSystem::~GSubSystem()
{
}

bool GSubSystem::canHandle(int componentType)
{
	return getSubSystemRegister()->hasConnectedComponentToSubSystem(this->getType(), componentType);
}

void GSubSystem::addComponent(GComponent * component)
{
	component->prepareUpdateOrder();

	this->componentList->insert(component);
}

void GSubSystem::removeComponent(GComponent * component)
{
	this->componentList->remove(component);
}

void GSubSystem::updateSubSystem(GUpdateContext * context)
{
	this->doUpdateSubSystem(context);
}

void GSubSystem::doUpdateSubSystem(GUpdateContext * context)
{
	for(ListType::iterator it = this->componentList->begin(); it != this->componentList->end(); ++it) {
		GComponent * component = *it;

		if(component != NULL && component->getEnabled()) {
			component->update(context);
		}
	}
}


void GSubSystemManager::GSubSystemSortedList::addSubSystem(GSubSystem * subSystem)
{
	this->sortedList.insert(subSystem);
}

void GSubSystemManager::GSubSystemSortedList::removeSubSystem(GSubSystem * subSystem)
{
	this->sortedList.remove(subSystem);
}

void GSubSystemManager::GSubSystemSortedList::addComponent(GComponent * component)
{
	int componentType = component->getType();

	for(SortedListType::iterator it = this->sortedList.begin(); it != this->sortedList.end(); ++it) {
		GSubSystem * subSystem = *it;
		if(subSystem->canHandle(componentType)) {
			subSystem->addComponent(component);
		}
	}
}

void GSubSystemManager::GSubSystemSortedList::removeComponent(GComponent * component)
{
	int componentType = component->getType();

	for(SortedListType::iterator it = this->sortedList.begin(); it != this->sortedList.end(); ++it) {
		GSubSystem * subSystem = *it;
		if(subSystem->canHandle(componentType)) {
			subSystem->removeComponent(component);
		}
	}
}

void GSubSystemManager::GSubSystemSortedList::execute(GUpdateContext * context)
{
	for(SortedListType::iterator it = this->sortedList.begin(); it != this->sortedList.end(); ++it) {
		(*it)->updateSubSystem(context);
	}
}

GSubSystemManager::GSubSystemManager()
{
}

GSubSystemManager::~GSubSystemManager()
{
	this->clear();
}

void GSubSystemManager::addSubSystem(GSubSystem * subSystem)
{
	int type = subSystem->getType();

	this->removeSubSystem(type);

	this->composite.addElement(type, subSystem);

	GSubSystem * s = this->getSubSystem(type);
	if(s != NULL) {
		if(s->isRender()) {
			this->renderList.addSubSystem(s);
		}
		else {
			this->updateList.addSubSystem(s);
		}
	}
}

void GSubSystemManager::removeSubSystem(int type)
{
	GSubSystem * s = this->getSubSystem(type);
	if(s != NULL) {
		if(s->isRender()) {
			this->renderList.removeSubSystem(s);
		}
		else {
			this->updateList.removeSubSystem(s);
		}
	}

	this->composite.removeElement(type);
}

void GSubSystemManager::addComponent(GComponent * component)
{
	this->updateList.addComponent(component);
	this->renderList.addComponent(component);
}

void GSubSystemManager::removeComponent(GComponent * component)
{
	this->updateList.removeComponent(component);
	this->renderList.removeComponent(component);
}

void GSubSystemManager::executeUpdatable(GUpdateContext * context)
{
	this->updateList.execute(context);
}

void GSubSystemManager::executeRenderable(GUpdateContext * context)
{
	this->renderList.execute(context);
}

void GSubSystemManager::clear()
{
	for(SortedListType::iterator it = this->updateList.sortedList.begin(); it != this->updateList.sortedList.end(); ++it) {
		this->removeSubSystem((*it)->getType());
	}
	for(SortedListType::iterator it = this->renderList.sortedList.begin(); it != this->renderList.sortedList.end(); ++it) {
		this->removeSubSystem((*it)->getType());
	}
}



GSubSystemRegister::GSubSystemRegister() : availableType(0)
{
}

GSubSystemRegister::~GSubSystemRegister()
{
}

int GSubSystemRegister::registerSubSystem(std::string name, int updateOrder, const SubSystemCreatorType & creator)
{
	GSubSystemRegisterItem * item = getRegisterItemFromName(name);

	if(item == NULL) {
		int type = this->availableType;

		this->availableType++;
		this->subSystemMap[type] = GSubSystemRegisterItem(name, type, updateOrder, creator);

		return type;
	}
	else {
		item->setCreator(creator);
		item->setUpdateOrder(updateOrder);

		return item->getType();
	}
}

void GSubSystemRegister::connectComponentToSubSystem(const std::string & subSystemName, const std::string & componentName)
{
	int subSystemType = this->getSubSystemTypeFromName(subSystemName);
	int componentType = getComponentRegister()->getComponentTypeFromName(componentName);

	this->connectComponentToSubSystem(subSystemType, componentType);
}

void GSubSystemRegister::connectComponentToSubSystem(int subSystemType, int componentType)
{
	debugAssert(subSystemType != invalidSubSystemType, "Invalid sub system type");
	debugAssert(componentType != invalidComponentType, "Invalid component type");

	GSubSystemRegister::GSubSystemRegisterItem * item = this->getRegisterItemFromType(subSystemType);

	debugAssert(item != NULL, "");

	item->addComponentType(componentType);
}

void GSubSystemRegister::disconnectComponentFromSubSystem(int subSystemType, int componentType)
{
	debugAssert(subSystemType != invalidSubSystemType, "Invalid sub system type");
	debugAssert(componentType != invalidComponentType, "Invalid component type");

	GSubSystemRegister::GSubSystemRegisterItem * item = this->getRegisterItemFromType(subSystemType);

	debugAssert(item != NULL, "");

	item->removeComponentType(componentType);
}

void GSubSystemRegister::disconnectAllComponentFromSubSystem(int subSystemType)
{
	debugAssert(subSystemType != invalidSubSystemType, "Invalid sub system type");

	GSubSystemRegister::GSubSystemRegisterItem * item = this->getRegisterItemFromType(subSystemType);

	debugAssert(item != NULL, "");

	item->removeAllComponentType();
}

bool GSubSystemRegister::hasConnectedComponentToSubSystem(int subSystemType, int componentType)
{
	GSubSystemRegister::GSubSystemRegisterItem * item = this->getRegisterItemFromType(subSystemType);

	return item != NULL && item->containsComponentType(componentType);
}

void GSubSystemRegister::fillSubSystemManager(GSubSystemManager * manager) const
{
	MapType::const_iterator it = this->subSystemMap.begin();

	while(it != this->subSystemMap.end()) {
		const GSubSystemRegister::GSubSystemRegisterItem * item = &it->second;
		GSubSystem * subSystem = item->createSubSystem();
		manager->addSubSystem(subSystem);

		++it;
	}
}

int GSubSystemRegister::getSubSystemTypeFromName(const std::string & name)
{
	GSubSystemRegister::GSubSystemRegisterItem * item = this->getRegisterItemFromName(name);

	if(item == NULL) {
		return invalidSubSystemType;
	}
	else {
		return item->getType();
	}
}

GSubSystemRegister::GSubSystemRegisterItem * GSubSystemRegister::getRegisterItemFromType(int type)
{
	MapType::iterator it = this->subSystemMap.find(type);
	
	if(it == this->subSystemMap.end()) {
		return NULL;
	}
	else {
		return & it->second;
	}
}

GSubSystemRegister::GSubSystemRegisterItem * GSubSystemRegister::getRegisterItemFromName(const std::string & name)
{
	for(MapType::iterator it = this->subSystemMap.begin(); it != this->subSystemMap.end(); ++it) {
		if(it->second.getName() == name) {
			return & it->second;
		}
	}

	return NULL;
}



GSubSystemRegister subSystemRegister;

GSubSystemRegister * getSubSystemRegister()
{
	return &subSystemRegister;
}


} // namespace cpgf
