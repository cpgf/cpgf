#include "cpgf/gglobal.h"


namespace cpgf {

namespace {

bool libraryIsActive = true;

} // unnamed namespace

void shutDownLibrary()
{
	libraryIsActive = false;
}

bool isLibraryLive()
{
	return libraryIsActive;
}


} // namespace cpgf
