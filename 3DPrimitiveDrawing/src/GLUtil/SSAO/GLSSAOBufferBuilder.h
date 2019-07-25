#ifndef GLSSAOBufferBuilder_H
#define GLSSAOBufferBuilder_H

#include <vector>
#include <glm/vec3.hpp>

class GLSSAOBufferBuilder
{
private:
	unsigned int _w;
	unsigned int _h;

	unsigned int _gBufferFBO;
	unsigned int _gPosition;
	unsigned int _gNormal;
	unsigned int _gAlbedo;
	unsigned int _noiseTexID;

	unsigned int _ssaoFBO;
	unsigned int _ssaoColorAttachmentID;

	unsigned int _ssaoBlurFBO;
	unsigned int _ssaoBlurColorAttachmentID;

	std::vector<glm::vec3> _ssaoSamples;

	void CreateGBufferFBO(unsigned int w, unsigned int h);
	void CreateSSAOFBO(unsigned int w, unsigned int h);
	void CreateSSAOBlurFBO(unsigned int w, unsigned int h);
	void GenerateSampleKernelAndNoiseTexture();

	float lerp(float a, float b, float f);

public:
	GLSSAOBufferBuilder(int w, int h);
	~GLSSAOBufferBuilder();

	unsigned int GetGBufferFBO();
	unsigned int GetGPositionTexID();
	unsigned int GetGNormalTexID();
	unsigned int GetGAlbedoTexID();
	unsigned int GetNoiseTexID();

	unsigned int GetSSAOFBO();
	unsigned int GetSSAOColorAttachmentID();

	unsigned int GetSSAOBlurFBO();
	unsigned int GetSSAOBlurColorAttachmentID();

	std::vector<glm::vec3> GetSamples();

	unsigned int GetW();
	unsigned int GetH();
};

#endif
