#include "cpgf/gmetadefine.h"
#include "../samplescriptbindutil.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/gscopedinterface.h"

#include <iostream>

using namespace cpgf;
using namespace std;

int main(int argc, char * argv[])
{
	ScriptHelper scriptHelper(argc, argv);
	
	GScopedInterface<IMetaClass> metaClass(scriptHelper.borrowService()->findClassByName("irrlicht"));
	
	scriptSetValue(scriptHelper.borrowScriptObject(), "irr", GScriptValue::fromClass(metaClass.get()));

	scriptHelper.execute();

	return 0;
}
