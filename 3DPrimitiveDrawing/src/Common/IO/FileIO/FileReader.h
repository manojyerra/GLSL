#ifndef FileReader_H
#define FileReader_H

#include <stdio.h>
#include <string>
using namespace std;

class FileReader
{
private:
	FILE* fp;
	unsigned int pos;
	unsigned int length;

	char* buf;

public:
	FileReader(string filePath, string readMode);
	~FileReader();

	static unsigned int GetLength(string filePath);
	static bool IsFileExists(string filePath);

	int GetLength();
	char* GetData();
	char* ReadLine();
	void Reset();
};

#endif
