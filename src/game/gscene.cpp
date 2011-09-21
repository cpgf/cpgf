#include "cpgf/game/gscene.h"
#include "cpgf/game/gentity.h"
#include "cpgf/game/gapplication.h"
#include "cpgf/game/ginput.h"


namespace cpgf {


GScene::GScene(GSubSystemManager * subSystemManager) : manager(NULL), entityPool(new GEntityPool())
{
	this->subSystemManager.reset(subSystemManager);
}

GScene::~GScene()
{
}

void GScene::executeUpdatable(GUpdateContext * context)
{
}

void GScene::executeRenderable(GUpdateContext * context)
{
}

void GScene::loadScene()
{
}

void GScene::unloadScene()
{
}

void GScene::onEnter()
{
	GUpdateRunner * updateRunner = this->manager->getUpdateRunner();
	GSubSystemManager * subSystemManager = this->getSubSystemManager();

	updateRunner->addUpdatable(subSystemManager);
	updateRunner->addRenderable(subSystemManager);

	this->doOnEnter();
}

void GScene::onExit()
{
	GUpdateRunner * updateRunner = this->manager->getUpdateRunner();
	GSubSystemManager * subSystemManager = this->getSubSystemManager();

	updateRunner->removeUpdatable(subSystemManager);
	updateRunner->removeRenderable(subSystemManager);

	this->doOnExit();
}

void GScene::doOnEnter()
{
}

void GScene::doOnExit()
{
}

void GScene::setManager(GSceneManager * manager)
{
	this->manager = manager;
}



GSceneManager::GSceneManager(GUpdateRunner * updateRunner)
	: currentScene(NULL), sceneToGoto(NULL), updateRunner(updateRunner)
{
	this->updateRunner->addUpdatable(this);
}

GSceneManager::~GSceneManager()
{
	this->updateRunner->removeUpdatable(this);

	this->doGotoScene(NULL);
}

void GSceneManager::executeUpdatable(GUpdateContext * context)
{
	if(this->sceneToGoto != NULL) {
		this->doGotoScene(this->sceneToGoto);
		this->sceneToGoto = NULL;
	}
}

void GSceneManager::gotoScene(GScene * newScene)
{
	if(newScene != NULL) {
		newScene->setManager(this);
	}

	if(this->currentScene == NULL) {
		this->doGotoScene(newScene);
		this->sceneToGoto = NULL;
	}
	else {
		this->sceneToGoto = newScene;
	}
}

void GSceneManager::doGotoScene(GScene * newScene)
{
	if(this->currentScene != NULL) {
		this->currentScene->sendEvent(GEvent(eventSceneExited, this->currentScene.get()));
		this->currentScene->unloadScene();
		this->currentScene->onExit();
	}

	this->currentScene.reset(newScene);

	if(this->currentScene != NULL) {
		this->currentScene->loadScene();
		this->currentScene->onEnter();

		this->currentScene->sendEvent(GEvent(eventSceneEntered, this->currentScene.get()));
	}
}


} // namespace cpgf
