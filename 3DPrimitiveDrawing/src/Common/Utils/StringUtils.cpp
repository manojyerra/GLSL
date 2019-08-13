#include "StringUtils.h"

std::vector<std::string> StringUtils::split(std::string str, char splitBy)
{
	std::vector<std::string> splitVec;

	if(str.find(splitBy) == -1)
		return splitVec;

	unsigned int startIndex = 0;
	unsigned int len = (unsigned int)str.length();
	bool justNowOneStrSplitted = false;

	for(unsigned int i=0;i<len;i++)
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
	unsigned int strLen = (unsigned int)str.length();
	unsigned int checkStrLen = (unsigned int)checkStr.length();

	if(strLen < checkStrLen)
		return false;

	for(unsigned int i=0;i<checkStrLen;i++)
	{
		if(str[i] != checkStr[i])
			return false;
	}

	return true;
}

bool StringUtils::endsWith(std::string str, std::string checkStr)
{
	unsigned int strLen = (unsigned int)str.length();
	unsigned int checkStrLen = (unsigned int)checkStr.length();

	if(strLen < checkStrLen)
		return false;

	unsigned int strLastIndex = (unsigned int)(str.length()-1);
	unsigned int checkStrLastIndex = (unsigned int)(checkStr.length()-1);

	for(unsigned int i=0;i<checkStrLen;i++)
	{
		if(str[strLastIndex-i] != checkStr[checkStrLastIndex-i])
			return false;
	}

	return true;
}

std::string StringUtils::getFolderNameFromPath(const std::string str)
{
	size_t found;
	found = str.find_last_of("/\\");

	if (found != -1)
	{
		return str.substr(0, found);
	}

	return ".";
}

double StringUtils::atof_exp(const char *p)
{
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

		//TODO : check the performance difference between math pow function and std pow function
		r += f / pow(10.0, n);
	}

	//for exponential support
	//char s1[] = "87.549e2", s2[] = "-982.47e-3", s3[] = "-54e", s4[] = "-.64e4",  //supported
	//char s5[] = "+.87e+ 6";  //not supported

	if (*p == 'e' || *p == 'E') {
		++p;

		if(*p >= '0' && *p <= '9')
		{
			unsigned int expNum = atoi_naive(p);
			r *= pow(10, expNum);
		}
		else if (*p == '+')
		{
			++p;
			unsigned int expNum = atoi_naive(p);
			r *= pow(10, expNum);
		}
		else if (*p == '-')
		{
			++p;
			unsigned int expNum = atoi_naive(p);
			r /= pow(10, expNum);
		}
	}

	if (neg) {
		r = -r;
	}

	return r;
}

float StringUtils::atof_naive(const char *p) {
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

		//TODO : check the performance difference between math function pow and std function pow
		r += f / pow(10.0, n);
	}
	if (neg) {
		r = -r;
	}

	//TODO: Casting double value to float for fixing warnings, but think of having another method specifically for double like atod 
	//TODO: Mainly using it in reading obj file, when we have to read more precision, we need double as data type.
	//TODO: Check the precision in stl file which generated by merge is sufficient for float or we need double as data type.
	return (float)r;
}

unsigned int StringUtils::atoi_naive(const char * p)
{
	unsigned int x = 0;
	while (*p) {
		x = x * 10 + (*p++ - '0');
	}
	return x;
}
