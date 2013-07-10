#ifndef __BUILDERCONTEXT_H
#define __BUILDERCONTEXT_H

#include "builderitem.h"

#include "cpgf/gscopedptr.h"

#include <vector>

namespace metagen {


class CppContext;
class CppContanier;
class CppFile;
class BuilderFile;
class BuilderContainer;
class Config;

class BuilderContext
{
private:
	typedef std::vector<BuilderItem *> ItemListType;

public:
	explicit BuilderContext(const Config * config);
	~BuilderContext();

	void process(const CppContext * cppContext);

protected:
	void doProcessFile(const CppFile * cppFile);

	void flatten(BuilderFile * file);
	void doFlatten(BuilderFile * file, BuilderContainer * builderContainer);

	bool shouldSkipItem(const CppItem * cppItem);
	
	BuilderItem * createItem(const CppItem * cppItem);

private:
	const Config * config;
	ItemListType itemList;
};


} // namespace metagen


#endif
