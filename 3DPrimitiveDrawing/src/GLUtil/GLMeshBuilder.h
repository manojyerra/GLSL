#ifndef GLMeshBuilder_H
#define GLMeshBuilder_H

#include "GLMat.h"
#include "ImageBuffer.h"

class GLMeshBuilder
{
private:
	GLuint _vertexBufferID;
	GLuint _normalBufferID;
	GLuint _uvBufferID;
	GLuint _baseTexID;

	const GLvoid* _vertexBuffer;
	const GLvoid* _normalBuffer;
	const GLvoid* _uvBuffer;
	ImageBuffer* _imageBuffer;

	GLsizeiptr _vertexBufferLen;
	GLsizeiptr _normalBufferLen;
	GLsizeiptr _uvBufferLen;

public:
	GLMeshBuilder();
	~GLMeshBuilder();

	GLMeshBuilder* SetVertexBuffer(const GLvoid* buffer, GLsizeiptr len);
	GLMeshBuilder* SetNormalBuffer(const GLvoid* buffer, GLsizeiptr len);
	GLMeshBuilder* SetUVBuffer(const GLvoid* buffer, GLsizeiptr len);
	GLMeshBuilder* SetImageBuffer(ImageBuffer* imgBuf);
	void build();

	GLuint GetVertexBufferID();
	GLuint GetNormalBufferID();
	GLuint GetUvBufferID();
	GLuint GetBaseTexID();

	GLuint GetVertexBufferSize();
};

#endif