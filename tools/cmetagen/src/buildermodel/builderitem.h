#ifndef __BUILDERITEM_H
#define __BUILDERITEM_H

#include "model/cppitem.h"

#include "cpgf/gflags.h"


class BuilderFileWriter;
class BuilderContainer;
class CodeBlock;
class Config;

class BuilderItem
{
private:
	enum BuilderFlags {
		bfSkipBind = 1 << 0,
	};

public:
	explicit BuilderItem(const CppItem * cppItem);
	virtual ~BuilderItem();

	const CppItem * getCppItem() const;
	
	void setSkipBind(bool skip);
	bool shouldSkipBind() const; // used by config script
	
	virtual bool canBind() const; // determine if the item is supported by current library

	void writeMetaData(BuilderFileWriter * writer);
	
	void setConfig(const Config * config) { this->config = config; }
	const Config * getConfig() const { return this->config; }
	
	BuilderContainer * getParent() const { return this->parent; }
	
protected:
	std::string getPolicyText() const;
	virtual std::string doGetPolicyText() const;
	
	virtual void doWriteMetaData(BuilderFileWriter * writer) = 0;
	
	void checkBuilderItemCategory(ItemCategory category);
	
private:
	void setParent(BuilderContainer * parent) { this->parent = parent; }
	
private:
	const CppItem * cppItem;
	const Config * config;
	cpgf::GFlags<BuilderFlags> flags;
	BuilderContainer * parent;
	
private:
	friend class BuilderContainer;
};



#endif
