#ifndef __BUILDERINVOKABLE_H
#define __BUILDERINVOKABLE_H

#include "builderitem.h"

namespace metagen {

class CppInvokable;

class BuilderInvokable : public BuilderItem
{
private:
	typedef BuilderItem super;
	
public:
	explicit BuilderInvokable(const CppItem * cppItem);
	virtual ~BuilderInvokable();
	
	virtual bool doCanBind() const;

protected:
};

std::string getInvokablePolicyText(const CppInvokable * cppInvokable, bool prefixWithComma);

} // namespace metagen


#endif
