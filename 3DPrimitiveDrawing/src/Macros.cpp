#include "Macros.h"

GLuint Macros::_bufferID = 0;
std::map<GLuint, GLBufferInfo> Macros::_bufferMap;

void Macros::bindBuffer(GLenum target, GLuint bufferID)
{
	_bufferID = bufferID;
	glBindBuffer(target, bufferID);
}

void Macros::bufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage, const char* fileName, long lineNum)
{
	GLBufferInfo bufferInfo(fileName, lineNum, size);
	_bufferMap.insert(std::make_pair(_bufferID, bufferInfo));

	glBufferData(target, size, data, usage);
	glBindBuffer(target, 0);
}

void Macros::deleteBuffers(GLsizei n, const GLuint* buffers, const char* fileName, long lineNum)
{
	_bufferMap.erase(buffers[0]);
	glDeleteBuffers(n, buffers);
	glFinish();
}

void Macros::printBuffersInfo()
{
	map<GLuint, GLBufferInfo>::iterator itr;

	for (itr = _bufferMap.begin(); itr != _bufferMap.end(); ++itr) {
		printf("\n FileName: %s, LineNumber: %ld", itr->second.fileName.c_str(), itr->second.lineNum);
	}
}


///////////////////////////////////////////////////////////////////

void Macros::write(const char * pszFormat, ...)
{
	const int MAX_LEN = 10000;
	char szBuf[MAX_LEN];

	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf_s(szBuf, MAX_LEN, MAX_LEN, pszFormat, ap);
	va_end(ap);

	WCHAR wszBuf[MAX_LEN] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
	OutputDebugStringW(wszBuf);
	OutputDebugStringA("");

	//WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), szBuf, sizeof(szBuf), NULL, FALSE);
	//printf("%s\n", szBuf);
}