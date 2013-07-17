#ifndef __BUILDERDESTRUCTOR_H
#define __BUILDERDESTRUCTOR_H

#include "builderitem.h"

namespace metagen {


class CppDestructor;

class BuilderDestructor : public BuilderItem
{
private:
	typedef BuilderItem super;
	
public:
	explicit BuilderDestructor(const CppItem * cppItem);
	virtual ~BuilderDestructor();

	const CppDestructor * getCppDestructor() const;
	
	virtual bool canBind() const;

protected:
	virtual void doWriteMetaData(BuilderWriter * writer);
};


} // namespace metagen


#endif
