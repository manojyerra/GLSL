#include "GLSSAOBufferBuilder.h"
#include "GLInclude.h"
#include <glm/glm.hpp>
#include <iostream>
#include <random>

GLSSAOBufferBuilder::GLSSAOBufferBuilder(int w, int h)
{
	_w = w;
	_h = h;

	_gBufferFBO = 0;

	_gPosition = 0;
	_gNormal = 0;
	_gAlbedo = 0;
	_noiseTexID = 0;

	_ssaoFBO = 0;
	_ssaoColorAttachmentID = 0;
	
	_ssaoBlurFBO = 0;
	_ssaoBlurColorAttachmentID = 0;

	CreateGBufferFBO(_w, _h);
	CreateSSAOFBO(_w, _h);
	CreateSSAOBlurFBO(_w, _h);
	GenerateSampleKernelAndNoiseTexture();
}

void GLSSAOBufferBuilder::CreateGBufferFBO(unsigned int w, unsigned int h)
{
	unsigned int gBufferFBO;
	glGenFramebuffersEXT(1, &gBufferFBO);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, gBufferFBO);
	unsigned int gPosition, gNormal, gAlbedo;

	// position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, w, h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, gPosition, 0);

	// normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, w, h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, gNormal, 0);

	// color + specular color buffer
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_TEXTURE_2D, gAlbedo, 0);

	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT };
	glDrawBuffers(3, attachments);

	// create and attach depth buffer (renderbuffer)
	unsigned int rboDepth;
	glGenRenderbuffersEXT(1, &rboDepth);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rboDepth);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, _w, _h);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
		throw new std::exception("Framebuffer not complete!");

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	_gBufferFBO = gBufferFBO;

	_gPosition = gPosition;
	_gNormal = gNormal;
	_gAlbedo = gAlbedo;
}

void GLSSAOBufferBuilder::CreateSSAOFBO(unsigned int w, unsigned int h)
{
	// SSAO color buffer
	unsigned int ssaoFBO;
	glGenFramebuffersEXT(1, &ssaoFBO);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ssaoFBO);

	unsigned int ssaoColorBuffer;
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _w, _h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, ssaoColorBuffer, 0);
	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
		throw new std::exception("SSAO Framebuffer not complete!");

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	_ssaoFBO = ssaoFBO;
	_ssaoColorAttachmentID = ssaoColorBuffer;
}

void GLSSAOBufferBuilder::CreateSSAOBlurFBO(unsigned int w, unsigned int h)
{
	unsigned int ssaoBlurFBO;
	glGenFramebuffersEXT(1, &ssaoBlurFBO);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ssaoBlurFBO);

	unsigned int ssaoColorBufferBlur;
	glGenTextures(1, &ssaoColorBufferBlur);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _w, _h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
		throw new std::exception("SSAO Blur Framebuffer not complete!");
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	_ssaoBlurFBO = ssaoBlurFBO;
	_ssaoBlurColorAttachmentID = ssaoColorBufferBlur;
}

void GLSSAOBufferBuilder::GenerateSampleKernelAndNoiseTexture()
{
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
	std::default_random_engine generator;

	for (unsigned int i = 0; i < 64; ++i)
	{
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / 64.0f;

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

	glGenTextures(1, &_noiseTexID);
	glBindTexture(GL_TEXTURE_2D, _noiseTexID);
	//TODO : replaced GL_RGB32F with GL_RGB32F_ARB. Confirm that it is correct replacement.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F_ARB, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

float GLSSAOBufferBuilder::lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

unsigned int GLSSAOBufferBuilder::GetGBufferFBO()
{
	return _gBufferFBO;
}

unsigned int GLSSAOBufferBuilder::GetSSAOFBO()
{
	return _ssaoFBO;
}

unsigned int GLSSAOBufferBuilder::GetSSAOColorAttachmentID()
{
	return _ssaoColorAttachmentID;
}

unsigned int GLSSAOBufferBuilder::GetSSAOBlurFBO()
{
	return _ssaoBlurFBO;
}

unsigned int GLSSAOBufferBuilder::GetSSAOBlurColorAttachmentID()
{
	return _ssaoBlurColorAttachmentID;
}

unsigned int GLSSAOBufferBuilder::GetGPositionTexID()
{
	return _gPosition;
}

unsigned int GLSSAOBufferBuilder::GetGNormalTexID()
{
	return _gNormal;
}

unsigned int GLSSAOBufferBuilder::GetGAlbedoTexID()
{
	return _gAlbedo;
}

unsigned int GLSSAOBufferBuilder::GetNoiseTexID()
{
	return _noiseTexID;
}

std::vector<glm::vec3> GLSSAOBufferBuilder::GetSamples()
{
	return _ssaoSamples;
}

unsigned int GLSSAOBufferBuilder::GetW()
{
	return _w;
}

unsigned int GLSSAOBufferBuilder::GetH()
{
	return _h;
}

GLSSAOBufferBuilder::~GLSSAOBufferBuilder()
{
	////Delete resources
	//glDeleteTextures(1, &_texID);
	//glDeleteRenderbuffersEXT(1, &_depthBufID);
	////Bind 0, which means render to back buffer, as a result, fb is unbound
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	//glDeleteFramebuffersEXT(1, &_fboID);
}