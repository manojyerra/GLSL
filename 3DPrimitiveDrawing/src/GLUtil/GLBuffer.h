#ifndef GLBatch_H
#define GLBatch_H

#include "Macros.h"
#include "glm/vec3.hpp"

class GLBatch
{
private:
	unsigned int _capacity;

	GLfloat* _vertexArr;
	GLfloat* _uvArr;
	GLubyte* _colorArr;
	GLfloat* _normalArr;

	int _count;
	GLenum _mode;
	
	GLubyte _r;
	GLubyte _g;
	GLubyte _b;

	GLfloat _u;
	GLfloat _v;

	GLfloat _nx;
	GLfloat _ny;
	GLfloat _nz;

	GLuint _vertexBufferID;
	GLuint _colorBufferID;
	GLuint _uvBufferID;
	GLuint _normalBufferID;

	void ReCreateMem();

public:
	GLBatch(unsigned int capacity, bool colorDataPresent, bool uvDataPresent, bool normalsDataPresent);
	~GLBatch();

	void glBegin();

	void glColor3ub(GLubyte r, GLubyte g, GLubyte b);
	void glColor3f(float r, float g, float b);
	void glColor(unsigned int color);

	void glTexCoord2f(GLfloat u, GLfloat v);
	void glTexCoord2f(const glm::vec3& vec);

	void glNormal3f(GLfloat x, GLfloat y, GLfloat z);
	void glNormal3f(const glm::vec3& vec);

	void glVertex3f(const glm::vec3& vec);
	void glVertex3f(GLfloat x, GLfloat y, GLfloat z);

	const char* GetVertexBuffer();
	const char* GetUVBuffer();
	const char* GetColorBuffer();
	const char* GetNormalBuffer();

	unsigned int GetVertexBufferSize();
	unsigned int GetUVBufferSize();
	unsigned int GetColorBufferSize();
	unsigned int GetNormalBufferSize();
	unsigned int GetVertexCount();

	void ResetCount();
};

#endif
