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
	void info(const std::string & message) const;
	void warn(const std::string & message) const;
	void error(const std::string & message) const;

private:
	void doLog(LogLevel leve, const std::string & message) const;
};

const Logger & getLogger();


} // namespace metagen


#endif
