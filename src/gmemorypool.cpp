#include "cpgf/gmemorypool.h"
#include "cpgf/gcompiler.h"
#include "pinclude/gstaticuninitializerorders.h"

#include <string.h>

using namespace std;

namespace cpgf {


namespace memorypool_internal {

class GMemoryPoolChunk
{
private:
	typedef unsigned char FreeListType;

public:
	GMemoryPoolChunk(unsigned int blockSize, unsigned int blockCount);
	~GMemoryPoolChunk();

	GMemoryPoolRange getRange() const;

	bool isFull() const;

	bool isEmpty() const;

	void * allocate();
	void free(void * p);

	bool ownsPointer(void * p) const;

	void setInit(void (*funcInit)(void * p));
	void setDeinit(void (*funcDeinit)(void * p));

private:
	void deinitAll();
	void * getMemory() const;

private:
	const unsigned int blockSize;
	const unsigned int blockCount;
	unsigned int usedCount;
	FreeListType * freeList;
	GScopedArray<char> memory;

	void (*funcInit)(void * p);
	void (*funcDeinit)(void * p);
};


GMemoryPoolRange::GMemoryPoolRange(void * start) : start(start), end(NULL) {
}

GMemoryPoolRange::GMemoryPoolRange(void * start, void * end) : start(start), end(end) {
}

GMemoryPoolRange::GMemoryPoolRange(const GMemoryPoolRange & other) : start(other.start), end(other.end) {
}

GMemoryPoolRange & GMemoryPoolRange::operator = (const GMemoryPoolRange & other) {
	this->start = other.start;
	this->end = other.end;

	return *this;
}

bool GMemoryPoolRange::operator < (const GMemoryPoolRange & other) const {
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


inline void * poolAddPointer(void * p, int offset)
{
	return static_cast<char *>(p) + offset;
}

GMemoryPoolChunk::GMemoryPoolChunk(unsigned int blockSize, unsigned int blockCount)
	: blockSize(blockSize), blockCount(blockCount), usedCount(0), memory(new char[blockSize * blockCount + sizeof(FreeListType) * blockCount]),
		funcInit(NULL), funcDeinit(NULL) {
	GASSERT(blockCount > 1);
	GASSERT(blockCount <= (1 << (sizeof(FreeListType) * 8)));

	this->freeList = (FreeListType *)(this->memory.get());

	for(unsigned int i = 0; i < this->blockCount; ++i) {
		this->freeList[i] = static_cast<FreeListType>(i);
	}
}

GMemoryPoolChunk::~GMemoryPoolChunk() {
	this->deinitAll();
}

GMemoryPoolRange GMemoryPoolChunk::getRange() const {
	return GMemoryPoolRange(this->getMemory(),
		poolAddPointer(this->getMemory(), this->blockCount * this->blockSize));
}

bool GMemoryPoolChunk::isFull() const {
	return this->usedCount == this->blockCount;
}

bool GMemoryPoolChunk::isEmpty() const {
	return this->usedCount == 0;
}

void * GMemoryPoolChunk::allocate() {
	if(! this->isFull()) {
		void * p = poolAddPointer(
			this->getMemory(),
			this->freeList[this->usedCount++] * this->blockSize)
		;
		if(this->funcInit != NULL) {
			this->funcInit(p);
		}
		return p;
	}
	else {
		return NULL;
	}
}

void GMemoryPoolChunk::free(void * p) {
	if(this->ownsPointer(p)) {
		void * m = this->getMemory();

		// GCC will give "invalid use of void" error if we don't convert to char *
		long offset = static_cast<long>((char *)p - (char *)m);
			
		GASSERT(offset % this->blockSize == 0);

		if(this->funcDeinit != NULL) {
			this->funcDeinit(p);
		}

		--this->usedCount;
		this->freeList[this->usedCount] = static_cast<FreeListType>(offset / this->blockSize);

#if G_DEBUG
		memset(p, 0xbd, this->blockSize);
#endif

	}
}

bool GMemoryPoolChunk::ownsPointer(void * p) const {
	void * m = this->getMemory();
	return usedCount > 0 && p >= m && p < poolAddPointer(m, this->blockSize * this->blockCount);
}

void GMemoryPoolChunk::setInit(void (*funcInit)(void * p)) {
	this->funcInit = funcInit;
}

void GMemoryPoolChunk::setDeinit(void (*funcDeinit)(void * p)) {
	this->funcDeinit = funcDeinit;
}

void GMemoryPoolChunk::deinitAll() {
	if(this->funcDeinit != NULL && this->usedCount > 0) {
		bool usedList[(1 << (sizeof(FreeListType) * 8))];
		for(unsigned int i = 0; i < this->blockCount; ++i) {
			usedList[i] = true;
		}
		for(unsigned int i = this->usedCount; i < this->blockCount; ++i) {
			usedList[this->freeList[i]] = false;
		}
		for(unsigned int i = 0; i < this->blockCount; ++i) {
			if(usedList[i]) {
				void * p = poolAddPointer(
					this->getMemory(),
					this->freeList[i] * this->blockSize)
				;
				this->funcDeinit(p);
			}
		}
	}
}

void * GMemoryPoolChunk::getMemory() const {
	return (char *)(this->memory.get()) + sizeof(FreeListType) * this->blockCount;
}



GMemoryPoolImplement::GMemoryPoolImplement(unsigned int blockSize, unsigned int blockCount)
	: blockSize(blockSize), blockCount(blockCount),
		funcInit(NULL), funcDeinit(NULL) {
}

GMemoryPoolImplement::~GMemoryPoolImplement() {
	this->freeAll();
}

void * GMemoryPoolImplement::allocate() {
	if(this->freeMap.empty()) {
		GMemoryPoolChunk * chunk = new GMemoryPoolChunk(this->blockSize, this->blockCount);
		chunk->setInit(this->funcInit);
		chunk->setDeinit(this->funcDeinit);

		this->freeMap[chunk->getRange()] = chunk;
		return chunk->allocate();
	}
	else {
		MapType::iterator it = this->freeMap.begin();
		GMemoryPoolChunk * chunk = it->second;
		void * p = chunk->allocate();
		if(chunk->isFull()) {
			this->freeMap.erase(it);
			this->fullMap[chunk->getRange()] = chunk;
		}
		return p;
	}
}

void GMemoryPoolImplement::free(void * p) {
	GMemoryPoolRange range(p);
	MapType::iterator it;
		
	it = this->fullMap.find(range);
	if(it != this->fullMap.end()) {
		GMemoryPoolChunk * chunk = it->second;
		this->fullMap.erase(it);
		chunk->free(p);
		this->freeMap[chunk->getRange()] = chunk;
	}
	else {
		it = this->freeMap.find(range);
		if(it != this->freeMap.end()) {
			GMemoryPoolChunk * chunk = it->second;
			chunk->free(p);
			if(chunk->isEmpty()) {
				this->freeMap.erase(chunk->getRange());
				delete chunk;
			}
		}
		else {
			GASSERT(false);
		}
	}
}

void GMemoryPoolImplement::setInit(void (*funcInit)(void * p)) {
	this->funcInit = funcInit;
}

void GMemoryPoolImplement::setDeinit(void (*funcDeinit)(void * p)) {
	this->funcDeinit = funcDeinit;
}

void GMemoryPoolImplement::freeAll() {
	this->freeAllMap(&this->fullMap);
	this->freeAllMap(&this->freeMap);
}

void GMemoryPoolImplement::freeAllMap(MapType * m) {
	for(MapType::iterator it = m->begin(); it != m->end(); ++it) {
		delete it->second;
	}
	m->clear();
}


} // namespace memorypool_internal


class GMemoryPoolManagerImplement
{
private:
	class PoolMapKey {
	public:
		PoolMapKey()
			: blockSize(0), blockCount(0)
		{
		}

