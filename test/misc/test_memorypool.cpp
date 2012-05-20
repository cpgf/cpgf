#include "test_misc_common.h"

#include "cpgf/gmemorypool.h"


using namespace cpgf;


namespace Test_MemoryPoool { namespace {


int counter = 0;

class TestMemoryPoolItem
{
public:
	TestMemoryPoolItem() : flag(1) {
		++counter;
	}
	
	~TestMemoryPoolItem() {
		GCHECK(this->flag);
		
		this->flag = 0;
		
		--counter;
	}
	
private:
	int flag;
};


GTEST(TestMemoryPool)
{
	const int ItemCount = 100;

	counter = 0;
	GObjectPool<TestMemoryPoolItem> pool;
	TestMemoryPoolItem * pointers[ItemCount];

	for(int i = 0; i < ItemCount; ++i) {
		pointers[i] = pool.allocate();
	}
	GEQUAL(ItemCount, counter);

	for(int i = 0; i < 50; ++i) {
		pool.free(pointers[i]);
	}
	GEQUAL(ItemCount - 50, counter);

	for(int i = 50; i < ItemCount; ++i) {
		pool.free(pointers[i]);
	}
	GEQUAL(0, counter);
}




} }

