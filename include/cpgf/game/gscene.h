#ifndef __GSCENE_H
#define __GSCENE_H

#include "cpgf/gcontainer.h"
#include "cpgf/game/gsubsystem.h"
#include "cpgf/game/gevent.h"

namespace cpgf {


class GEntity;
class GEntityPool;

class GSceneManager;

class GScene : public GEventDispatcher, public IUpdatable, public IRenderable
{
public:
	GScene(GSubSystemManager * subSystemManager);
	virtual ~GScene();

	GEntityPool * getEntityPool() const {
		return this->entityPool.get();
	}

	inline GSubSystemManager * getSubSystemManager() const {
		return this->subSystemManager.get();
	}

protected:
	virtual void executeUpdatable(GUpdateContext * context);
	virtual void executeRenderable(GUpdateContext * context);

	void loadScene();
	void unloadScene();

	void onEnter();
	void onExit();

	void setManager(GSceneManager * manager);

	virtual void doOnEnter();
	virtual void doOnExit();

private:
	GSceneManager * manager;
	GScopedPointer<GSubSystemManager> subSystemManager;

	GScopedPointer<GEntityPool> entityPool;

	friend class GSceneManager;
};

class GUpdateRunner;

class GSceneManager : public IUpdatable
{
public:
	GSceneManager(GUpdateRunner * updateRunner);
	~GSceneManager();

	void executeUpdatable(GUpdateContext * context);

	void gotoScene(GScene * newScene);

	inline GUpdateRunner * getUpdateRunner() const {
		return this->updateRunner;
	}

protected:
	void doGotoScene(GScene * newScene);

private:
	GScopedPointer<GScene> currentScene;
	GScene * sceneToGoto;
	GUpdateRunner * updateRunner;
};


} // namespace cpgf


#endif
