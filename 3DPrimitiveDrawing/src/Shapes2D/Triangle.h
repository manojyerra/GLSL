#ifndef Triangle_H
#define Triangle_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "glm/glm.hpp"
#include "GLBuffer.h"

class Triangle
{
	ShaderProgram* _shaderProgram;
	GLint _vertexBufferID;
	GLuint _colorBufferID;
	GLuint _vertexCount;

	GLMat _modelMat;
	GLBuffer* _glBuffer;

public:
	Triangle(glm::vec3& v1, glm::vec3& v2, glm::vec3& v3);
	~Triangle();

	void Draw();
};

#endif