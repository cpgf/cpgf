#include "cpgf/scriptbind/gscriptbindutil.h"

#include "../pinclude/gscriptbindapiimpl.h"

#include <string>


using namespace std;
using namespace cpgf::bind_internal;


namespace cpgf {


inline bool isCSymbol(unsigned char c) {
    return isalpha(c) || c == '_' || isdigit(c);
}

std::string normalizeReflectName(const char * name)
{
	size_t length = strlen(name);

	if(length == 0) {
		return "";
	}

	const char * p = name + (length - 1);
	const char * end = p;

	while(!isCSymbol(*end) && end > name) {
		--end;
	}
	if(isCSymbol(*end)) {
		++end;
	}

	while(isCSymbol(*p) && p > name) {
		--p;
	}

	if(!isCSymbol(*p)) {
		++p;
	}

	return std::string(p, end - p);
}


GVariant scriptGetFundamental(GScriptObject * scriptObject, const char * name)
{
	return scriptObject->getFundamental(name);
}

GVariant scriptGetFundamental(IScriptObject * scriptObject, const char * name)
{
	GVariant v;
	scriptObject->getFundamental(&v.data, name);
	return v;
}


IScriptObject * scriptObjectToInterface(GScriptObject * scriptObject, bool freeObject)
{
	return new ImplScriptObject(scriptObject, freeObject);
}

IScriptObject * scriptObjectToInterface(GScriptObject * scriptObject)
{
	return scriptObjectToInterface(scriptObject, false);
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
				scriptObject->bindAccessible(normalizeReflectName(name).c_str(), instance, gdynamic_cast<IMetaAccessible *>(metaObject.get()));
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

