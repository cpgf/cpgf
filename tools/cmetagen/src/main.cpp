#include "parser/clangparser.h"
#include "model/metacontext.h"
#include "model/cppnamespace.h"

#include <iostream>

int main(int argc, char ** argv)
{
	MetaContext context;
	ClangParser parser(&context);
	
	parser.parse("z.h");

//	context.getGlobalNamespace()->dump(std::cout, 0);

	return 0;
}
