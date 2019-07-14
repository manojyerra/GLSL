#include "GLMemoryTrace.h"
#include <windows.h>

std::map<GLuint, GLBufferInfo> GLMemoryTrace::_bufferMap;
std::map<GLuint, GLBufferInfo> GLMemoryTrace::_textureMap;
std::map<GLuint, GLBufferInfo> GLMemoryTrace::_renderBufferMap;


void GLMemoryTrace::bufferData(GLuint bufferID, GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage, const char* fileName, long lineNum)
{
	GLBufferInfo bufferInfo(fileName, lineNum, size);
	_bufferMap.insert(std::make_pair(bufferID, bufferInfo));

	glBufferData(target, size, data, usage);
}

void GLMemoryTrace::deleteBuffers(GLsizei n, const GLuint* buffers)
{
	if (_bufferMap.count(buffers[0]) <= 0)
		throw std::exception("Error : TextureID not found.");

	_bufferMap.erase(buffers[0]);
	glDeleteBuffers(n, buffers);
	glFinish();
}

void GLMemoryTrace::texImage2D(GLuint textureID, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height,
								GLint border, GLenum format, GLenum type, const GLvoid *pixels, const char* fileName, long lineNum)
{
	int bytesPP = (format == GL_RGBA) ? 4 : 3;

	GLBufferInfo bufferInfo(fileName, lineNum, width*height*bytesPP);
	_textureMap.insert(std::make_pair(textureID, bufferInfo));

	glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
}

void GLMemoryTrace::deleteTextures(GLsizei n, const GLuint *textures)
{
	if (_textureMap.count(textures[0]) <= 0)
		throw std::exception("Error : TextureID not found.");

	_textureMap.erase(textures[0]);
	glDeleteTextures(n, textures);
	glFinish();
}

void GLMemoryTrace::renderbufferStorageEXT(GLuint rboID, GLenum target, GLenum internalformat,
	GLsizei width, GLsizei height, const char* fileName, long lineNum)
{
	GLBufferInfo bufferInfo(fileName, lineNum, width*height*3);
	_renderBufferMap.insert(std::make_pair(rboID, bufferInfo));

	glRenderbufferStorageEXT(target, internalformat, width, height);
}

void GLMemoryTrace::deleteRenderbuffersEXT(GLsizei n, const GLuint* renderbuffers)
{
	if (_renderBufferMap.count(renderbuffers[0]) <= 0)
		throw std::exception("Error : TextureID not found.");

	_renderBufferMap.erase(renderbuffers[0]);
	glDeleteRenderbuffersEXT(n, renderbuffers);
	glFinish();
}

void GLMemoryTrace::printMemoryLeaks()
{
#if (defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))

	char arr[1024];
	OutputDebugString("\n\nGLMemoryTrace : Begin\n\n");

	std::map<GLuint, GLBufferInfo>::iterator itr;

	for (itr = _bufferMap.begin(); itr != _bufferMap.end(); ++itr) {
		sprintf(arr, "\n FileName: %s, LineNumber: %ld", itr->second.fileName.c_str(), itr->second.lineNum);
		OutputDebugString(arr);
	}

	for (itr = _textureMap.begin(); itr != _textureMap.end(); ++itr) {
		sprintf(arr, "\n FileName: %s, LineNumber: %ld", itr->second.fileName.c_str(), itr->second.lineNum);
		OutputDebugString(arr);
	}

	for (itr = _renderBufferMap.begin(); itr != _renderBufferMap.end(); ++itr) {
		sprintf(arr, "\n FileName: %s, LineNumber: %ld", itr->second.fileName.c_str(), itr->second.lineNum);
		OutputDebugString(arr);
	}

	OutputDebugString("\n\n\nGLMemoryTrace : End\n\n\n");

#else

	for (itr = _bufferMap.begin(); itr != _bufferMap.end(); ++itr) {
		printf("\n FileName: %s, LineNumber: %ld", itr->second.fileName.c_str(), itr->second.lineNum);
	}

#endif
}

bool GLMemoryTrace::hasMemoryLeaks()
{
	return (_bufferMap.size() > 0);
}
