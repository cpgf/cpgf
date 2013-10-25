#ifndef CPGF_CPPCONTEXT_H
#define CPGF_CPPCONTEXT_H

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
class CppSourceFile;
class Project;

class CppContext
{
private:
	typedef std::vector<CppItem *> ItemListType;
	typedef std::map<std::string, CppItem *> ItemNameMapType;

public:
	explicit CppContext(const Project * project);
	~CppContext();
	
	void process(const CppSourceFile & sourceFile);
	
	CppFile * getCppFile() const { return this->cppFile.get(); }
	const Project * getProject() const { return this->project; }
	
	CppItem * findItemByDecl(const clang::Decl * decl) const;
	CppClass * findClassByType(const CppType & cppType) const;

	CppClassTraits getClassTraits(const CppClass * cppClass) const;
	
private:
	std::string getDeclKeyName(const clang::Decl * decl) const;
	void beginFile(clang::Decl * decl);
	void endFile();
	
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
