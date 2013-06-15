#ifndef __BUILDEROPERATOR_H
#define __BUILDEROPERATOR_H

#include "builderitem.h"


class BuilderOperator : public BuilderItem
{
private:
	typedef BuilderItem super;
	
public:
	explicit BuilderOperator(const CppItem * cppItem);
	virtual ~BuilderOperator();
	
protected:
	virtual void doWriteMetaData(CppPairWriter * writer);
};

#endif
