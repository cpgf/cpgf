#ifndef CPGF_BUILDERCONTAINER_H
#define CPGF_BUILDERCONTAINER_H

#include "builderitem.h"

#include <vector>


namespace metagen {


class CppContainer;

class BuilderContainer : public BuilderItem
{
private:
	typedef BuilderItem super;

public:
	typedef std::vector<BuilderItem *> ItemListType;

public:
	explicit BuilderContainer(const CppItem * cppItem);
	virtual ~BuilderContainer();
	
	void addItem(BuilderItem * item);
	
	const CppContainer * getCppContainer() const;

private:
	ItemListType itemList;
};


} // namespace metagen


#endif
