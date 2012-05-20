#ifndef __GMEMORYPOOL_H
#define __GMEMORYPOOL_H

#include "cpgf/gassert.h"
#include "cpgf/gscopedptr.h"

#include <map>


namespace cpgf {

namespace memorypool_internal {


template <unsigned int Size>
struct GMemoryPoolTrunkAllocator_Heap
{
	void * getMemory() const {
		if(! this->memory) {
			this->memory.reset(new char[Size]);
		}

		return this->memory.get();
	}

private:
	mutable GScopedArray<char> memory;
};

template <unsigned int Size>
struct GMemoryPoolTrunkkAllocator_Inline
{
	void * getMemory() const {
		return &this->memory[0];
	}

private:
	mutable char memory[Size];
};

inline void * poolAddPointer(void * p, int offset)
{
	return static_cast<char *>(p) + offset;
}

class GMemoryPoolRange
{
public:
	explicit GMemoryPoolRange(void * start) : start(start), end(NULL) {
	}

	GMemoryPoolRange(void * start, void * end) : start(start), end(end) {
	}

	GMemoryPoolRange(const GMemoryPoolRange & other) : start(other.start), end(other.end) {
	}

	GMemoryPoolRange & operator = (const GMemoryPoolRange & other) {
		this->start = other.start;
		this->end = other.end;

		return *this;
	}

	bool operator < (const GMemoryPoolRange & other) const {
		if(this->end == NULL) {
			return this->start < other.start;
		}
		else {
			if(other.end == NULL) {
				return other.start >= this->end;
			}
			else {
				return this->start < other.start;
			}
		}
	}

private:
	void * start;
	void * end;
};

template <unsigned int BlockSize, unsigned int BlockCount, template<unsigned int> class TrunkAllocator = GMemoryPoolTrunkkAllocator_Inline>
class GMemoryPoolTrunk
{
private:
	typedef unsigned char FreeListType;

	GASSERT_STATIC(BlockCount > 1);
	GASSERT_STATIC(BlockCount <= (1 << (sizeof(FreeListType) * 8)));

public:
	GMemoryPoolTrunk() : usedCount(0) {
		for(unsigned int i = 0; i < BlockCount; ++i) {
			this->freeList[i] = static_cast<FreeListType>(i);
		}
	}

	GMemoryPoolRange getRange() const {
		return GMemoryPoolRange(this->trunkAllocator.getMemory(),
			poolAddPointer(this->trunkAllocator.getMemory(), BlockCount * BlockSize));
	}

	bool isFull() const {
		return this->usedCount == BlockCount;
	}

	bool isEmpty() const {
		return this->usedCount == 0;
	}

	void * allocate() {
		if(! this->isFull()) {
			return poolAddPointer(
				this->trunkAllocator.getMemory(),
				this->freeList[this->usedCount++] * BlockSize)
			;
		}
		else {
			return NULL;
		}
	}

	bool free(void * p) {
		if(this->ownsPointer(p)) {
			void * m = this->trunkAllocator.getMemory();

			// GCC will give "invalid use of void" error if we don't convert to char *
			long offset = static_cast<long>((char *)p - (char *)m);
			
			GASSERT(offset % BlockSize == 0);

			--this->usedCount;
			this->freeList[this->usedCount] = static_cast<FreeListType>(offset / BlockSize);

			return true;
		}

		return false;
	}

	bool ownsPointer(void * p) const {
		void * m = this->trunkAllocator.getMemory();
		return usedCount > 0 && p >= m && p < poolAddPointer(m, BlockSize * BlockCount);
	}

private:
	unsigned int usedCount;
	FreeListType freeList[BlockCount];
	TrunkAllocator<BlockSize * BlockCount> trunkAllocator;
};

template <unsigned int BlockSize, unsigned int BlockCount>
class GMemoryPoolImplement
{
private:
	typedef GMemoryPoolTrunk<BlockSize, BlockCount> TrunkType;
	typedef std::map<GMemoryPoolRange, TrunkType *> MapType;

	struct GMemoryPoolNode {
		GMemoryPoolNode() : next(NULL) {}

		TrunkType trunk;
		GMemoryPoolNode * next;
	};

public:
	GMemoryPoolImplement() : root(NULL) {
	}

	~GMemoryPoolImplement() {
		this->freeAll();
	}

	void * allocate() {
		if(this->freeMap.empty()) {
			TrunkType * trunk = new TrunkType;
			this->freeMap[trunk->getRange()] = trunk;
			return trunk->allocate();
		}
		else {
			typename MapType::iterator it = this->freeMap.begin();
			TrunkType * trunk = it->second;
			void * p = trunk->allocate();
			if(trunk->isFull()) {
				this->freeMap.erase(it);
				this->fullMap[trunk->getRange()] = trunk;
			}
			return p;
		}
	}

	void free(void * p) {
		GMemoryPoolRange range(p);
		typename MapType::iterator it;
		
		it = this->fullMap.find(range);
		if(it != this->fullMap.end()) {
			TrunkType * trunk = it->second;
			this->fullMap.erase(it);
			trunk->free(p);
			this->freeMap[trunk->getRange()] = trunk;
		}
		else {
			it = this->freeMap.find(range);
			if(it != this->freeMap.end()) {
				TrunkType * trunk = it->second;
				trunk->free(p);
				if(trunk->isEmpty()) {
					this->freeMap.erase(trunk->getRange());
					delete trunk;
				}
			}
		}
	}

private:
	void freeAll() {
		this->freeAllMap(&this->fullMap);
		this->freeAllMap(&this->freeMap);
	}

	void freeAllMap(MapType * m) {
		for(typename MapType::iterator it = m->begin(); it != m->end(); ++it) {
			delete it->second;
		}
	}

private:
	GMemoryPoolNode * root;
	MapType fullMap;
	MapType freeMap;
};


} // namespace memorypool_internal


template <unsigned int BlockSize, unsigned int BlockCount = 256>
class GMemoryPool
{
private:
	typedef memorypool_internal::GMemoryPoolImplement<BlockSize, BlockCount> ImplementType;

public:
	void * allocate() {
		return this->implement.allocate();
	}

	void free(void * p) {
		this->implement.free(p);
	}

private:
	ImplementType implement;
};


template <typename T, unsigned int ObjectSize = sizeof(T), unsigned int BlockCount = 256>
class GObjectPool
{
private:
	typedef memorypool_internal::GMemoryPoolImplement<ObjectSize, BlockCount> ImplementType;

public:
	T * allocate() {
		return new (this->implement.allocate()) T();
	}

	void free(T * p) {
		p->~T();
		this->implement.free(p);
	}

private:
	ImplementType implement;
};



} // namespace cpgf



#endif
