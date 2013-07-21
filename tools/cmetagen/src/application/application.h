#ifndef __APPLICATION_H
#define __APPLICATION_H

namespace metagen {


class Application
{
public:
	Application(int argc, char * argv[]);
	~Application();
	
	void run();

private:
	void doRun();
};


} // namespace metagen


#endif
