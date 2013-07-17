#ifndef __BUILDERITEM_H
#define __BUILDERITEM_H

#include "model/cppitem.h"

#include "cpgf/gflags.h"

namespace metagen {


class BuilderWriter;
class BuilderContainer;
class CodeBlock;
class Project;

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
	bool shouldSkipBind() const; // used by project script
	
	virtual bool canBind() const; // determine if the item is supported by current library

	void writeMetaData(BuilderWriter * writer);
	
	void setProject(const Project * project) { this->project = project; }
	const Project * getProject() const { return this->project; }
	
	BuilderContainer * getParent() const { return this->parent; }
	
protected:
	std::string getPolicyText() const;
	virtual std::string doGetPolicyText() const;
	
	virtual void doWriteMetaData(BuilderWriter * writer) = 0;
	
	void checkBuilderItemCategory(ItemCategory category);
	
private:
	void setParent(BuilderContainer * parent) { this->parent = parent; }
	
private:
	const CppItem * cppItem;
	const Project * project;
	cpgf::GFlags<BuilderFlags> flags;
	BuilderContainer * parent;
	
private:
	friend class BuilderContainer;
};


} // namespace metagen


#endif
