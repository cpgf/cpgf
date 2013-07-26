#ifndef __CLANGPARSER_H
#define __CLANGPARSER_H

#include "cpgf/gscopedptr.h"

namespace metagen {


class ClangParserImplement;
class CppContext;

class ClangParser
{
public:
	ClangParser();
	~ClangParser();

	void parse(CppContext * context, const char * fileName);

private:
	cpgf::GScopedPointer<ClangParserImplement> implement;
};


} // namespace metagen


#endif
