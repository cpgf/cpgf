#ifndef __BUILDERFILE_H
#define __BUILDERFILE_H

#include "buildercontainer.h"

#include <vector>


class CppFile;
class Config;
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
	BuilderFileWriterListType builderFileWriterList;
};

#endif
