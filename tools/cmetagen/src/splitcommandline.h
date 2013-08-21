#ifndef __SPLITCOMMANDLINE_H
#define __SPLITCOMMANDLINE_H

#include <vector>
#include <string>

namespace metagen {

void splitCommandLine(std::vector<std::string> * outCommands, const char * commandLine);

} // namespace metagen


#endif
