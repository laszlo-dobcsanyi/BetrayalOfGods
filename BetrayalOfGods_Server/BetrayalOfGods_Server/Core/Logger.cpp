#include "Core\Logger.h"

int Logger::level = 9;
bool Logger::only = false;

void Logger::Write(const int& _level, const std::string& _msg)
{
	if (((!only) && (_level <= level)) || ((only) && (_level == level))) std::cout << _msg.c_str() << std::endl;
}
