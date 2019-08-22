#ifndef BasicShader_H
#define BasicShader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "glm//glm.hpp"
#include "Shader.h"

class BasicShader : public Shader
{
private:
	glm::vec3 _color;
	ShaderProgram* _shaderProgram;

public:
	BasicShader();
	~BasicShader();

	void SetColor(glm::vec3 color);
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif