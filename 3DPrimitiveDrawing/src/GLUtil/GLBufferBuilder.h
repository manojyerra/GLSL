#ifndef GLBufferBuilder_H
#define GLBufferBuilder_H

#include "GLInclude.h"
#include "ImageBuffer.h"

class GLBufferBuilder
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
	GLBufferBuilder();
	~GLBufferBuilder();

	GLBufferBuilder* SetVertexBuffer(const GLvoid* buffer, GLsizeiptr len);
	GLBufferBuilder* SetNormalBuffer(const GLvoid* buffer, GLsizeiptr len);
	GLBufferBuilder* SetUVBuffer(const GLvoid* buffer, GLsizeiptr len);
	GLBufferBuilder* SetColorBuffer(const GLvoid* buffer, GLsizeiptr len);
	GLBufferBuilder* SetIndicesBuffer(const GLvoid* buffer, GLsizeiptr len);
	GLBufferBuilder* SetImageBuffer(ImageBuffer* imgBuf);
	void build();

	GLuint GetVertexBufferID();
	GLuint GetNormalBufferID();
	GLuint GetUVBufferID();
	GLuint GetColorBufferID();
	GLuint GetIndicesBufferID();
	GLuint GetBaseTexID();

	GLsizeiptr GetVertexBufferSize();
	GLsizeiptr GetIndicesBufferSize();

	void UpdateColorBuffer(const GLvoid* buffer, GLsizeiptr len);
};

#endif