#include "../samplescriptbindutil.h"

#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/gscopedptr.h"
#include "cpgf/gapiutil.h"
#include "cpgf/gscopedinterface.h"

#include <cmath>
#include <cstring>
#include <iostream>

using namespace std;
using namespace cpgf;

void scriptTrace(const char * s)
{
	cout << "script message: " << s << endl;
}

int main(int argc, char * argv[])
{
	ScriptHelper scriptHelper(argc, argv);
	
	GDefineMetaGlobal()
		._class(
			GDefineMetaNamespace::declare("test")
				._method("scriptTrace", &scriptTrace)
		)
	;

	GScopedInterface<IMetaClass> metaClass(scriptHelper.borrowService()->findClassByName("sfml"));
	scriptSetValue(scriptHelper.borrowScriptObject(), "sfml", GScriptValue::fromClass(metaClass.get()));
	
	metaClass.reset(scriptHelper.borrowService()->findClassByName("test"));
	scriptSetValue(scriptHelper.borrowScriptObject(), "test", GScriptValue::fromClass(metaClass.get()));

	scriptHelper.execute();
	
	return 0;
}
