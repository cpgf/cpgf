#ifndef CPGF_GMEMORYPOOL_H
#define CPGF_GMEMORYPOOL_H

#include "cpgf/gassert.h"
#include "cpgf/gscopedptr.h"
#include "cpgf/gclassutil.h"

#include <limits>
#include <memory>
#include <vector>
#include <map>

namespace cpgf {

class GMemoryPoolChunk
{
public:
	typedef unsigned char IndexType;
	static constexpr size_t MaxBlockCount = std::numeric_limits<IndexType>::max();
	
public:
	GMemoryPoolChunk(
		const size_t blockSize, // must be >= sizeof(IndexType)
		const size_t alignment,
		const size_t blockCount // must be <= MaxBlockCount
	);
	~GMemoryPoolChunk();

	GMemoryPoolChunk(const GMemoryPoolChunk & other) = delete;
	GMemoryPoolChunk(GMemoryPoolChunk && other) = default;
	GMemoryPoolChunk & operator = (const GMemoryPoolChunk & other) = delete;
	GMemoryPoolChunk & operator = (GMemoryPoolChunk && other) = default;

	void * allocate();
	void free(void * p);
	
	bool isAvailable() const {
		return this->availableCount > 0;
	}
	
	bool isFree() const {
		return this->availableCount == this->blockCount;
	}
	
	bool belongsTo(const void * p) const {
		return p >= this->buffer.get() && p < this->buffer.get() + this->chunkSize;
	}

private:
	size_t alignment;
	size_t blockCount;
	size_t alignedBlockSize;
	size_t chunkSize;
	std::unique_ptr<unsigned char> buffer;
	unsigned char * data;
	IndexType availableIndex;
	size_t availableCount;
};

class GMemorySizedPool
{
public:
	GMemorySizedPool(
		const size_t blockSize,
		const size_t alignment,
		const size_t blockCount
	);
	~GMemorySizedPool();

	void * allocate();
	void free(void * p);
	
private:
	size_t blockSize;
	size_t alignment;
	size_t blockCount;
	std::vector<GMemoryPoolChunk> chunkList;
	GMemoryPoolChunk * availableChunk;
};


class GMemoryPool
{
public:
	static GMemoryPool * getInstance();
	
public:
	explicit GMemoryPool(
			const size_t alignment = 64,
			const size_t blockCountPerChunk = 256
		);
	~GMemoryPool();
	
	void * allocate(const size_t size);
	void free(void * p, const size_t size);
	
private:
	size_t alignment;
	size_t blockCountPerChunk;
	std::map<size_t, std::unique_ptr<GMemorySizedPool> > poolMap;
};


template <typename T>
class GObjectPool
{
public:
	GObjectPool() {
	}

	T * allocate() {
		void * p = GMemoryPool::getInstance()->allocate(sizeof(T));
		return new (p) T ();
	}

	void free(T * p) {
		p->~T();
		GMemoryPool::getInstance()->free(p, sizeof(T));
	}
};


template <typename T, typename... Params>
T * allocateObjectOnMemoryPool(Params && ... params)
{
	void * p = GMemoryPool::getInstance()->allocate(sizeof(T));
	return new (p) T (std::forward<Params>(params)...);
}

template <typename T>
void freeObjectOnMemoryPool(T * obj)
{
	obj->~T();
	GMemoryPool::getInstance()->free(obj, sizeof(T));
}

template <class T>
struct GMemoryPoolAllocator
{
	typedef T value_type;
	typedef T * pointer;

	GMemoryPoolAllocator()
		: heapPool(GMemoryPool::getInstance())
	{}

	template <class U>
	GMemoryPoolAllocator(const GMemoryPoolAllocator<U> & other)
		: heapPool(other.heapPool)
	{
	}
	
	T * allocate(std::size_t n)
	{
		return (T *)(this->heapPool->allocate(n));
	}
	
	void deallocate(T * p, std::size_t n)
	{
		this->heapPool->free(p, n);
	}
	
	GMemoryPool * heapPool;
};

template <class T, class U>
bool operator == (const GMemoryPoolAllocator<T> & a, const GMemoryPoolAllocator<U> & b)
{
	return a.heapPool == b.heapPool;
}

template <class T, class U>
bool operator != (const GMemoryPoolAllocator<T> & a, const GMemoryPoolAllocator<U> & b)
{
	return ! operator == (a, b);
}

template <typename T, typename... Params>
std::shared_ptr<T>  createPooledSharedPtr(Params && ... params)
{
	return std::allocate_shared<T>(GMemoryPoolAllocator<T>(), std::forward<Params>(params)...);
}




} // namespace cpgf



#endif
