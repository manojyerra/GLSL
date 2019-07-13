#ifndef Macros_H
#define Macros_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <map>
using namespace std;

#define glColorui(c) glColor4ub((c >> 24) & 255, (c >> 16) & 255, (c >> 8) & 255, (c) & 255)
#define glColorA(c,a) glColor4ub((c >> 24) & 255, (c >> 16) & 255, (c >> 8) & 255, a)
#define DEG_RAD 0.0174532925194f
#define RAD_DEG 57.295780490443f
#define PI_VAL 3.1415926f

#define writeConsole Macros::write

//Begin : MessageBox related

#define MESSAGE_OK						0x00000000L
#define MESSAGE_OKCANCEL				0x00000001L
#define MESSAGE_ABORTRETRYIGNORE		0x00000002L
#define MESSAGE_YESNOCANCEL				0x00000003L
#define MESSAGE_YESNO					0x00000004L
#define MESSAGE_RETRYCANCEL				0x00000005L

#define ShowMessageBox					MessageBox

//End : MessageBox related


//#ifdef _DEBUG
//   #define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
//#else
//   #define DEBUG_CLIENTBLOCK
//#endif // _DEBUG
//
//#ifdef _DEBUG
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//#endif
//
//#ifdef _DEBUG
//#define new DEBUG_CLIENTBLOCK
//#endif

#define __glBindBuffer(a, b) Macros::bindBuffer(a, b)
#define __glBufferData(a, b, c, d) Macros::bufferData(a, b, c, d, __FILE__, __LINE__)
#define __glDeleteBuffers(a, b) Macros::deleteBuffers(a, b, __FILE__, __LINE__)

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


class Macros
{
private:
	static GLuint _bufferID;
	static std::map<GLuint, GLBufferInfo> _bufferMap;

public:
	static void write(const char * pszFormat, ...);

	static void bindBuffer(GLenum target, GLuint bufferID);
	static void bufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage, const char* fileName, long lineNum);

	static void deleteBuffers(GLsizei n, const GLuint* buffers, const char* fileName, long lineNum);

	static void printBuffersInfo();
};


#endif
