#ifndef StringUtils_H
#define StringUtils_H

#include <vector>
#include <string>

class StringUtils
{
public:
	static std::vector<std::string> split(std::string str, char splitBy);
	static bool startsWith(std::string str, std::string checkStr);
	static bool endsWith(std::string str, std::string checkStr);

	static double atof_naive(const char *p);
	static unsigned int atoi_naive(const char * p);
};

#endif