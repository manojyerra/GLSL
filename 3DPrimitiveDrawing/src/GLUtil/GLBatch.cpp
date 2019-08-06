#include "GLBatch.h"
#include "GLMemory.h"

GLBatch::GLBatch(unsigned int capacity, bool colorDataPresent, bool uvDataPresent, bool normalsDataPresent)
{
	_capacity = capacity;

	_vertexArr = NULL;
	_uvArr = NULL;
	_colorArr = NULL;
	_normalArr = NULL;

	_vertexArr = new GLfloat[_capacity * 3];
	if (colorDataPresent)	_colorArr = new GLubyte[_capacity * 3];
	if (uvDataPresent)		_uvArr = new GLfloat[_capacity * 2];
	if (normalsDataPresent)	_normalArr = new GLfloat[_capacity * 3];

	_r = 255;
	_g = 255;
	_b = 255;

	_u = 0.0f;
	_v = 0.0f;

	_nx = 0.0f;
	_ny = 0.0f;
	_nz = 0.0f;

	_count = 0;
	_mode = 0;

	_vertexBufferID = 0;
	_colorBufferID = 0;
	_uvBufferID = 0;
	_normalBufferID = 0;
}

void GLBatch::ReCreateMem()
{
	int newCapacity = 2 * _capacity;

	GLfloat* newVertexArr = NULL;
	GLubyte* newColorArr = NULL;
	GLfloat* newUVArr = NULL;
	GLfloat* newNormalArr = NULL;

	if (_vertexArr)	newVertexArr = new GLfloat[newCapacity * 3];
	if (_colorArr)	newColorArr = new GLubyte[newCapacity * 3];
	if (_uvArr)		newUVArr = new GLfloat[newCapacity * 2];
	if (_normalArr)	newNormalArr = new GLfloat[newCapacity * 3];

	if (_vertexArr)	memcpy(newVertexArr, _vertexArr, _capacity * 3 * 4);
	if (_colorArr)	memcpy(newColorArr, _colorArr, _capacity * 3);
	if (_uvArr)		memcpy(newUVArr, _uvArr, _capacity * 2 * 4);
	if (_normalArr)	memcpy(newNormalArr, _normalArr, _capacity * 3 * 4);

	if (_vertexArr)	delete[] _vertexArr;
	if (_colorArr)	delete[] _colorArr;
	if (_uvArr)		delete[] _uvArr;
	if (_normalArr)	delete[] _normalArr;

	_vertexArr = newVertexArr;
	_colorArr = newColorArr;
	_uvArr = newUVArr;
	_normalArr = newNormalArr;

	_capacity = newCapacity;
}

void GLBatch::glBegin()
{
	_count = 0;
}

void GLBatch::glColor(unsigned int color)
{
	_r = ((color >> 24) & 255);
	_g = ((color >> 16) & 255);
	_b = ((color >> 8) & 255);
}

void GLBatch::glColor3ub(GLubyte r, GLubyte g, GLubyte b)
{
	_r = r;
	_g = g;
	_b = b;
}

void GLBatch::glColor3f(float r, float g, float b)
{
	_r = (GLubyte)(r*255.0f);
	_g = (GLubyte)(g*255.0f);
	_b = (GLubyte)(b*255.0f);
}

void GLBatch::glColor3f(const glm::vec3& color)
{
	_r = (GLubyte)(color.r * 255.0f);
	_g = (GLubyte)(color.g * 255.0f);
	_b = (GLubyte)(color.b * 255.0f);
}

void GLBatch::glTexCoord2f(const glm::vec3& vec)
{
	this->glTexCoord2f(vec.x, vec.y);
}

void GLBatch::glTexCoord2f(GLfloat u, GLfloat v)
{
	_u = u;
	_v = v;
}

void GLBatch::glNormal3f(GLfloat x, GLfloat y, GLfloat z)
{
	_nx = x;
	_ny = y;
	_nz = z;
}

void GLBatch::glNormal3f(const glm::vec3& vec)
{
	this->glNormal3f(vec.x, vec.y, vec.z);
}

void GLBatch::glVertex3f(const glm::vec3& vec)
{
	this->glVertex3f(vec.x, vec.y, vec.z);
}

