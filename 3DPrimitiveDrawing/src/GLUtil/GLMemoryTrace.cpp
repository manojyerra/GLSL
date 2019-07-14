#include "GLMemoryTrace.h"

GLuint GLMemoryTrace::_bufferID = 0;
std::map<GLuint, GLBufferInfo> GLMemoryTrace::_bufferMap;

void GLMemoryTrace::bindBuffer(GLenum target, GLuint bufferID)
{
	_bufferID = bufferID;
	glBindBuffer(target, bufferID);
}

void GLMemoryTrace::bufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage, const char* fileName, long lineNum)
{
	GLBufferInfo bufferInfo(fileName, lineNum, size);
	_bufferMap.insert(std::make_pair(_bufferID, bufferInfo));

	glBufferData(target, size, data, usage);
	glBindBuffer(target, 0);
}

void GLMemoryTrace::deleteBuffers(GLsizei n, const GLuint* buffers, const char* fileName, long lineNum)
{
	_bufferMap.erase(buffers[0]);
	glDeleteBuffers(n, buffers);
	glFinish();
}

void GLMemoryTrace::printMemoryLeaks()
{
	std::map<GLuint, GLBufferInfo>::iterator itr;

	for (itr = _bufferMap.begin(); itr != _bufferMap.end(); ++itr) {
		printf("\n FileName: %s, LineNumber: %ld", itr->second.fileName.c_str(), itr->second.lineNum);
	}
}

bool GLMemoryTrace::hasMemoryLeaks()
{
	return (_bufferMap.size() > 0);
}
