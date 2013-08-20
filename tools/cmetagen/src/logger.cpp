#include "logger.h"

#include <iostream>

using namespace std;

namespace metagen {

Logger logger;

const Logger & getLogger()
{
	return logger;
}

void Logger::info(const std::string & message) const
{
	this->doLog(levelInfo, "Info: " + message);
}

void Logger::warn(const std::string & message) const
{
	this->doLog(levelWarn, "Warning: " + message);
}

void Logger::error(const std::string & message) const
{
	cerr << "Error: " << message;
}

void Logger::doLog(LogLevel leve, const std::string & message) const
{
	cout << message;
}


} // namespace metagen