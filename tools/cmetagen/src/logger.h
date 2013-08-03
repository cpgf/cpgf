#ifndef __LOGGER_H
#define __LOGGER_H

#include <string>


namespace metagen {

class Logger
{
private:
	enum LogLevel {
		levelInfo,
		levelWarn,
		levelError
	};

public:
	void info(const std::string & message);
	void warn(const std::string & message);
	void error(const std::string & message);

private:
	void doLog(LogLevel leve, const std::string & message);
};

extern Logger logger;


} // namespace metagen


#endif
