#ifndef Rect2D_H
#define Rect2D_H

#include "Math/Vector3.h"
#include "ShaderProgram.h"

class Rect2D
{
	ShaderProgram* _shaderProgram;
	
	GLint _vertexBufferID;
	GLuint _uvBufferID;
	GLuint _vertexCount;

	//Begin : Texture related
	GLuint _textureID;
	GLuint _textureWidth;
	GLuint _textureHeight;
	//End : Texture related


	//Begin : Texture related
	GLuint LoadFile(string filePath);
	GLuint GenerateGLTextureID(void* data, int width, int height, int bytesPP);
	//End : Texture related

public:
	float x;
	float y;
	float w;
	float h;

	Rect2D();
	Rect2D(float x, float y, float w, float h);
	void SetBoundsByPoints(float x1, float y1, float x2, float y2);
	void FitIntoBoundry(float bx, float by, float bw, float bh);
	void Draw();
	void DrawWithLines();

	~Rect2D();
};

#endif