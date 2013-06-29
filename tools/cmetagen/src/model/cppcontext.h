#ifndef __CPPCONTEXT_H
#define __CPPCONTEXT_H

#include "cppitem.h"

#include "cpgf/gscopedptr.h"

#include <vector>

class CppFile;

class CppContext
{
private:
	typedef std::vector<CppItem *> ItemListType;
	
public:
	typedef std::vector<CppFile *> FileListType;

public:
	CppContext();
	~CppContext();
	
	CppFile * getCurrentFileInfo() const { return this->currentFileInfo; }
	void beginFile(const char * fileName, clang::Decl * decl);
	void endFile(const char * fileName);
	
	const FileListType * getFileList() const { return &this->fileList; }

	template <typename T>
	T * createItem(clang::Decl * decl) {
		T * item = new T(decl);
		this->itemList.push_back(item);
		return item;
	}
	
private:
	CppFile * currentFileInfo;
	ItemListType itemList;
	FileListType fileList;
};


#endif