void GLBatch::glVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
	if (_colorArr)
	{
		_colorArr[_count * 3 + 0] = _r;
		_colorArr[_count * 3 + 1] = _g;
		_colorArr[_count * 3 + 2] = _b;
	}

	if (_uvArr)
	{
		_uvArr[_count * 2 + 0] = _u;
		_uvArr[_count * 2 + 1] = _v;
	}

	if (_normalArr)
	{
		_normalArr[_count * 3 + 0] = _nx;
		_normalArr[_count * 3 + 1] = _ny;
		_normalArr[_count * 3 + 2] = _nz;
	}

	_vertexArr[_count * 3 + 0] = x;
	_vertexArr[_count * 3 + 1] = y;
	_vertexArr[_count * 3 + 2] = z;

	_count++;

	if (_count >= _capacity)
		ReCreateMem();
}

const char*  GLBatch::GetVertexBuffer() { return (const char*)_vertexArr; }
const char*  GLBatch::GetUVBuffer() { return (const char*)_uvArr; }
const char*  GLBatch::GetColorBuffer() { return (const char*)_colorArr; }
const char*  GLBatch::GetNormalBuffer() { return (const char*)_normalArr; }

unsigned int GLBatch::GetVertexBufferSize() { return _count * 3 * sizeof(GLfloat); }
unsigned int GLBatch::GetUVBufferSize() { return _count * 2 * sizeof(GLfloat); }
unsigned int GLBatch::GetColorBufferSize() { return _count * 3 * sizeof(GLbyte); }
unsigned int GLBatch::GetNormalBufferSize() { return _count * 3 * sizeof(GLfloat); }

unsigned int GLBatch::GetVertexCount()
{
	return _count;
}

void GLBatch::ResetCount()
{
	_count = 0;
}

GLBatch::~GLBatch()
{
	if (_vertexArr) { delete[] _vertexArr;	_vertexArr = NULL; }
	if (_colorArr) { delete[] _colorArr;	_colorArr = NULL; }
	if (_uvArr) { delete[] _uvArr;		_uvArr = NULL; }
	if (_normalArr) { delete[] _normalArr;	_normalArr = NULL; }
}

//void GLBatch::glEnd()
//{
//	if(_vertexBufferID == 0)
//	{
//		_vertexBufferID = GLCreateBuffer(_count * 3 * 4, _vertexArr);
//
//		if(_colorArr)
//		{
//			_colorBufferID = GLCreateBuffer(_count * 3, _colorArr);
//		}
//
//		if(_uvArr)
//		{
//			_uvBufferID = GLCreateBuffer(_count * 2 * 4, _uvArr);
//		}
//
//		if(_normalArr)
//		{
//			_normalBufferID = GLCreateBuffer(_count * 3 * 4, _normalArr);
//		}
//
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//		if (_vertexArr) { delete[] _vertexArr;	_vertexArr = NULL;	}
//		if(_colorArr)	{ delete[] _colorArr;	_colorArr = NULL;	}
//		if(_uvArr)		{ delete[] _uvArr;		_uvArr = NULL;		}
//		if(_normalArr)	{ delete[] _normalArr;	_normalArr = NULL;	}
//	}
//}

//void GLBatch::Draw(GLuint programID)
//{
//	glEnable(GL_BLEND);
//
//	if(_normalBufferID)
//	{
//		GLuint vertexNormalID = glGetAttribLocation(programID, "vertexNormal");
//		glEnableVertexAttribArray(vertexNormalID);
//		glBindBuffer(GL_ARRAY_BUFFER, _normalBufferID);
//		glVertexAttribPointer(
//			vertexNormalID,				// The attribute we want to configure
//			3,							// size
//			GL_FLOAT,					// type
//			GL_FALSE,					// normalized?
//			0,							// stride
//			(void*)0					// array buffer offset
//		);
//	}
//
//	if(_colorBufferID)
//	{
//		GLuint vertexColorID = glGetAttribLocation(programID, "vertexColor");
//		glEnableVertexAttribArray(vertexColorID);
//		glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
//		glVertexAttribPointer(
//			vertexColorID,              // The attribute we want to configure
//			4,                          // size
//			GL_UNSIGNED_BYTE,           // type
//			GL_TRUE,                    // normalized?
//			0,                          // stride
//			(void*)0                    // array buffer offset
//		);
//	}
//
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glBindBuffer( GL_ARRAY_BUFFER, _vertexBufferID );
//	glVertexPointer(3, GL_FLOAT, 0, (void*)0);
//
//	glDrawArrays(_mode, 0, _count);
//
//	glBindBuffer( GL_ARRAY_BUFFER, 0);
//	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);
//}