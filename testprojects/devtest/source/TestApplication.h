#ifndef __TESTAPPLICATION_H
#define __TESTAPPLICATION_H

#include "cpgf/game/gcore.h"
#include "cpgf/game/gapplication.h"
#include "cpgf/game/gscene.h"

using namespace cpgf;

class TestApplication : public GApplication
{
private:
	typedef GApplication super;

protected:
	virtual void onEngineStart() override;
	virtual void onEngineStop() override;

	virtual void onEngineUpdate(GUpdateContext * context) override;
	virtual void onEngineRender(GUpdateContext * context) override;

private:
	GScopedPointer<GSceneManager> sceneManager;
};


#endif

