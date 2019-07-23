#include "StringUtils.h"

std::vector<std::string> StringUtils::split(std::string str, char splitBy)
{
	std::vector<std::string> splitVec;

	if(str.find(splitBy) == -1)
		return splitVec;

	int startIndex = 0;
	int len = str.length();
	bool justNowOneStrSplitted = false;

	for(int i=0;i<len;i++)
	{
		if(justNowOneStrSplitted)
		{
			if(str[i] != splitBy)
			{
				justNowOneStrSplitted = false;
				startIndex = i;
			}
		}

		if(str[i] == splitBy || i == len-1)
		{
			justNowOneStrSplitted = true;

			if(i == len-1)
				splitVec.push_back(str.substr(startIndex, i-startIndex+1));
			else
				splitVec.push_back(str.substr(startIndex, i-startIndex));

			startIndex = i;
		}
	}
		
	return splitVec;
}

bool StringUtils::startsWith(std::string str, std::string checkStr)
{
	int strLen = str.length();
	int checkStrLen = checkStr.length();

	if(strLen < checkStrLen)
		return false;

	for(int i=0;i<checkStrLen;i++)
	{
		if(str[i] != checkStr[i])
			return false;
	}

	return true;
}

bool StringUtils::endsWith(std::string str, std::string checkStr)
{
	int strLen = str.length();
	int checkStrLen = checkStr.length();

	if(strLen < checkStrLen)
		return false;

	int strLastIndex = str.length()-1;
	int checkStrLastIndex = checkStr.length()-1;

	for(int i=0;i<checkStrLen;i++)
	{
		if(str[strLastIndex-i] != checkStr[checkStrLastIndex-i])
			return false;
	}

	return true;
}


double StringUtils::atof_naive(const char *p) {
	double r = 0.0;
	bool neg = false;
	if (*p == '-') {
		neg = true;
		++p;
	}
	while (*p >= '0' && *p <= '9') {
		r = (r*10.0) + (*p - '0');
		++p;
	}
	if (*p == '.') {
		double f = 0.0;
		int n = 0;
		++p;
		while (*p >= '0' && *p <= '9') {
			f = (f*10.0) + (*p - '0');
			++p;
			++n;
		}

		//TODO : check the performance difference between math funtion pow and std function pow
		r += f / std::pow(10.0, n);
	}
	if (neg) {
		r = -r;
	}

	return r;
}

unsigned int StringUtils::atoi_naive(const char * p)
{
	unsigned int x = 0;
	while (*p) {
		x = x * 10 + (*p++ - '0');
	}
	return x;
}
