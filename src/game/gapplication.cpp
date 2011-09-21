#include "cpgf/game/gapplication.h"
#include "cpgf/game/grenderengine.h"
#include "cpgf/game/ginput.h"
#include "cpgf/game/gsubsystem.h"
#include "cpgf/game/gcomponent.h"

namespace cpgf {

static GScopedPointer<GEngine> engineHolder;
GEngine *gEngine = NULL;

void GEngine::createEngine(IEngineEvent *engineHandler, GRenderEngine *renderEngine, GDevice *device, GInput * input)
{
	if(gEngine == NULL) {
		gEngine = new GEngine(engineHandler, renderEngine, device, input);
		engineHolder.reset(gEngine);
	}
}

GEngine::GEngine(IEngineEvent *engineHandler, GRenderEngine *renderEngine, GDevice *device, GInput * input)
	: engineHandler(engineHandler),
	renderEngine(renderEngine),
	device(device),
	input(input),
	eventQueue(new GEventQueue())
{
}

GEngine::~GEngine()
{
}

void GEngine::startEngine()
{
	this->renderEngine->initializeEngine();

	this->engineHandler->onEngineStart();
}

void GEngine::stopEngine()
{
	this->renderEngine->finalizeEngine();

	this->engineHandler->onEngineStop();
}

void GEngine::deactivateEngine()
{
	this->engineHandler->onEngineDeactivate();
}

void GEngine::activateEngine()
{
	this->engineHandler->onEngineActivate();
}

void GEngine::resizeViewport(int width, int height)
{
	this->device->viewportWidth = static_cast<gcoord>(width);
	this->device->viewportHeight = static_cast<gcoord>(height);

	this->renderEngine->onEngineResize();
	this->engineHandler->onEngineResize();
}

void GEngine::updateEngine()
{
	GUpdateContext context;
	context.input = this->input.get();

	if(this->input != NULL) {
		this->input->updateInput();
	}

	this->eventQueue->processEvents();

	this->engineHandler->onEngineUpdate(&context);

	if(this->input != NULL) {
		this->input->resetInput();
	}
}

void GEngine::drawEngine()
{
	GUpdateContext context;
	context.renderEngine = this->renderEngine.get();

	this->engineHandler->onEngineRender(&context);
}


GApplication::GApplication()
{
}

GApplication::~GApplication()
{
}

void GApplication::onEngineStart()
{
}

void GApplication::onEngineStop()
{
}

void GApplication::onEngineActivate()
{
}

void GApplication::onEngineDeactivate()
{
}

void GApplication::onEngineResize()
{
}

void GApplication::onEngineUpdate(GUpdateContext * context)
{
	this->updateRunner.update(context);
}

void GApplication::onEngineRender(GUpdateContext * context)
{
	context->renderEngine->beginRender();

	this->updateRunner.render(context);

	context->renderEngine->endRender();
	context->renderEngine->flush();
}


} // namespace cpgf

