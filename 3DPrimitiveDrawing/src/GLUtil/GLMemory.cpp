#include "GLMemory.h"
#include "Platform.h"

std::map<std::string, GLMemoryInfo> GLMemory::_memInfoMap;

GLuint GLMemory::CreateBuffer(GLsizeiptr size, GLvoid* data, const char* filePath, long lineNum)
{
	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//GLMemoryInfo memInfo(filePath, lineNum, size);
	//std::string str = "Buffer_" + std::to_string(bufferID);
	//_memInfoMap.insert(std::make_pair(str, memInfo));

	return bufferID;
}

GLuint GLMemory::CreateTexture(GLint internalFormat, GLsizei w, GLsizei h, GLenum format, GLenum type, GLvoid* buffer, const char* filePath, long lineNum)
{
	unsigned int textureID = 0;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, type, buffer);

	GLMemoryInfo memInfo(filePath, lineNum, w*h*3);
	std::string str = "Texture_" + std::to_string(textureID);
	_memInfoMap.insert(std::make_pair(str, memInfo));

	return textureID;
}

GLuint GLMemory::CreateMSTexture(int numSamples, GLint internalFormat, GLsizei w, GLsizei h, const char* filePath, long lineNum)
{
	unsigned int textureID = 0;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureID);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numSamples, internalFormat, w, h, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	GLMemoryInfo memInfo(filePath, lineNum, w * h * 3);
	std::string str = "Texture_" + std::to_string(textureID);
	_memInfoMap.insert(std::make_pair(str, memInfo));

	return textureID;
}

GLuint GLMemory::CreateRenderBuffer(GLsizei width, GLsizei height, GLenum internalformat, const char* filePath, long lineNum)
{
	unsigned int bufferID;
	glGenRenderbuffers(1, &bufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, bufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	GLMemoryInfo memInfo(filePath, lineNum, width*height * 3);
	std::string str = "RenderBuffer_" + std::to_string(bufferID);
	_memInfoMap.insert(std::make_pair(str, memInfo));

	return bufferID;
}

GLuint GLMemory::CreateMSRenderBuffer(int numSamples, GLsizei width, GLsizei height, GLenum internalformat, const char* filePath, long lineNum)
{
	unsigned int bufferID;
	glGenRenderbuffers(1, &bufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, bufferID);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, numSamples, internalformat, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	GLMemoryInfo memInfo(filePath, lineNum, width * height * 3);
	std::string str = "RenderBuffer_" + std::to_string(bufferID);
	_memInfoMap.insert(std::make_pair(str, memInfo));

	return bufferID;
}

GLuint GLMemory::CreateFrameBuffer(const char* filePath, long lineNum)
{
	unsigned int bufferID = 0;
	glGenFramebuffers(1, &bufferID);

	GLMemoryInfo memInfo(filePath, lineNum, 1);
	std::string str = "FrameBuffer_" + std::to_string(bufferID);
	_memInfoMap.insert(std::make_pair(str, memInfo));

	return bufferID;
}

void GLMemory::DeleteBuffer(GLuint id)
{
	//std::string key = "Buffer_" + std::to_string(id);

	//if (_memInfoMap.count(key) <= 0)
	//	throw std::exception("Error : BufferID not found.");

	//_memInfoMap.erase(key);

	glDeleteBuffers(1, &id);
	//glFinish();
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

	glDeleteRenderbuffers(1, &id);
	glFinish();
}

void GLMemory::DeleteFrameBuffer(GLuint id)
{
	std::string key = "FrameBuffer_" + std::to_string(id);

	if (_memInfoMap.count(key) <= 0)
		throw std::exception("Error : FrameBufferID not found.");

	_memInfoMap.erase(key);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &id);
	glFinish();
}

void GLMemory::printMemoryLeaks()
{
	if (!hasMemoryLeaks())
	{
		Platform::debugPrint("\n\nGLMemory: No memory leaks detected.\n\n");
		return;
	}

	Platform::debugPrint("\n\nGLMemoryTrace : Begin\n\n");

	std::map<std::string, GLMemoryInfo>::iterator itr;

	for (itr = _memInfoMap.begin(); itr != _memInfoMap.end(); ++itr)
	{
		Platform::debugPrint("\n FileName: %s, LineNumber: %ld", itr->second.fileName.c_str(), itr->second.lineNum);
	}

	Platform::debugPrint("\n\n\nGLMemoryTrace : End\n\n\n");
}

bool GLMemory::hasMemoryLeaks()
{
	return (_memInfoMap.size() > 0);
}





//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//if (bytesPP == 4)
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
//else if (bytesPP == 3)
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
//else if (bytesPP == 1)
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_RGB8, GL_UNSIGNED_BYTE, buffer);
//else
//	throw std::exception("Error : Failed to create texuture due to unsupported bytes per pixel");

//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

//glBindTexture(GL_TEXTURE_2D, 0);