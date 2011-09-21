#include "cpgf/game/gentity.h"
#include "cpgf/game/gcomponent.h"
#include "cpgf/game/gsubsystem.h"

using namespace std;


namespace cpgf {

GEntity::GComponentInstallSession::GComponentInstallSession(GEntity * ownerEntity) : ownerEntity(ownerEntity), sessionCount(0)
{
}

void GEntity::GComponentInstallSession::beginInstall()
{
	debugAssert(this->sessionCount == 0, "");

	this->sessionCount++;
}

bool GEntity::GComponentInstallSession::endInstall()
{
	debugAssert(this->sessionCount > 0, "");

	this->sessionCount--;

	if(this->sessionCount == 0) {
		int count = this->installList.size();
		for(int i = 0; i < count; ++i) {
			this->installList[i]->attachToEntity(this->ownerEntity);
		}
		return true;
	}

	return false;
}

void GEntity::GComponentInstallSession::install(GComponent * component)
{
	debugAssert(this->sessionCount > 0, "");

	this->installList.push_back(component);
}


GEntity::GEntity(GSubSystemManager * subSystemManager)
	: installSession(NULL), subSystemManager(subSystemManager), previous(NULL), next(NULL)
{
}

GEntity::~GEntity()
{
}

void GEntity::beginInstallComponent()
{
	if(!this->installSession) {
		this->installSession.reset(new GComponentInstallSession(this));
	}

	this->installSession->beginInstall();
}

void GEntity::endInstallComponent()
{
	debugAssert(this->installSession, "");

	if(this->installSession->endInstall()) {
		this->installSession.reset(NULL);
	}
}

void GEntity::installComponent(GComponent * component)
{
	debugAssert(this->installSession != NULL, "");

	this->installSession->install(component);

	int type = component->getType();

	this->uninstallComponent(type);

	this->composite.addElement(type, component);
}

void GEntity::uninstallComponent(int type)
{
	GComponent * component = this->getComponent(type);
	if(component != NULL) {
		component->detachFromEntity(this);

		this->composite.removeElement(type);
	}
}



} // namespace cpgf

