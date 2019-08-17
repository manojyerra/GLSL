#ifndef GLMemory_H
#define GLMemory_H

#include "GLInclude.h"
#include <string>
#include <map>

#define GLCreateBuffer(a, b)					GLMemory::CreateBuffer(a, b, __FILE__, __LINE__)
#define GLCreateTexture(a, b, c, d, e, f)		GLMemory::CreateTexture(a, b, c, d, e, f, __FILE__, __LINE__)
#define GLCreateMSTexture(a, b, c)				GLMemory::CreateMSTexture(a, b, c, __FILE__, __LINE__)
#define GLCreateRenderBuffer(a, b, c)			GLMemory::CreateRenderBuffer(a, b, c, __FILE__, __LINE__)
#define GLCreateMSRenderBuffer(a, b, c)			GLMemory::CreateMSRenderBuffer(a, b, c, __FILE__, __LINE__)
#define GLCreateFrameBuffer						GLMemory::CreateFrameBuffer(__FILE__, __LINE__)

#define GLDeleteBuffer(a)			GLMemory::DeleteBuffer(a)
#define GLDeleteTexture(a)			GLMemory::DeleteTexture(a)
#define GLDeleteRenderBuffer(a)		GLMemory::DeleteRenderBuffer(a)
#define GLDeleteFrameBuffer(a)		GLMemory::DeleteFrameBuffer(a)

class GLMemoryInfo
{
public:
	std::string fileName;
	long lineNum;
	GLsizeiptr size;

	GLMemoryInfo(std::string fileName, long lineNum, GLsizeiptr size)
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
	static GLuint CreateFrameBuffer(const char* filePath, long lineNum);

	static GLuint CreateTexture(GLint internalFormat, GLsizei w, GLsizei h, GLenum format, GLenum type, GLvoid* buffer, const char* filePath, long lineNum);
	static GLuint CreateMSTexture(GLint internalFormat, GLsizei w, GLsizei h, const char* filePath, long lineNum);

	static GLuint CreateRenderBuffer(GLsizei width, GLsizei height, GLenum internalformat, const char* filePath, long lineNum);
	static GLuint CreateMSRenderBuffer(GLsizei width, GLsizei height, GLenum internalformat, const char* filePath, long lineNum);

	static void DeleteBuffer(GLuint id);
	static void DeleteTexture(GLuint id);
	static void DeleteRenderBuffer(GLuint id);
	static void DeleteFrameBuffer(GLuint id);

	static bool hasMemoryLeaks();
	static void printMemoryLeaks();
};

#endif
