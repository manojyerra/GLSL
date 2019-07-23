#include "GLMemory.h"
#include <Windows.h>

std::map<std::string, GLMemoryInfo> GLMemory::_memInfoMap;

GLuint GLMemory::CreateBuffer(GLsizeiptr size, GLvoid* data, const char* filePath, long lineNum)
{
	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLMemoryInfo memInfo(filePath, lineNum, size);
	std::string str = "Buffer_" + std::to_string(bufferID);
	_memInfoMap.insert(std::make_pair(str, memInfo));

	return bufferID;
}

GLuint GLMemory::CreateTexture(GLsizei width, GLsizei height, unsigned int bytesPP, GLvoid* buffer, const char* filePath, long lineNum)
{
	unsigned int textureID = 0;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (bytesPP == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	else if (bytesPP == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	else if (bytesPP == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_RGB8, GL_UNSIGNED_BYTE, buffer);
	else
		throw std::exception("Error : Failed to create texuture due to unsupported bytes per pixel");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

	GLMemoryInfo memInfo(filePath, lineNum, width*height*bytesPP);
	std::string str = "Texture_" + std::to_string(textureID);
	_memInfoMap.insert(std::make_pair(str, memInfo));

	return textureID;
}

GLuint GLMemory::CreateRenderBuffer(GLsizei width, GLsizei height, GLenum internalformat, const char* filePath, long lineNum)
{
	unsigned int rbo;
	glGenRenderbuffersEXT(1, &rbo);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rbo);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, internalformat, width, height);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	GLMemoryInfo memInfo(filePath, lineNum, width*height * 3);
	std::string str = "RenderBuffer_" + std::to_string(rbo);
	_memInfoMap.insert(std::make_pair(str, memInfo));

	return rbo;
}

void GLMemory::DeleteBuffer(GLuint id)
{
	std::string key = "Buffer_" + std::to_string(id);

	if (_memInfoMap.count(key) <= 0)
		throw std::exception("Error : BufferID not found.");

	_memInfoMap.erase(key);

	glDeleteBuffers(1, &id);
	glFinish();
}

void GLMemory::DeleteTexture(GLuint id)
{
	std::string key = "Texture_" + std::to_string(id);

	if (_memInfoMap.count(key) <= 0)
		throw std::exception("Error : TextureID not found.");

	_memInfoMap.erase(key);

	glDeleteTextures(1, &id);
	glFinish();
}

void GLMemory::DeleteRenderBuffer(GLuint id)
{
	std::string key = "RenderBuffer_" + std::to_string(id);

	if (_memInfoMap.count(key) <= 0)
		throw std::exception("Error : RenderBufferID not found.");

	_memInfoMap.erase(key);

	glDeleteRenderbuffersEXT(1, &id);
	glFinish();
}

void GLMemory::printMemoryLeaks()
{
#if (defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))

	if (!hasMemoryLeaks())
	{
		OutputDebugString("\n\nGLMemory: No memory leaks detected.\n\n");
		return;
	}

	char arr[1024];
	OutputDebugString("\n\nGLMemoryTrace : Begin\n\n");

	std::map<std::string, GLMemoryInfo>::iterator itr;

	for (itr = _memInfoMap.begin(); itr != _memInfoMap.end(); ++itr)
	{
		sprintf(arr, "\n FileName: %s, LineNumber: %ld", itr->second.fileName.c_str(), itr->second.lineNum);
		OutputDebugString(arr);
	}

	OutputDebugString("\n\n\nGLMemoryTrace : End\n\n\n");

#else

	for (itr = _memInfoMap.begin(); itr != _memInfoMap.end(); ++itr)
	{
		printf("\n FileName: %s, LineNumber: %ld", itr->second.fileName.c_str(), itr->second.lineNum);
	}

#endif
}

bool GLMemory::hasMemoryLeaks()
{
	return (_memInfoMap.size() > 0);
}