#include "cpgf/gmetaenum.h"

#include <vector>


#if defined(_MSC_VER)
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#endif


namespace cpgf {

namespace meta_internal {


void splitToken(const char * buffer, std::vector<std::string> * tokens, bool excludeQualifyName)
{
	if(buffer == NULL) {
		return;
	}
	
	const char * p = buffer;
	const char * end = buffer + strlen(buffer);

	for(;;) {
        while(!isCSymbole(*p)) {
            if(!*p) {
                return;
            }
            ++p;
        }

		const char * next = strchr(p, ',');
		if(!next) {
			next = end;
		}

		if(excludeQualifyName) {
			const char * reverse = next;

			for(;;) {
				--reverse;
				if(reverse == p) {
					break;
				}

				if(isCSymbole(*reverse)) {
					++reverse;
					break;
				}
			}

			for(;;) {
				--reverse;
				if(reverse == p) {
					break;
				}

				if(!isCSymbole(*reverse)) {
					++reverse;
					break;
				}
			}

			p = reverse;
		}

		const char * t = next;
		--t;
		while(!isCSymbole(*t)) {
			if(t == p) {
				break;
			}
			--t;
		}
		tokens->push_back(std::string(p, t - p + 1));
		p = next;

		if(!*p) {
			break;
		}
	}
}


class GMetaEnumDataImplement
{
public:
	std::vector<long long> enumerators;
	std::vector<std::string> keyNameList;
};


GMetaEnumData::GMetaEnumData(const char * keyNames, size_t typeSize)
	: implement(new GMetaEnumDataImplement), typeSize(typeSize)
{
	splitToken(keyNames, &this->implement->keyNameList, true);
}

GMetaEnumData::~GMetaEnumData()
{
}

size_t GMetaEnumData::getTypeSize() const
{
	return this->typeSize;
}

size_t GMetaEnumData::getCount() const
{
	return this->implement->enumerators.size();
}

const char * GMetaEnumData::getKey(size_t index) const
{
	if(index >= this->getCount()) {
		return NULL;
	}
	else {
		return this->implement->keyNameList[index].c_str();
	}
}

long long GMetaEnumData::getValue(size_t index) const
{
	if(index >= this->getCount()) {
		return 0;
	}
	else {
		return this->implement->enumerators[index];
	}
}

int GMetaEnumData::findKey(const char * key) const
{
	size_t count = this->getCount();

	for(size_t i = 0; i < count; ++i) {
		if(strcmp(this->getKey(i), key) == 0) {
			return static_cast<int>(i);
		}
	}

	return -1;
}

void GMetaEnumData::addEnum(const char * key, long long value)
{
	this->implement->keyNameList.push_back(key);
	this->implement->enumerators.push_back(value);
}

void GMetaEnumData::addValue(long long value)
{
	this->implement->enumerators.push_back(value);
}

void * GMetaEnumData::createInstance() const
{
	return newZeroBuffer(NULL, this->getTypeSize(), NULL);
}

void * GMetaEnumData::createInplace(void * placement) const
{
	return newZeroBuffer(placement, this->getTypeSize(), NULL);
}

void * GMetaEnumData::cloneInstance(void * instance) const
{
	return newZeroBuffer(NULL, this->getTypeSize(), instance);
}

void * GMetaEnumData::cloneInplace(void * instance, void * placement) const
{
	return newZeroBuffer(placement, this->getTypeSize(), instance);
}

void GMetaEnumData::destroyInstance(void * instance) const
{
	delete static_cast<char *>(instance);
}



} // namespace meta_internal



} // namespace cpgf
