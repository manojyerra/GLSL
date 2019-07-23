#ifndef GLMeshBuilder_H
#define GLMeshBuilder_H

#include "GLInclude.h"
#include "ImageBuffer.h"

class GLMeshBuilder
{
private:
	GLuint _vertexBufferID;
	GLuint _normalBufferID;
	GLuint _uvBufferID;
	GLuint _colorBufferID;
	GLuint _indicesBufferID;
	GLuint _baseTexID;

	const GLvoid* _vertexBuffer;
	const GLvoid* _normalBuffer;
	const GLvoid* _uvBuffer;
	const GLvoid* _colorBuffer;
	const GLvoid* _indicesBuffer;
	ImageBuffer* _imageBuffer;

	GLsizeiptr _vertexBufferLen;
	GLsizeiptr _normalBufferLen;
	GLsizeiptr _uvBufferLen;
	GLsizeiptr _colorBufferLen;
	GLsizeiptr _indicesBufferLen;

public:
	GLMeshBuilder();
	~GLMeshBuilder();

	GLMeshBuilder* SetVertexBuffer(const GLvoid* buffer, GLsizeiptr len);
	GLMeshBuilder* SetNormalBuffer(const GLvoid* buffer, GLsizeiptr len);
	GLMeshBuilder* SetUVBuffer(const GLvoid* buffer, GLsizeiptr len);
	GLMeshBuilder* SetColorBuffer(const GLvoid* buffer, GLsizeiptr len);
	GLMeshBuilder* SetIndicesBuffer(const GLvoid* buffer, GLsizeiptr len);
	GLMeshBuilder* SetImageBuffer(ImageBuffer* imgBuf);
	void build();

	GLuint GetVertexBufferID();
	GLuint GetNormalBufferID();
	GLuint GetUVBufferID();
	GLuint GetColorBufferID();
	GLuint GetIndicesBufferID();
	GLuint GetBaseTexID();

	GLuint GetVertexBufferSize();
	GLuint GetIndicesBufferSize();
};

#endif