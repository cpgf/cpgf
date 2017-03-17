#ifndef CPGF_GMEMORYPOOL_H
#define CPGF_GMEMORYPOOL_H

#include "cpgf/gassert.h"
#include "cpgf/gscopedptr.h"
#include "cpgf/gclassutil.h"

#include <memory>
#include <map>
#include <deque>

namespace cpgf {

enum class GMemoryPoolPurgeStrategy
{
	never,
	onSceneFreed, // after previous scene is freed and next scene is not created yet
	onSceneSwitched, // after previous scene is freed and next scene has been created
	onFree
};

class GMemorySizedPool
{
private:
	struct IdleIndex {
		int chunkIndex;
		int blockIndex;
	};
	
	struct Chunk {
		std::unique_ptr<char> rawMemory;
		void * start;
	};

	struct ChunkRange {
		ChunkRange(void * start) : start(start), end(nullptr) {
		}

		ChunkRange(void * start, void * end) : start(start), end(end) {
		}

		bool operator < (const ChunkRange & other) const {
			if(this->end == nullptr) {
				return this->start < other.start;
			}
			else {
				if(other.end == nullptr) {
					return other.start >= this->end;
				}
				else {
					return this->start < other.start;
				}
			}
		}

		void * start;
		void * end;
	};
	
public:
	GMemorySizedPool(
			const std::size_t blockSize,
			const std::size_t alignment = 64,
			const std::size_t blockCountPerChunk = 256,
			const GMemoryPoolPurgeStrategy purgeStrategy = GMemoryPoolPurgeStrategy::onSceneSwitched
		);
	
	void * allocate();
	void * allocate(const std::size_t size);
	void free(void * p);
	
	void purge();

private:
	void doPurgeChunk(const int index);

private:
	std::size_t blockSize;
	std::size_t blockTotalSize;
	std::size_t alignment;
	std::size_t blockCountPerChunk;
	std::size_t chunkSize;
	GMemoryPoolPurgeStrategy purgeStrategy;
	
	std::deque<Chunk> chunkList;
	std::deque<IdleIndex> idleList;
	std::map<ChunkRange, int> chunkMap;
};

class GMemoryPool
{
public:
	static GMemoryPool * getInstance();
	
public:
	explicit GMemoryPool(
			const std::size_t alignment = 64,
			const std::size_t blockCountPerChunk = 256,
			const GMemoryPoolPurgeStrategy purgeStrategy = GMemoryPoolPurgeStrategy::onSceneSwitched
		);
	~GMemoryPool();
	
	void * allocate(const std::size_t size);
	void free(void * p, size_t size);
	
	void purge();

	GMemoryPoolPurgeStrategy getPurgeStrategy() const { return this->purgeStrategy; }

private:
	std::size_t alignment;
	std::size_t blockCountPerChunk;
	GMemoryPoolPurgeStrategy purgeStrategy;
	std::map<std::size_t, std::unique_ptr<GMemorySizedPool> > poolMap;
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
