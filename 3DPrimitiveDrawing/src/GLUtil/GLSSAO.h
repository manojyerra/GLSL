#ifndef GLSSAO_H
#define GLSSAO_H

class GLSSAO
{
private:
	unsigned int _w;
	unsigned int _h;

	unsigned int _gBufferFBO;
	

	unsigned int _ssaoFBO;
	unsigned int _ssaoBlurFBO;

	void CreateGBufferFBO(unsigned int w, unsigned int h);
	void CreateSSAOFBO(unsigned int w, unsigned int h);
	void CreateSSAOBlurFBO(unsigned int w, unsigned int h);
	void GenerateSampleKernelAndNoiseTexture();

	float lerp(float a, float b, float f);

public:
	unsigned int _gAlbedo;
	unsigned int _gPosition;
	unsigned int _gNormal;

	GLSSAO(int w, int h);
	~GLSSAO();

	void BindGBuffer();
	void UnBindGBuffer();

	unsigned int GetW();
	unsigned int GetH();
};

#endif
