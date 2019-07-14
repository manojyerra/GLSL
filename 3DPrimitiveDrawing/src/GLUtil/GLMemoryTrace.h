#ifndef GLMemoryTrace_H
#define GLMemoryTrace_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>

#define __glBufferData(a, b, c, d, e) GLMemoryTrace::bufferData(a, b, c, d, e, __FILE__, __LINE__)
#define __glDeleteBuffers(a, b) GLMemoryTrace::deleteBuffers(a, b)

#define __glTexImage2D(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) GLMemoryTrace::texImage2D(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, __FILE__, __LINE__)
#define __glDeleteTextures(v1, v2) GLMemoryTrace::deleteTextures(v1, v2)

#define __glRenderbufferStorageEXT(v1, v2, v3, v4, v5) GLMemoryTrace::renderbufferStorageEXT(v1, v2, v3, v4, v5, __FILE__, __LINE__)
#define __glDeleteRenderbuffersEXT(v1, v2)


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
	static std::map<GLuint, GLBufferInfo> _bufferMap;
	static std::map<GLuint, GLBufferInfo> _textureMap;
	static std::map<GLuint, GLBufferInfo> _renderBufferMap;

public:
	static void bufferData(GLuint bufferID, GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage, const char* fileName, long lineNum);
	static void deleteBuffers(GLsizei n, const GLuint* buffers);

	static void texImage2D(GLuint textureID, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, 
						GLint border, GLenum format, GLenum type, const GLvoid *pixels, const char* fileName, long lineNum);
	static void deleteTextures(GLsizei n, const GLuint *textures);

	static void renderbufferStorageEXT(GLuint rboID, GLenum target, GLenum internalformat, 
						GLsizei width, GLsizei height, const char* fileName, long lineNum);

	static void deleteRenderbuffersEXT(GLsizei n, const GLuint* renderbuffers);

	static void printMemoryLeaks();
	static bool hasMemoryLeaks();
};

#endif
