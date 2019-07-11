#include "FileReader.h"
#include "Macros.h"

FileReader::FileReader(string filePath, string readMode)
{
	fp = fopen(filePath.c_str(), readMode.c_str());
	pos = 0;
	length = 0;
	buf = NULL;

    if(fp)
    {
		length = GetLength((char*)filePath.c_str());

		buf = (char*)malloc(length+1);

		memset(buf, (int)'\0', length+1);

		fread(buf, 1, length, fp);

		fclose(fp);
	}
}

unsigned int FileReader::GetLength(string filePath)
{
	FILE  *fp = fopen(filePath.c_str(), "rb");
    
    if(fp)
    {
        fseek(fp, 0, SEEK_END);
        unsigned int fileSize = (unsigned int)ftell(fp);
        fseek(fp, 0, SEEK_SET);
        fclose(fp);
        return fileSize;
    }
    
    return 0;
}

bool FileReader::IsFileExists(string filePath)
{
	FILE  *fp = fopen(filePath.c_str(), "r");
    
    if(fp)
    {
        fclose(fp);
        return true;
    }

	return false;
}

char* FileReader::GetData()
{
	return buf;
}

int FileReader::GetLength()
{
    return length;
}

void FileReader::Reset()
{
	pos = 0;
}

char* FileReader::ReadLine()
{
	if(pos >= length)
		return NULL;

	unsigned int i = pos;

	for(; i<length && buf[i] != '\n' && buf[i] != '\0'; i++);

	if(buf[i] == '\0')
		i = length;

	int lineLen = i - pos;

	if(buf[i] == '\n' || i == length)
	{
		char* lineBuf = (char*)malloc(lineLen+1);
		memset(lineBuf, (int)'\0', lineLen+1);
		memcpy(lineBuf, &buf[pos], lineLen);
		pos = i+1;
		return lineBuf;
	}

	return NULL;
}

FileReader::~FileReader()
{
	if(buf)
	{
		free(buf);
		buf = NULL;
	}
}


//char* FileReader::GetData()
//{
//    if(fp)
//    {
//        fseek(fp, 0, SEEK_END);
//        unsigned int fileSize = (unsigned int)ftell(fp);
//        
//        fseek(fp, 0, SEEK_SET);
//        char* arrData = (char*)malloc(fileSize+1);
//        memset(arrData, (int)'\0', fileSize+1);
//        
//        if(fread(arrData, 1, fileSize, fp) != fileSize)
//        {
//            free(arrData);
//            fclose(fp);
//            return NULL;
//        }
//        
//        fclose(fp);
//        return arrData;
//    }
//    
//    return NULL;
//}
