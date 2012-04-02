#ifndef __GMETAARCHIVECOMMONIMPL_H
#define __GMETAARCHIVECOMMONIMPL_H


#include "cpgf/gflags.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gexception.h"

#include "cpgf/gstringmap.h"


#include <stack>


namespace cpgf {


class GBaseClassMap
{
private:
	typedef GStringMap<void *, GStringMapReuseKey> MapType;

public:
	bool hasMetaClass(void * instance, IMetaClass * metaClass) const;
	void addMetaClass(void * instance, IMetaClass * metaClass);

private:
	MapType itemMap;
};


class GClassSerializeHeader
{
private:
	typedef std::stack<bool> SType;
	
public:
	void beginSerialize() {
		this->headerStack.push(false);
	}
	
	void endSerialize() {
		this->headerStack.pop();
	}
	
	bool needBegin() const {
		return (! this->headerStack.empty()) && ! this->headerStack.top();
	}
	
	bool needEnd() const {
		return ! this->needBegin();
	}
	
	void addedHeader() {
		if(this->needBegin()) {
			this->headerStack.pop();
			this->headerStack.push(true);
		}
	}
	
private:
	SType headerStack;
	
private:
	friend class GClassSerializeHeaderGuard;	
};


class GClassSerializeHeaderGuard
{
public:
	explicit GClassSerializeHeaderGuard(GClassSerializeHeader * header)
		: header(header)
	{
		this->header->beginSerialize();
	}
	
	~GClassSerializeHeaderGuard()
	{
		this->header->endSerialize();
	}
	
private:
	GClassSerializeHeader * header;	
};


} // namespace cpgf


#endif
