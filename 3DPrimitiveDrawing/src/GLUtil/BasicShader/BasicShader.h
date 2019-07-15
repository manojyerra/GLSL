#ifndef BasicShader_H
#define BasicShader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "glm//glm.hpp"

class BasicShader
{
private:
	unsigned int _vertexBufferID;	
	glm::vec3 _color;
	float _alpha;

	ShaderProgram* _shaderProgram;
	GLMat _modelMat;

public:
	BasicShader();
	~BasicShader();

	void SetVertexBufferID(unsigned int bufferID);
	void SetModelMatrix(float* mat);
	void SetColor(glm::vec3& color);
	void SetAlpha(float alpha);

	void Begin();
	void SetUniformsAndAttributes();
	void End();
};

#endif