#include "GLSSAO.h"
#include "GLBatch.h"
#include <random>
#include "Cam2D.h"
#include "GLMemory.h"
#include "GLState.h"

GLSSAO::GLSSAO(float w, float h) : GLSSAOBufferBuilder(w, h)
{
	_quadRenderer = nullptr;

	_modelMat.m[0] = w;
	_modelMat.m[5] = h;
	_modelMat.m[12] = 0;
	_modelMat.m[13] = 0;

	_noiseTexID = 0;
	_ssaoSamples.clear();
	GenerateSampleKernelAndNoiseTexture();

	_quadRenderer = CreateQuadRenderer(GLMeshRenderer::SSAO_SHADER);

	SSAOShader* ssaoShader = (SSAOShader*)_quadRenderer->GetCurrentShader();
	ssaoShader->SetGPositionTexID(GetGPositionTexID());
	ssaoShader->SetGNormalTexID(GetGNormalTexID());
	ssaoShader->SetNoiseTexID(_noiseTexID);
	ssaoShader->SetSamples(_ssaoSamples);
	ssaoShader->SetScreenSize(w, h);

	_blurQuadRenderer = CreateQuadRenderer(GLMeshRenderer::SSAO_BLUR_SHADER);
	SSAOBlurShader* ssaoBlurShader = (SSAOBlurShader*)_blurQuadRenderer->GetCurrentShader();
	ssaoBlurShader->SetSSAOInputTexID(GetSSAOColorAttachmentID());

	_texture = new GLTexture(0, 0, w, h);
	_texture->GetShader()->Set2DCamera(true);
}

void GLSSAO::GenerateSampleKernelAndNoiseTexture()
{
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
	std::default_random_engine generator;

	for (unsigned int i = 0; i < NUM_SAMPLES; ++i)
	{
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / (NUM_SAMPLES*1.0f);

		// scale samples s.t. they're more aligned to center of kernel
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		_ssaoSamples.push_back(sample);
	}

	std::vector<glm::vec3> ssaoNoise;
	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
		ssaoNoise.push_back(noise);
	}

	//TODO: GPU Memory allocation should happen only from GLMemory class. BTW delete the texture in the destructor.

	_noiseTexID = GLCreateTexture(GL_RGB32F, 4, 4, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
}

float GLSSAO::lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

GLMeshRenderer* GLSSAO::CreateQuadRenderer(unsigned int shaderType)
{
	GLBatch buffer(100, false, true, false);

	buffer.glBegin();

	buffer.glTexCoord2f(0.0f, 1.0f);
	buffer.glVertex3f(-1.0f, 1.0f, 0.0f);

	buffer.glTexCoord2f(0.0f, 0.0f);
	buffer.glVertex3f(-1.0f, -1.0f, 0.0f);

	buffer.glTexCoord2f(1.0f, 1.0f);
	buffer.glVertex3f(1.0f, 1.0f, 0.0f);

	buffer.glTexCoord2f(1.0f, 0.0f);
	buffer.glVertex3f(1.0f, -1.0f, 0.0f);

	ModelInfo modelInfo;
	modelInfo.SetVertexBuffer(buffer.GetVertexBuffer(), buffer.GetVertexBufferSize());
	modelInfo.SetUVBuffer(buffer.GetUVBuffer(), buffer.GetUVBufferSize());

	GLMeshRenderer* renderer = new GLMeshRenderer(&modelInfo, shaderType);
	renderer->SetPrimitiveType(GLMeshRenderer::triangleStrip);

	return renderer;
}

void GLSSAO::Begin()
{
	//Geometry pass
	glBindFramebuffer(GL_FRAMEBUFFER, GetGBufferFBO());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLSSAO::End()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Calculating SSAO
	glBindFramebuffer(GL_FRAMEBUFFER, GetSSAOFBO());
	glClear(GL_COLOR_BUFFER_BIT);
	DrawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Blur SSAO
	glBindFramebuffer(GL_FRAMEBUFFER, GetSSAOBlurFBO());
	glClear(GL_COLOR_BUFFER_BIT);
	DrawBlurQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int GLSSAO::GetOcclusionMap()
{
	return GetSSAOBlurColorAttachmentID();
}

void GLSSAO::DrawOcclusionMap()
{
	bool depth = GLState::GLEnable(GL_DEPTH_TEST, false);
	Cam2D::GetInstance()->SetProjection();
	_texture->GetShader()->SetTextureID(GetOcclusionMap());
	_texture->Draw();
	GLState::GLEnable(GL_DEPTH_TEST, depth);
}

void GLSSAO::DrawQuad()
{
	_quadRenderer->SetModelMatrix(_modelMat.m);
	_quadRenderer->Draw();
}

void GLSSAO::DrawBlurQuad()
{
	_blurQuadRenderer->SetModelMatrix(_modelMat.m);
	_blurQuadRenderer->Draw();
}

GLSSAO::~GLSSAO()
{
	if(_texture)
	{
		delete _texture;
		_texture = nullptr;
	}

	if(_noiseTexID)
	{
		GLDeleteTexture(_noiseTexID);
	}

	if (_quadRenderer)
	{
		delete _quadRenderer;
		_quadRenderer = nullptr;
	}

	if(_blurQuadRenderer)
	{
		delete _blurQuadRenderer;
		_blurQuadRenderer = nullptr;
	}
}
