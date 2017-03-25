#ifndef CPGF_GSTRINGMAP_H
#define CPGF_GSTRINGMAP_H

#include "cpgf/gassert.h"
#include "cpgf/gstringutil.h"

#include <unordered_map>

#include <string.h>

namespace cpgf {

// We need to duplicate the key if the passed-in key will be freed outside.
struct GStringMapNewKey
{
	static const char * allocateKey(const char * key) {
		size_t len = strlen(key) + 1;
		char * ns = new char[len];
		memmove(ns, key, len);
		return ns;
	}

	static void freeKey(const char * key) {
		delete[] key;
	}
};

struct GStringMapReuseKey
{
	static const char * allocateKey(const char * key) {
		return key;
	}

	static void freeKey(const char * /*key*/) {
	}
};

template <typename T, typename KeyOp = GStringMapNewKey>
class GStringMap
{
private:
	typedef std::unordered_map<const char *, T, GCStringHash, GCStringEqual> MapType;

public:
	typedef typename MapType::iterator iterator;
	typedef typename MapType::const_iterator const_iterator;

public:
	~GStringMap() {
		this->clear();
	}

	iterator begin() {
		return this->stringMap.begin();
	}

	const_iterator begin() const {
		return this->stringMap.begin();
	}

	iterator end() {
		return this->stringMap.end();
	}

	const_iterator end() const {
		return this->stringMap.end();
	}

	bool hasKey(const char * key) const {
		return this->stringMap.find(key) != this->stringMap.end();
	}

	bool isEmpty() const {
		return this->stringMap.empty();
	}
	
	int getCount() const {
		return static_cast<int>(this->stringMap.size());
	}

	iterator find(const char * key) {
		return this->stringMap.find(key);
	}

	const_iterator find(const char * key) const {
		return this->stringMap.find(key);
	}

	void set(const char * key, const T & value) {
		typename MapType::iterator it = this->stringMap.find(key);
		if(it == this->stringMap.end()) {
			this->stringMap.insert(std::make_pair(KeyOp::allocateKey(key), value));
		}
		else {
			it->second = value;
		}
	}

	bool remove(const char * key) {
		typename MapType::iterator it = this->stringMap.find(key);
		if(it != this->stringMap.end()) {
			KeyOp::freeKey(it->first);
			this->stringMap.erase(it);

			return true;
		}
		else {
			return false;
		}
	}

	void clear() {
		for(typename MapType::iterator it = this->stringMap.begin(); it != this->stringMap.end(); ++it) {
			KeyOp::freeKey(it->first);
		}

		this->stringMap.clear();
	}

private:
	MapType stringMap;
};



} // namespace cpgf



#endif

