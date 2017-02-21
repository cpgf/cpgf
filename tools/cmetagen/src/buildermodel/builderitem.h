#ifndef CPGF_BUILDERITEM_H
#define CPGF_BUILDERITEM_H

#include "model/cppitem.h"
#include "indeterminatevalue.h"

#include "cpgf/gflags.h"

namespace metagen {


typedef IndeterminateValue<bool> TristateBool;
typedef IndeterminateValue<std::string> TristateString;

class BuilderWriter;
class BuilderContainer;
class CodeBlock;
class Project;

class BuilderItem
{
public:
	explicit BuilderItem(const CppItem * cppItem);
	virtual ~BuilderItem();

	const CppItem * getCppItem() const;
	
	virtual bool canBind() const; // determine if the item is supported by current library

	void writeMetaData(BuilderWriter * writer);
	
	void setProject(const Project * project) { this->project = project; }
	const Project * getProject() const { return this->project; }
	
	BuilderContainer * getParent() const { return this->parent; }

public:
	// functions used by either project script or builders.	
	void setSkipBind(bool skip);
	bool shouldSkipBind() const;
	void setWrapClass(bool wrap);
	bool shouldWrapClass() const;
	
protected:
	virtual void doWriteMetaData(BuilderWriter * writer) = 0;
	virtual bool doCanBind() const;
	
	void checkBuilderItemCategory(ItemCategory category);
	
private:
	void setParent(BuilderContainer * parent) { this->parent = parent; }
	
private:
	const CppItem * cppItem;
	const Project * project;
	BuilderContainer * parent;

private:
	TristateBool skipBind;
	TristateBool wrapClass;
	
private:
	friend class BuilderContainer;
};


} // namespace metagen


#endif
