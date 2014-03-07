#include "cpgf/glifecycle.h"
#include "cpgf/gscopedptr.h"

#include <algorithm>
#include <vector>


using namespace std;

namespace cpgf {

namespace {

bool libraryIsActive = true;

class GOrderedStaticUninitializerManager
{
private:
	struct Item
	{
		GStaticUninitializationOrderType order;
		GStaticUninitializerType uninitializer;

		bool operator < (const Item & other) const {
			return this->order < other.order;
		}
	};

	typedef vector<Item> ListType;

public:
	~GOrderedStaticUninitializerManager() {
		sort(this->itemList.begin(), this->itemList.end());
		for(ListType::iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
			it->uninitializer();
		}
	}

	void add(GStaticUninitializationOrderType order, const GStaticUninitializerType & uninitializer)
	{
		Item item;
		item.order = order;
		item.uninitializer = uninitializer;
		this->itemList.push_back(item);
	}

private:
	ListType itemList;
};

GScopedPointer<GOrderedStaticUninitializerManager> orderedStaticUninitializerManager;

} // unnamed namespace

void shutDownLibrary()
{
	orderedStaticUninitializerManager.reset();
	libraryIsActive = false;
}

bool isLibraryLive()
{
	return libraryIsActive;
}


void addOrderedStaticUninitializer(GStaticUninitializationOrderType order, const GStaticUninitializerType & uninitializer)
{
	if(! orderedStaticUninitializerManager) {
		orderedStaticUninitializerManager.reset(new GOrderedStaticUninitializerManager());
	}

	orderedStaticUninitializerManager->add(order, uninitializer);
}

G_GUARD_LIBRARY_LIFE


} // namespace cpgf
