#include "splitcommandline.h"

#include <cctype>


namespace metagen {

void splitCommandLine(std::vector<std::string> * outCommands, const char * commandLine)
{
	std::string command;
	const char * p = commandLine;
	bool isInQuote = false;
	while(*p != 0 && isspace(*p)) {
		++p;
	}
	for(;;) {
		char c = *p;
		if(c == 0) {
			break;
		}
		if(c == '\\') {
			c = *++p;
			if(c == 0) {
				c = '\\';
			}
			command.push_back(c);
			++p;
		}
		else if(c == '"') {
			isInQuote = ! isInQuote;
			++p;
		}
		else if(isspace(c)) {
			if(isInQuote) {
				command.push_back(c);
				++p;
			}
			else {
				outCommands->push_back(command);
				command.clear();
				while(*p != 0 && isspace(*p)) {
					++p;
				}
			}
		}
		else {
			command.push_back(c);
			++p;
		}
	}
	if(! command.empty()) {
		outCommands->push_back(command);
	}
}


} // namespace metagen
