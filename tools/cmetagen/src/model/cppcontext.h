#ifndef __CPPCONTEXT_H
#define __CPPCONTEXT_H

#include "parser/clangparser.h"
#include "cppitem.h"
#include "cpptype.h"
#include "cppclasstraits.h"

#include "cpgf/gscopedptr.h"

#include <string>
#include <vector>
#include <map>

namespace metagen {


class CppFile;
class CppClass;
class Project;

class CppContext
{
private:
	typedef std::vector<CppItem *> ItemListType;
	typedef std::map<std::string, CppItem *> ItemNameMapType;

public:
	explicit CppContext(const Project * project);
	~CppContext();
	
	void process(const std::string & sourceFileName);
	
	CppFile * getCppFile() const { return this->cppFile.get(); }
	const Project * getProject() const { return this->project; }
	
	CppItem * findItemByDecl(const clang::Decl * decl) const;
	CppClass * findClassByType(const CppType & cppType) const;

	CppClassTraits getClassTraits(const CppClass * cppClass) const;
	
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
	const Project * project;
	ClangParser parser;
	
private:
	friend class ClangParserImplement;
};


} // namespace metagen


#endif
