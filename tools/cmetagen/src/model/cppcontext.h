#ifndef __CPPCONTEXT_H
#define __CPPCONTEXT_H

#include "cppitem.h"

#include "cpgf/gscopedptr.h"

#include <string>
#include <vector>
#include <map>

namespace metagen {


class CppFile;
class Project;

class CppContext
{
private:
	typedef std::vector<CppItem *> ItemListType;
	typedef std::map<std::string, CppNamedItem *> ItemNameMapType;

public:
	explicit CppContext(const Project * project);
	~CppContext();
	
	CppFile * getCppFile() const { return this->cppFile.get(); }
	const Project * getProject() const { return this->project; }
	
	const CppNamedItem * findNamedItem(ItemCategory category, const std::string & qualifiedName) const;
	
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
	ItemNameMapType itemNameMap[icCount];
	const Project * project;
	
private:
	friend class ClangParserImplement;
};


} // namespace metagen


#endif
