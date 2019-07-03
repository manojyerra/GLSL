#ifndef SSAO_H
#define SSAO_H

#include "Macros.h"

class SSAO
{
private:
	unsigned int _w;
	unsigned int _h;

	unsigned int _gBufferFBO;
	unsigned int _ssaoFBO;
	unsigned int _ssaoBlurFBO;

	unsigned int CreateGBufferFBO(unsigned int w, unsigned int h);

public:
	SSAO(int w, int h);
	~SSAO();

	unsigned int GetW();
	unsigned int GetH();


};

#endif
