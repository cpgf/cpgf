#ifndef __METACONTEXT_H
#define __METACONTEXT_H

#include "cppitem.h"

#include "cpgf/gscopedptr.h"

#include "boost/filesystem.hpp"

#include <vector>

class CppType;
class FileInfo;

class MetaContext
{
private:
	typedef std::vector<CppType *> TypeListType;
	typedef std::vector<CppItem *> ItemListType;
	typedef std::vector<FileInfo *> FileInfoListType;

public:
	MetaContext();
	~MetaContext();
	
	FileInfo * getCurrentFileInfo() const { return this->currentFileInfo; }
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
	FileInfo * currentFileInfo;
	TypeListType typeList;
	ItemListType itemList;
	FileInfoListType fileInfoList;
};


#endif
