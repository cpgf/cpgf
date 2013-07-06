#ifndef __BUILDERFIELD_H
#define __BUILDERFIELD_H

#include "builderitem.h"


class CppField;

class BuilderField : public BuilderItem
{
private:
	typedef BuilderItem super;
	
public:
	explicit BuilderField(const CppItem * cppItem);
	virtual ~BuilderField();

	const CppField * getCppField() const;
		
	virtual bool canBind() const;
	
protected:
	virtual void doWriteMetaData(BuilderFileWriter * writer);
	void doWriteReflection(BuilderFileWriter * writer);
	void doWriteBitFieldWrapper(BuilderFileWriter * writer);
};

#endif