		PoolMapKey(unsigned int blockSize, unsigned int blockCount)
			: blockSize(blockSize), blockCount(blockCount)
		{
		}

		bool operator < (const PoolMapKey & other) const {
			return this->blockSize < other.blockSize
				|| (this->blockSize == other.blockSize && this->blockCount < other.blockCount);
		}

	private:
		unsigned int blockSize;
		unsigned int blockCount;
	};


	typedef map<PoolMapKey, GMemoryPool *> PoolMapType;

public:
	~GMemoryPoolManagerImplement() {
		for(PoolMapType::iterator it = this->poolMap.begin(); it != this->poolMap.end(); ++it) {
			delete it->second;
		}
	}

	GMemoryPool * getMemoryPool(unsigned int blockSize, unsigned int blockCount) {
		PoolMapKey key(blockSize, blockCount);
		GMemoryPool * pool = this->poolMap[key];
		if(pool == NULL) {
			pool = new GMemoryPool(blockSize, blockCount);
			this->poolMap[key] = pool;
		}
		return pool;
	}

private:
	PoolMapType poolMap;
};

namespace {

GMemoryPoolManager * globalPool = NULL;

} // unnamed namespace

GMemoryPoolManager * GMemoryPoolManager::getGlobal()
{
	if(globalPool == NULL && isLibraryLive()) {
		globalPool = new GMemoryPoolManager();
		addOrderedStaticUninitializer(suo_MemoryPool, makeUninitializerDeleter(&globalPool));
	}

	return globalPool;
}

GMemoryPoolManager::GMemoryPoolManager()
	: implement(new GMemoryPoolManagerImplement)
{
}

GMemoryPoolManager::~GMemoryPoolManager()
{
}

GMemoryPool * GMemoryPoolManager::getMemoryPool(unsigned int blockSize, unsigned int blockCount)
{
	return this->implement->getMemoryPool(blockSize, blockCount);
}


G_GUARD_LIBRARY_LIFE


} // namespace cpgf
