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
	virtual ~GMetaDataNameReplacer() {
	}

	void add(const char * name, const char * replacement) {
		if(! this->stringMap) {
			this->stringMap.reset(new MapType);
		}
		this->stringMap->set(name, replacement);
	}

	virtual const char * replace(const char * name) const {
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
	return replacer ? replacer->replace(name) : name;
}


} // namespace cpgf


#endif

