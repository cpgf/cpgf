#ifndef __GSUBSYSTEMS2D_H
#define __GSUBSYSTEMS2D_H

#include "cpgf/game/gsubsystem.h"

namespace cpgf {


const char * const subSystemName_TouchInput = "touch";
const char * const subSystemName_KeyboardInput = "keyboard";
const char * const subSystemName_LocalTransform = "localtransform";
const char * const subSystemName_GlobalTransform = "globaltransform";
const char * const subSystemName_Motion = "motion";
const char * const subSystemName_Render = "render";


void initializeSubsystemAndComponent2D();


class GRenderSubSystem : public GSubSystem
{
private:
	typedef GSubSystem super;

public:
	GRenderSubSystem(int type, int updateOrder);

	virtual bool isRender() override;
};

class GInputData;

class GTouchInputSubSystem : public GSubSystem
{
private:
	typedef GSubSystem super;

public:
	GTouchInputSubSystem(int type, int updateOrder);

protected:
	virtual void doUpdateSubSystem(GUpdateContext * context) override;

	bool processInput(const GInputData * const inputData);
};


} // namespace cpgf


#endif
