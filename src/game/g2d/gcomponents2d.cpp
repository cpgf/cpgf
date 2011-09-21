#include "cpgf/game/g2d/gcomponents2d.h"
#include "cpgf/game/g2d/grenderengine2d.h"
#include "cpgf/game/gentity.h"
#include "cpgf/game/gapplication.h"
#include "cpgf/game/geventsdef.h"


namespace cpgf {


int GOrientationComponent::thisComponentType = invalidComponentType;

int GLocalTransformComponent::thisComponentType = invalidComponentType;
int GLocalTransformComponent::orientationComponentType = invalidComponentType;

int GGlobalTransformComponent::thisComponentType = invalidComponentType;
int GGlobalTransformComponent::localTransformComponentType = invalidComponentType;

int GRenderComponent::thisComponentType = invalidComponentType;
int GRenderComponent::globalTransformComponentType = invalidComponentType;
int GRenderComponent::orientationComponentType = invalidComponentType;

int GMotionComponent::thisComponentType = invalidComponentType;
int GMotionComponent::orientationComponentType = invalidComponentType;

int GTouchInputComponent::thisComponentType = invalidComponentType;
int GTouchInputComponent::orientationComponentType = invalidComponentType;


void GOrientationComponent::applyToMatrix(GMatrix4 * matrix)
{
	matrix->loadIdentity();
	matrix->translate(this->getX(), this->getY());
	matrix->scale(this->getScaleX(), this->getScaleY());
	matrix->rotateZ(this->rotate);
}


void GLocalTransformComponent::doUpdate(GUpdateContext * context)
{
	GOrientationComponent * orientation = static_cast<GOrientationComponent *>(this->ownerEntity->getComponent(orientationComponentType));

	if(orientation != NULL) {
		orientation->applyToMatrix(this->matrix.get());
	}
}



int GGlobalTransformComponent::loadUpdateOrder()
{
	return this->getParentedLevel();
}

void GGlobalTransformComponent::attachToEntity(GEntity *entity)
{
	super::attachToEntity(entity);

	if(this->ownerEntity->getComponent(localTransformComponentType) != NULL) {
		this->matrix.reset(new GMatrix4());
	}
}

void GGlobalTransformComponent::detachFromEntity(GEntity *entity)
{
	this->matrix.reset(NULL);
}

void GGlobalTransformComponent::doUpdate(GUpdateContext * context)
{
	if(this->matrix == NULL) {
		return;
	}

	GLocalTransformComponent * local = static_cast<GLocalTransformComponent *>(this->ownerEntity->getComponent(localTransformComponentType));
	GGlobalTransformComponent * global = static_cast<GGlobalTransformComponent *>(this->getParentComponent());
	if(global != NULL) {
		this->matrix->copyFrom(GMatrix4::multiply(*global->getMatrix(), *local->getMatrix()));
	}
	else {
		this->matrix->copyFrom(*local->getMatrix());
	}
}



int GRenderComponent::loadUpdateOrder()
{
	GOrientationComponent * orientation = static_cast<GOrientationComponent *>(this->ownerEntity->getComponent(orientationComponentType));

	if(orientation != NULL) {
		return orientation->getZOrder();
	}
	else {
		return 0;
	}
}

void GRenderComponent::doUpdate(GUpdateContext * context)
{
	gcoord x = 0, y = 0, w = 80, h = 30;
	GRenderEngine2D * re = gSafeCast<GRenderEngine2D *>(context->renderEngine);

	re->setColor(this->color);
	GGlobalTransformComponent * component = static_cast<GGlobalTransformComponent *>(this->ownerEntity->getComponent(globalTransformComponentType));
	re->getMatrix()->copyFrom(* component->getMatrix());

	GRect rect(x, y, x + w, y + h);
	re->fillRect(&rect);
}


void GMotionComponent::doUpdate(GUpdateContext * context)
{
	this->velocityX.update();
	this->velocityY.update();
	this->velocityRotate.update();

	GOrientationComponent * orientation = static_cast<GOrientationComponent *>(this->ownerEntity->getComponent(orientationComponentType));

	if(orientation != NULL) {
		orientation->setX(orientation->getX() + this->velocityX.getVelocity());
		orientation->setY(orientation->getY() + this->velocityY.getVelocity());
		orientation->setRotate(orientation->getRotate() + this->velocityRotate.getVelocity());
	}
}


int GTouchInputComponent::loadUpdateOrder()
{
	GOrientationComponent * orientation = static_cast<GOrientationComponent *>(this->ownerEntity->getComponent(orientationComponentType));

	if(orientation != NULL) {
		return orientation->getZOrder();
	}
	else {
		return 0;
	}
}

bool GTouchInputComponent::acceptLocation(gcoord screenX, gcoord screenY)
{
	return true;
}

bool GTouchInputComponent::processTouchInput(int message, int key, gcoord x, gcoord y)
{
	if(this->ownerEntity == NULL) {
		return false;
	}

	this->ownerEntity->sendEvent(GTouchInputEvent(this->ownerEntity, message, key, x, y));

	return true;
}


} // namespace cpgf

