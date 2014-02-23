#ifndef CPGF_BUILDERENUM_H
#define CPGF_BUILDERENUM_H

#include "builderitem.h"


namespace metagen {


class CppEnum;

class BuilderEnum : public BuilderItem
{
private:
	typedef BuilderItem super;
	
public:
	explicit BuilderEnum(const CppItem * cppItem);
	virtual ~BuilderEnum();
	
	const CppEnum * getCppEnum() const;
	
protected:
	virtual void doWriteMetaData(BuilderWriter * writer);
};


} // namespace metagen


#endif
