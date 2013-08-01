#ifndef __APPLICATION_H
#define __APPLICATION_H

#include "project.h"

#include <string>

namespace metagen {


class Application
{
public:
	Application(int argc, char * argv[]);
	~Application();
	
	void run();

private:
	void doRun();
	void processFiles();
	void processOnePath(const std::string & path);
	void processOneFile(const std::string & file);

private:	
	Project project;
};


} // namespace metagen


#endif
