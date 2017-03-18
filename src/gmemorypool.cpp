#include "cpgf/gmemorypool.h"
#include "cpgf/gcompiler.h"
#include "pinclude/gstaticuninitializerorders.h"

#include <thread>
#include <cassert>

using namespace std;

namespace cpgf {

constexpr size_t memoryPoolAlignment = 64;
constexpr size_t memoryPoolBlockCountPerTrunk = 256;

namespace {

size_t alignSize(const size_t size, const size_t alignment)
{
	return (size + alignment - 1) & ~(alignment - 1);
}

template <typename T>
T * alignPointer(T * p, const size_t alignment)
{
	return (T *)(alignSize((size_t)p, alignment));
}


} //unnamed namespace

GMemoryPoolChunk::GMemoryPoolChunk(
		const size_t blockSize,
		const size_t alignment,
		const size_t blockCount
	)
	:
		alignment(alignment),
		blockCount(blockCount),
		alignedBlockSize(alignSize(blockSize, alignment)),
		chunkSize(alignSize(alignedBlockSize * blockCount, alignment) + alignment - 1),
		buffer(new unsigned char[chunkSize]),
		data(alignPointer(buffer.get(), alignment)),
		availableIndex(0),
		availableCount(blockCount)
{
	unsigned char * p = data;
	for(size_t i = 1; i <= this->blockCount; ++i) {
		*(IndexType *)p = (IndexType)i;
		p += this->alignedBlockSize;
	}
}

GMemoryPoolChunk::~GMemoryPoolChunk()
{
}

void * GMemoryPoolChunk::allocate()
{
	// don't check if there is available block here
	// it's the caller's duty to ensure it.
	
	unsigned char * result = this->data + this->availableIndex * this->alignedBlockSize;
	this->availableIndex = *(IndexType *)result;
	--this->availableCount;
	
	return result;
}

void GMemoryPoolChunk::free(void * p)
{
	assert(p >= this->data);
	assert(p < this->data + this->blockCount * this->alignedBlockSize);
	assert(((unsigned char *)p - data) % this->alignedBlockSize == 0);
	
	*(IndexType *)p = this->availableIndex;
	this->availableIndex = (IndexType)(((unsigned char *)p - this->data) / this->alignedBlockSize);
	++this->availableCount;
}


GMemorySizedPool::GMemorySizedPool(
		const size_t blockSize,
		const size_t alignment,
		const size_t blockCount
	)
	:
		blockSize(blockSize),
		alignment(alignment),
		blockCount(blockCount)
{
	this->chunkList.emplace_back(blockSize, alignment, blockCount);
	this->availableChunk = &this->chunkList.back();
}

GMemorySizedPool::~GMemorySizedPool()
{
}

void * GMemorySizedPool::allocate()
{
	if(! this->availableChunk->isAvailable()) {
		this->availableChunk = nullptr;
		for(auto it = this->chunkList.rbegin(); it != this->chunkList.rend(); ++it) {
			if(it->isAvailable()) {
				this->availableChunk = &*it;
				break;
			}
		}
		if(this->availableChunk == nullptr) {
			this->chunkList.emplace_back(blockSize, alignment, blockCount);
			this->availableChunk = &this->chunkList.back();
		}
	}
	
	return this->availableChunk->allocate();
}

void GMemorySizedPool::free(void * p)
{
	GMemoryPoolChunk * chunk = nullptr;
	if(this->availableChunk->belongsTo(p)) {
		chunk = this->availableChunk;
		chunk->free(p);
	}
	else {
		for(auto it = this->chunkList.begin(); it != this->chunkList.end(); ++it) {
			if(it->belongsTo(p)) {
				chunk = &*it;
				chunk->free(p);
				break;
			}
		}
	}
	
	assert(chunk != nullptr);

	if(chunk != nullptr && chunk->isFree() && chunk != this->availableChunk) {
		// If chunk is free and the back() is not, swap chunk to back(),
		// If chunk is free and the back() is free too, free back() and swap chunk to back();

		if(chunk != &this->chunkList.back()) {
			if(this->chunkList.back().isFree()) {
				this->chunkList.pop_back();
			}
		}

		if(chunk != &this->chunkList.back()) {
			std::swap(*chunk, this->chunkList.back());
		}

//		this->availableChunk = &this->chunkList.back();
	}
}


GMemoryPool * GMemoryPool::getInstance()
{
	static GMemoryPool instance(memoryPoolAlignment, memoryPoolBlockCountPerTrunk);
	
	return &instance;
}

GMemoryPool::GMemoryPool(
		const size_t alignment,
		const size_t blockCountPerChunk
	)
	:
		alignment(alignment),
		blockCountPerChunk(blockCountPerChunk)
{
}

GMemoryPool::~GMemoryPool()
{
}

void * GMemoryPool::allocate(const size_t size)
{
	GMemorySizedPool * pool = nullptr;

	const size_t alignedSize = alignSize(size, this->alignment);

	auto it = this->poolMap.find(alignedSize);
	if(it != this->poolMap.end()) {
		pool = it->second.get();
	}
	else {
		pool = new GMemorySizedPool(alignedSize, this->alignment, this->blockCountPerChunk);
		this->poolMap.insert(std::make_pair(alignedSize, std::unique_ptr<GMemorySizedPool>(pool)));
	}
	
	return pool->allocate();
}

void GMemoryPool::free(void * p, const size_t size)
{
	const size_t alignedSize = alignSize(size, this->alignment);
	
	auto it = this->poolMap.find(alignedSize);
	if(it != this->poolMap.end()) {
		it->second->free(p);
	}
	else {
	}
}


G_GUARD_LIBRARY_LIFE


} // namespace cpgf
