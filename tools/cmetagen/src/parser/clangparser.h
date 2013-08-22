#ifndef __CLANGPARSER_H
#define __CLANGPARSER_H

#include "cpgf/gscopedptr.h"

namespace metagen {


class ClangParserImplement;
class CppContext;
class CppSourceFile;
class Project;

class ClangParser
{
public:
	explicit ClangParser(const Project * project);
	~ClangParser();

	void parse(CppContext * context, const CppSourceFile & sourceFile);

private:
	cpgf::GScopedPointer<ClangParserImplement> implement;
	const Project * project;
};


} // namespace metagen


#endif
