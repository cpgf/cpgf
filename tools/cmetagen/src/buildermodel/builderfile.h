#ifndef __BUILDERFILE_H
#define __BUILDERFILE_H

#include "buildercontainer.h"

#include <vector>


class Config;
class BuilderContainer;
class SplittedFile;

class BuilderFile : public BuilderContainer
{
private:
	typedef BuilderContainer super;
	typedef std::vector<SplittedFile *> SplittedFileListType;
	
public:
	explicit BuilderFile(const CppItem * cppItem);
	virtual ~BuilderFile();
	
	void setConfig(const Config * config) { this->config = config; }

	void prepare();
	
	void outputFiles();
	
protected:
	virtual void doWriteMetaData(CppPairWriter * writer);

private:
	void generateSplittedFiles();
	
private:
	const Config * config;
	SplittedFileListType splittedFileList;
};

#endif
