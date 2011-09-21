#ifndef __GAPPLICATION_H
#define __GAPPLICATION_H

#include "cpgf/game/gevent.h"
#include "cpgf/game/gsubsystem.h"
#include "cpgf/game/ggeometry.h"


namespace cpgf {

class GEngine;
class GRenderEngine;
class GInput;

struct IEngineEvent
{
	virtual void onEngineStart() = 0;
	virtual void onEngineStop() = 0;
	virtual void onEngineActivate() = 0;
	virtual void onEngineDeactivate() = 0;
	virtual void onEngineResize() = 0;

	virtual void onEngineUpdate(GUpdateContext * context) = 0;
	virtual void onEngineRender(GUpdateContext * context) = 0;
};

class GDevice
{
public:
	GDevice() : viewportWidth(0), viewportHeight(0) {
	}

	inline gcoord getViewportWidth() const {
		return this->viewportWidth;
	}

	inline gcoord getViewportHeight() const {
		return this->viewportHeight;
	}

	inline GSize getViewportSize() const {
		return GSize(this->viewportWidth, this->viewportHeight);
	}

	inline GRect getViewRect() const {
		return GRect(0, 0, this->viewportWidth, this->viewportHeight);
	}

private:
	gcoord viewportWidth;
	gcoord viewportHeight;

	friend class GEngine;
};


// don't inherite
class GEngine
{
public:
	static void createEngine(IEngineEvent * engineHandler, GRenderEngine * renderEngine, GDevice * device, GInput * input);

	inline GRenderEngine * getRenderEngine() const {
		return this->renderEngine.get();
	}

	inline GDevice * getDevice() const {
		return this->device.get();
	}

	inline GEventQueue * getEventQueue() const {
		return this->eventQueue.get();
	}

private:
	void startEngine();
	void stopEngine();
	void deactivateEngine();
	void activateEngine();
	void resizeViewport(int width, int height);

	void updateEngine();
	void drawEngine();

private:
	GEngine(IEngineEvent *engineHandler, GRenderEngine *renderEngine, GDevice *device, GInput * input);

public:
	~GEngine();

private:
	IEngineEvent * engineHandler;
	GScopedPointer<GRenderEngine> renderEngine;
	GScopedPointer<GDevice> device;
	GScopedPointer<GInput> input;
	GScopedPointer<GEventQueue> eventQueue;

	friend class private_GEngineAccess;
};


// inherite to create your own application
class GApplication : public GObject, public IEngineEvent
{
private:
	typedef GObject super;

public:
	GApplication();
	virtual ~GApplication();

	GUpdateRunner * getUpdateRunner() {
		return &this->updateRunner;
	}

protected:
	// IEngineEvent begin
	virtual void onEngineStart();
	virtual void onEngineStop();
	virtual void onEngineActivate();
	virtual void onEngineDeactivate();
	virtual void onEngineResize();

	virtual void onEngineUpdate(GUpdateContext * context);
	virtual void onEngineRender(GUpdateContext * context);
	// IEngineEvent end

private:
	GUpdateRunner updateRunner;
};

extern GEngine *gEngine;


} // namespace cpgf


#endif

