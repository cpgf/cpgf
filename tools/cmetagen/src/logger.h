#ifndef CPGF_LOGGER_H
#define CPGF_LOGGER_H

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
	void print(const std::string & message) const;
	void info(const std::string & message) const;
	void warn(const std::string & message) const;
	void error(const std::string & message) const;

private:
	void doLog(LogLevel level, const std::string & message) const;
};

const Logger & getLogger();


} // namespace metagen


#endif
