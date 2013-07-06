#ifndef __CPPCONTEXT_H
#define __CPPCONTEXT_H

#include "cppitem.h"

#include "cpgf/gscopedptr.h"

#include <vector>

class CppFile;
class Config;

class CppContext
{
private:
	typedef std::vector<CppItem *> ItemListType;

public:
	explicit CppContext(const Config * config);
	~CppContext();
	
	CppFile * getCppFile() const { return this->cppFile.get(); }
	const Config * getConfig() const { return this->config; }
	
private:
	void beginFile(const char * fileName, clang::Decl * decl);
	void endFile(const char * fileName);
	
	template <typename T>
	T * createItem(clang::Decl * decl) {
		T * item = new T(decl);
		this->itemCreated(item);
		return item;
	}
	
	void itemCreated(CppItem * item);
	
private:
	cpgf::GScopedPointer<CppFile> cppFile;
	ItemListType itemList;
	const Config * config;
	
private:
	friend class ClangParserImplement;
};


#endif
