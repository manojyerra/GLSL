#ifndef Triangle2D_H
#define Triangle2D_H

#include "Math/Vector3.h"
#include "ShaderProgram.h"

class Triangle2D
{
	ShaderProgram* shaderProgram;
	GLuint bufferID;

public:
	Triangle2D(CVector3 v1, CVector3 v2, CVector3 v3);
	void Draw();
	~Triangle2D();
};

#endif