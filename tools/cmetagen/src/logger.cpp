#include "logger.h"

#include <iostream>

using namespace std;

namespace metagen {

Logger logger;

void Logger::info(const std::string & message)
{
	this->doLog(levelInfo, message);
}

void Logger::warn(const std::string & message)
{
	this->doLog(levelWarn, message);
}

void Logger::error(const std::string & message)
{
	cerr << message;
}

void Logger::doLog(LogLevel leve, const std::string & message)
{
	cout << message;
}


} // namespace metagen
