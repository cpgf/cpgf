#ifndef CPGF_BUILDERNAMESPACE_H
#define CPGF_BUILDERNAMESPACE_H

#include "buildercontainer.h"

namespace metagen {


class CppNamespace;

class BuilderNamespace : public BuilderContainer
{
private:
	typedef BuilderContainer super;
	
public:
	explicit BuilderNamespace(const CppItem * cppItem);
	virtual ~BuilderNamespace();

	const CppNamespace * getCppNamespace() const;
		
protected:
	virtual void doWriteMetaData(BuilderWriter * writer);
};


} // namespace metagen


#endif
