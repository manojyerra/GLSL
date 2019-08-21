#ifndef ECoatParticleDataMgr_H
#define ECoatParticleDataMgr_H

#include "ECoatAssetsBuilder.h"
#include "ECoatResultReader.h"
#include "ParticleRenderer.h"
#include "ColorBar.h"
#include "BufferInfo.h"

class ECoatParticleDataMgr
{
private:
	ECoatAssetsBuilder* _assetsBuilder;
	ECoatResultReader* _resultFileReader;
	ColorBar* _colorBar;

	ParticleRenderer* _particleRenderer;
	char* GetParticleColorBuf(int frameNum, unsigned int* bufSize);
	BufferInfo GenerateNormals(STLReader* stlReader);

public:
	ECoatParticleDataMgr(ECoatAssetsBuilder* resultFileReader, ColorBar* colorBar);
	~ECoatParticleDataMgr();

	void ApplyContour(int frameNum);
	void Draw(bool drawAllParticles);
};

#endif
