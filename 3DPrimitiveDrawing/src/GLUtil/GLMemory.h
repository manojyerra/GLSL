#ifndef GLMemory_H
#define GLMemory_H

#include "GLInclude.h"
#include <string>
#include <map>

#define GLCreateBuffer(a, b)			GLMemory::CreateBuffer(a, b, __FILE__, __LINE__)
#define GLCreateTexture(a, b, c, d)		GLMemory::CreateTexture(a, b, c, d, __FILE__, __LINE__)
#define GLCreateRenderBuffer(a, b, c)	GLMemory::CreateRenderBuffer(a, b, c, __FILE__, __LINE__)

#define GLDeleteBuffer(a)			GLMemory::DeleteBuffer(a)
#define GLDeleteTexture(a)			GLMemory::DeleteTexture(a)
#define GLDeleteRenderBuffer(a)		GLMemory::DeleteRenderBuffer(a)

class GLMemoryInfo
{
public:
	std::string fileName;
	long lineNum;
	unsigned int size;

	GLMemoryInfo(std::string fileName, long lineNum, unsigned int size)
	{
		this->fileName = fileName;
		this->lineNum = lineNum;
		this->size = size;
	}

	bool operator< (const GLMemoryInfo& obj) const
	{
		return (obj.size < this->size);
	}
};

class GLMemory
{
private:
	static std::map<std::string, GLMemoryInfo> _memInfoMap;

public:
	static GLuint CreateBuffer(GLsizeiptr size, GLvoid* data, const char* filePath, long lineNum);
	static GLuint CreateTexture(GLsizei width, GLsizei height, unsigned int bytesPP, GLvoid* buffer, const char* filePath, long lineNum);
	static GLuint CreateRenderBuffer(GLsizei width, GLsizei height, GLenum internalformat, const char* filePath, long lineNum);

	static void DeleteBuffer(GLuint id);
	static void DeleteTexture(GLuint id);
	static void DeleteRenderBuffer(GLuint id);

	static bool hasMemoryLeaks();
	static void printMemoryLeaks();
};

#endif
