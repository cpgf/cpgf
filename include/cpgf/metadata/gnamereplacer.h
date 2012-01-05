#ifndef __GNAMEREPLACER_H
#define __GNAMEREPLACER_H

#include "cpgf/gstringmap.h"
#include "cpgf/gscopedptr.h"

#include <string>

namespace cpgf {

class GMetaDataNameReplacer
{
private:
	typedef GStringMap<std::string> MapType;

public:
	void add(const char * name, const char * replacement) {
		if(! this->stringMap) {
			this->stringMap.reset(new MapType);
		}
		this->stringMap->set(name, replacement);
	}

	const char * get(const char * name) const {
		if(! this->stringMap || ! this->stringMap->hasKey(name)) {
			return name;
		}
		else {
			return this->stringMap->find(name)->second.c_str();
		}
	}

private:
	GScopedPointer<MapType> stringMap;
};

inline const char * replaceName(const char * name, const GMetaDataNameReplacer * replacer)
{
	return replacer ? replacer->get(name) : name;
}

inline void initializeLuaReplacer(GMetaDataNameReplacer * replacer)
{
	replacer->add("end", "_end");
}


} // namespace cpgf


#endif

