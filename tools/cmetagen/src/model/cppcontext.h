#ifndef __CPPCONTEXT_H
#define __CPPCONTEXT_H

#include "cppitem.h"

#include "cpgf/gscopedptr.h"

#include <string>
#include <vector>
#include <map>

class CppFile;
class Config;

class CppContext
{
private:
	typedef std::vector<CppItem *> ItemListType;
	typedef std::map<std::string, CppNamedItem *> ItemNameMapType;

public:
	explicit CppContext(const Config * config);
	~CppContext();
	
	CppFile * getCppFile() const { return this->cppFile.get(); }
	const Config * getConfig() const { return this->config; }
	
	const CppNamedItem * findNamedItem(const std::string & qualifiedName) const;
	
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
	ItemNameMapType itemNameMap;
	const Config * config;
	
private:
	friend class ClangParserImplement;
};


#endif
