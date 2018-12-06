#ifndef Texture2D_H
#define Texture2D_H

#include "Math/Vector3.h"
#include "ShaderProgram.h"

class Texture2D
{
	float x;
	float y;
	float w;
	float h;

	ShaderProgram* _shaderProgram;
	
	GLint _vertexBufferID;
	GLuint _uvBufferID;
	GLuint _vertexCount;

	GLuint _textureID;
	GLuint _textureWidth;
	GLuint _textureHeight;

	GLuint LoadFile(string filePath);
	GLuint GenerateGLTextureID(void* data, int width, int height, int bytesPP);

public:
	Texture2D(string texturePath, float x, float y, float w, float h);
	
	void SetBoundsByPoints(float x1, float y1, float x2, float y2);
	void FitIntoBoundry(float bx, float by, float bw, float bh);
	void Draw();

	~Texture2D();
};

#endif