#ifndef __CLANGPARSER_H
#define __CLANGPARSER_H

#include "cpgf/gscopedptr.h"

class ClangParserImplement;
class MetaContext;

class ClangParser
{
public:
	explicit ClangParser(MetaContext * context);
	~ClangParser();

	void parse(const char * fileName);

private:
	cpgf::GScopedPointer<ClangParserImplement> implement;
};


#endif
