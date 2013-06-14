#ifndef __METACONTEXT_H
#define __METACONTEXT_H

#include "cppitem.h"

#include "cpgf/gscopedptr.h"

#include <vector>

class CppType;
class CppFile;

class MetaContext
{
private:
	typedef std::vector<CppType *> TypeListType;
	typedef std::vector<CppItem *> ItemListType;
	typedef std::vector<CppFile *> FileInfoListType;

public:
	MetaContext();
	~MetaContext();
	
	CppFile * getCurrentFileInfo() const { return this->currentFileInfo; }
	void beginFile(const char * fileName);
	void endFile(const char * fileName);

	CppType * createType();

	template <typename T>
	T * createItem() {
		T * item = new T();
		this->itemList.push_back(item);
		return item;
	}
	
private:
	CppFile * currentFileInfo;
	TypeListType typeList;
	ItemListType itemList;
	FileInfoListType fileInfoList;
};


#endif
