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

	const GLvoid* _vertexArr;
	const GLvoid* _normalArr;
	const GLvoid* _uvArr;
	ImageBuffer* _imageBuffer;

	GLsizeiptr _vertexArrLen;
	GLsizeiptr _normalArrLen;
	GLsizeiptr _uvArrLen;

public:
	GLMeshBuilder();
	~GLMeshBuilder();

	GLMeshBuilder* SetVertexArray(const GLvoid* arr, GLsizeiptr len);
	GLMeshBuilder* SetNormalArray(const GLvoid* arr, GLsizeiptr len);
	GLMeshBuilder* SetUVArray(const GLvoid* arr, GLsizeiptr len);
	GLMeshBuilder* SetImageBuffer(ImageBuffer* imgBuf);
	bool build();
};

#endif