#ifndef GLSSAO_H
#define GLSSAO_H

#include <vector>
#include <glm/vec3.hpp>

class GLSSAO
{
private:
	unsigned int _w;
	unsigned int _h;

	unsigned int _gBufferFBO;
	unsigned int _gPosition;
	unsigned int _gNormal;
	unsigned int _gAlbedo;

	unsigned int _ssaoFBO;
	unsigned int _ssaoColorAttachmentID;

	unsigned int _ssaoBlurFBO;
	unsigned int _noiseTexID;

	std::vector<glm::vec3> _ssaoSamples;

	void CreateGBufferFBO(unsigned int w, unsigned int h);
	void CreateSSAOFBO(unsigned int w, unsigned int h);
	void CreateSSAOBlurFBO(unsigned int w, unsigned int h);
	void GenerateSampleKernelAndNoiseTexture();

	float lerp(float a, float b, float f);

public:
	GLSSAO(int w, int h);
	~GLSSAO();

	void BindGBuffer();
	void UnBindGBuffer();

	void BindSSAOBuffer();
	void UnBindSSAOBuffer();

	unsigned int GetSSAOColorAttachmentID();

	unsigned int GetGPositionTexID();
	unsigned int GetGNormalTexID();
	unsigned int GetGAlbedoTexID();
	unsigned int GetNoiseTexID();

	std::vector<glm::vec3> GetSamples();

	unsigned int GetW();
	unsigned int GetH();
};

#endif
