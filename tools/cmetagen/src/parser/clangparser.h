#ifndef __CLANGPARSER_H
#define __CLANGPARSER_H

#include "cpgf/gscopedptr.h"

class ClangParserImplement;
class CppContext;

class ClangParser
{
public:
	explicit ClangParser(CppContext * context);
	~ClangParser();

	void parse(const char * fileName);

private:
	cpgf::GScopedPointer<ClangParserImplement> implement;
};


#endif
