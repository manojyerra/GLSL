#include "SSAO.h"
#include <assert.h>

SSAO::SSAO(int w, int h)
{
	_w = w;
	_h = h;

	_gBufferFBO = 0;
	_gPosition = 0;
	_gNormal = 0;
	_gAlbedo = 0;

	_ssaoFBO = 0;
	_ssaoBlurFBO = 0;

	 CreateGBufferFBO(_w, _h);
	 CreateSSAOFBO(_w, _h);
	 CreateSSAOBlurFBO(_w, _h);
}

void SSAO::CreateGBufferFBO(unsigned int w, unsigned int h)
{
	unsigned int gBufferFBO;
	glGenFramebuffersEXT(1, &gBufferFBO);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, gBufferFBO);
	unsigned int gPosition, gNormal, gAlbedo;

	// position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F_ARB, w, h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, gPosition, 0);

	// normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F_ARB, w, h, 0, GL_RGB, GL_FLOAT, NULL);
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
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, _w, _h);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	_gBufferFBO = gBufferFBO;

	_gPosition = gPosition;
	_gNormal = gNormal;
	_gAlbedo = gAlbedo;
}

void SSAO::CreateSSAOFBO(unsigned int w, unsigned int h)
{
	// SSAO color buffer
	unsigned int ssaoFBO;
	glGenFramebuffersEXT(1, &ssaoFBO);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ssaoFBO);

	unsigned int ssaoColorBuffer;
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _w, _h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, ssaoColorBuffer, 0);
	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
		std::cout << "SSAO Framebuffer not complete!" << std::endl;

	_ssaoFBO = ssaoFBO;
}

void SSAO::CreateSSAOBlurFBO(unsigned int w, unsigned int h)
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
		std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	_ssaoBlurFBO = ssaoBlurFBO;
}

unsigned int SSAO::GetW()
{
	return _w;
}

unsigned int SSAO::GetH()
{
	return _h;
}

SSAO::~SSAO()
{
	////Delete resources
	//glDeleteTextures(1, &_texID);
	//glDeleteRenderbuffersEXT(1, &_depthBufID);
	////Bind 0, which means render to back buffer, as a result, fb is unbound
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	//glDeleteFramebuffersEXT(1, &_fboID);
}
