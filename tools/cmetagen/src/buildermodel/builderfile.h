#ifndef __BUILDERFILE_H
#define __BUILDERFILE_H

#include "buildercontainer.h"

#include "cpgf/gscopedptr.h"

#include <vector>

namespace metagen {


class CppFile;
class Project;
class CppWriter;
class BuilderContainer;
class BuilderWriter;

class BuilderFile : public BuilderContainer
{
private:
	typedef BuilderContainer super;
	
public:
	explicit BuilderFile(const CppItem * cppItem);
	virtual ~BuilderFile();
	
	const CppFile * getCppFile() const;
	
protected:
	virtual void doWriteMetaData(BuilderWriter * writer);

};


} // namespace metagen


#endif
