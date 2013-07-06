#ifndef __BUILDERINVOKABLE_H
#define __BUILDERINVOKABLE_H


#include "builderitem.h"


class BuilderInvokable : public BuilderItem
{
private:
	typedef BuilderItem super;
	
public:
	explicit BuilderInvokable(const CppItem * cppItem);
	virtual ~BuilderInvokable();
	
	virtual bool canBind() const;
};



#endif
