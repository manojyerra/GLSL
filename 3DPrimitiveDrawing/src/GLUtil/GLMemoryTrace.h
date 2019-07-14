#ifndef GLMemoryTrace_H
#define GLMemoryTrace_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>

#define __glBindBuffer(a, b) GLMemoryTrace::bindBuffer(a, b)
#define __glBufferData(a, b, c, d) GLMemoryTrace::bufferData(a, b, c, d, __FILE__, __LINE__)
#define __glDeleteBuffers(a, b) GLMemoryTrace::deleteBuffers(a, b, __FILE__, __LINE__)


class GLBufferInfo
{
public:
	std::string fileName;
	long lineNum;
	unsigned int size;

	GLBufferInfo(std::string fileName, long lineNum, unsigned int size)
	{
		this->fileName = fileName;
		this->lineNum = lineNum;
		this->size = size;
	}

	bool operator< (const GLBufferInfo& obj) const
	{
		return (obj.size < this->size);
	}
};


class GLMemoryTrace
{
private:
	static GLuint _bufferID;
	static std::map<GLuint, GLBufferInfo> _bufferMap;

public:
	static void bindBuffer(GLenum target, GLuint bufferID);
	static void bufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage, const char* fileName, long lineNum);

	static void deleteBuffers(GLsizei n, const GLuint* buffers, const char* fileName, long lineNum);

	static void printMemoryLeaks();
	static bool hasMemoryLeaks();
};

#endif
