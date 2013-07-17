#ifndef __BUILDERFIELD_H
#define __BUILDERFIELD_H

#include "builderitem.h"

namespace metagen {


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
	virtual void doWriteMetaData(BuilderWriter * writer);
	
private:
	void doWriteReflection(BuilderWriter * writer);
	void doWriteBitFieldWrapper(BuilderWriter * writer);
};


} // namespace metagen


#endif
