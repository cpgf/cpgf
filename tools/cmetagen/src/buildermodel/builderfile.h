#ifndef __BUILDERFILE_H
#define __BUILDERFILE_H

#include "buildercontainer.h"

#include "cpgf/gscopedptr.h"

#include <vector>

namespace metagen {


class CppFile;
class Config;
class CppWriter;
class BuilderContainer;
class BuilderFileWriter;

class BuilderFile : public BuilderContainer
{
private:
	typedef BuilderContainer super;
	typedef std::vector<BuilderFileWriter *> BuilderFileWriterListType;
	
public:
	explicit BuilderFile(const CppItem * cppItem);
	virtual ~BuilderFile();
	
	const CppFile * getCppFile() const;
	
	void prepare();
	
	void outputFiles();
	
protected:
	virtual void doWriteMetaData(BuilderFileWriter * writer);

private:
	void createFileWriters();
	
private:
	cpgf::GScopedPointer<CppWriter> headerWriter;
	BuilderFileWriterListType builderFileWriterList;
};


} // namespace metagen


#endif
