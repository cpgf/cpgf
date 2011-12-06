#include "cpgf/scriptbind/gscriptbindutil.h"

#include "../pinclude/gscriptbindapiimpl.h"

#include <string>


using namespace std;
using namespace cpgf::bind_internal;


namespace cpgf {


IScriptObject * scriptObjectToInterface(GScriptObject * scriptObject)
{
	return new ImplScriptObject(scriptObject);
}

void injectMetaClassToScript(IScriptObject * scriptObject, IMetaClass * metaClass, void * instance)
{
	GScopedInterface<IObject> metaObject;
	
	GMetaMapClass mapClass(metaClass);
	
	const GMetaMapClass::MapType * mapData = mapClass.getMap();
	for(GMetaMapClass::MapType::const_iterator it = mapData->begin(); it != mapData->end(); ++it) {
		const char * name = it->first;
		const GMetaMapItem * item = &it->second;
		metaObject.reset(item->getItem());

		switch(item->getType()) {
			case mmitMethod:
				scriptObject->bindMethod(normalizeReflectName(name).c_str(), instance, gdynamic_cast<IMetaMethod *>(metaObject.get()));
				break;

			case mmitMethodList:
				scriptObject->bindMethodList(normalizeReflectName(name).c_str(), gdynamic_cast<IMetaList *>(metaObject.get()));
				break;

			case mmitProperty:
			case mmitField:
				break;

			case mmitEnum:
				scriptObject->bindEnum(normalizeReflectName(name).c_str(), gdynamic_cast<IMetaEnum *>(metaObject.get()));
				break;

			case mmitEnumValue:
				break;

			case mmitClass:
				scriptObject->bindClass(normalizeReflectName(name).c_str(), gdynamic_cast<IMetaClass *>(metaObject.get()));
				break;

			default:
				break;
		}
	}
	
}



} // namespace cpgf

