#include "cpgf/game/g2d/gsubsystems2d.h"
#include "cpgf/game/g2d/gcomponents2d.h"
#include "cpgf/game/ginput.h"


namespace cpgf {


template <typename ComponentType>
void registerComponentHelper(GComponentRegister * reg, const char * subSystemName, const char * componentName = "")
{
	reg->registerComponent(subSystemName, componentName,
		makeCallback1(&allocateNewBasedObject<GComponent, ComponentType, int>), ComponentType::componentInit);
}

void defaultRegisterComponents()
{
	GComponentRegister * reg = getComponentRegister();

	registerComponentHelper<GOrientationComponent>(reg, componentName_Orientation);
	registerComponentHelper<GRenderComponent>(reg, componentName_Render);
	registerComponentHelper<GLocalTransformComponent>(reg, componentName_LocalTransform);
	registerComponentHelper<GGlobalTransformComponent>(reg, componentName_GlobalTransform);
	registerComponentHelper<GMotionComponent>(reg, componentName_Motion);
	registerComponentHelper<GTouchInputComponent>(reg, componentName_TouchInput);
	registerComponentHelper<GRenderComponent>(reg, componentName_KeyboardInput);
}

template <typename SubSystemType>
int registerSubSystemHelper(GSubSystemRegister * reg, const char * subSystemName, int updateOrder)
{
	return reg->registerSubSystem(subSystemName, updateOrder,
		makeCallback2(&allocateNewBasedObject<GSubSystem, SubSystemType, int, int>));
}

void defaultRegisterSubSystems()
{
	GSubSystemRegister * reg = getSubSystemRegister();

	int updateOrder = 0;

	registerSubSystemHelper<GTouchInputSubSystem>(reg, subSystemName_TouchInput, updateOrder++);
	registerSubSystemHelper<GSubSystem>(reg, subSystemName_KeyboardInput, updateOrder++);
	registerSubSystemHelper<GRenderSubSystem>(reg, subSystemName_LocalTransform, updateOrder++);
	registerSubSystemHelper<GRenderSubSystem>(reg, subSystemName_GlobalTransform, updateOrder++);
	registerSubSystemHelper<GSubSystem>(reg, subSystemName_Motion, updateOrder++);
	registerSubSystemHelper<GRenderSubSystem>(reg, subSystemName_Render, updateOrder++);
}

void defaultConnectComponents()
{
	GSubSystemRegister * reg = getSubSystemRegister();

	reg->connectComponentToSubSystem(subSystemName_TouchInput, componentName_TouchInput);
	reg->connectComponentToSubSystem(subSystemName_KeyboardInput, componentName_KeyboardInput);
	reg->connectComponentToSubSystem(subSystemName_LocalTransform, componentName_LocalTransform);
	reg->connectComponentToSubSystem(subSystemName_GlobalTransform, componentName_GlobalTransform);
	reg->connectComponentToSubSystem(subSystemName_Motion, componentName_Motion);
	reg->connectComponentToSubSystem(subSystemName_Render, componentName_Render);
}

void initializeSubsystemAndComponent2D()
{
	defaultRegisterSubSystems();

	GComponentRegister * reg = getComponentRegister();
	reg->beginRegister();
	defaultRegisterComponents();
	reg->endRegister();

	defaultConnectComponents();
}


GRenderSubSystem::GRenderSubSystem(int type, int updateOrder) : super(type, updateOrder, compareComponentUpdateOrder)
{
}

bool GRenderSubSystem::isRender()
{
	return true;
}



GTouchInputSubSystem::GTouchInputSubSystem(int type, int updateOrder) : super(type, updateOrder, revertCompareComponentUpdateOrder)
{
}

void GTouchInputSubSystem::doUpdateSubSystem(GUpdateContext * context)
{
	GInputPool * const pool = context->input->getTouchPool();

	const GInputData * inputData = pool->peekNext();

	while(inputData != NULL) {
		if(! this->processInput(inputData)) {
			break;
		}
		pool->getNext();
		inputData = pool->peekNext();
	}
}

bool GTouchInputSubSystem::processInput(const GInputData * const inputData)
{
	for(ListType::iterator it = this->componentList->begin(); it != this->componentList->end(); ++it) {
		GTouchInputComponent * component = gSafeCast<GTouchInputComponent *>(*it);

		if(component != NULL) {
			if(component->acceptLocation(inputData->screenX, inputData->screenY)) {
				if(component->processTouchInput(inputData->message, inputData->key, inputData->screenX, inputData->screenY)) {
					return true;
				}
			}
		}
	}

	return false;
}


} // namespace cpgf
