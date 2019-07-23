#ifndef FileReader_H
#define FileReader_H

#include <string>

class FileReader
{
private:
	FILE* fp;
	unsigned int pos;
	unsigned int length;

	char* buf;

public:
	FileReader(std::string filePath, std::string readMode);
	~FileReader();

	static unsigned int GetLength(std::string filePath);
	static bool IsFileExists(std::string filePath);

	int GetLength();
	char* GetData();
	char* ReadLine();
	void Reset();
};

#endif
