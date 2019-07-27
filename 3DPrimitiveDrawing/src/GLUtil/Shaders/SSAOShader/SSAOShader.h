#ifndef SSAOShader_H
#define SSAOShader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "Shader.h"

class SSAOShader : public Shader
{
private:
	unsigned int _vertexBufferID;
	unsigned int _uvBufferID;

	unsigned int _gPositionTexID;
	unsigned int _gNormalTexID;
	unsigned int _noiseTexID;

	std::vector<glm::vec3> _samples;

	ShaderProgram* _shaderProgram;

	float _alpha;
	GLMat _modelMat;

public:

	SSAOShader();
	~SSAOShader();

	void SetVertexBufferID(unsigned int bufferID);
	void SetUVBufferID(unsigned int bufferID);

	void SetGPositionTexID(unsigned int texID);
	void SetGNormalTexID(unsigned int texID);
	void SetNoiseTexID(unsigned int texID);

	void SetSamples(std::vector<glm::vec3> samples);

	void SetModelMatrix(float* mat);

	void SetAlpha(float alpha) override;
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif