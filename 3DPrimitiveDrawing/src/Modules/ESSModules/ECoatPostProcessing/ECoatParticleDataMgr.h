#ifndef ECoatParticleDataMgr_H
#define ECoatParticleDataMgr_H

#include "ECoatAssetsBuilder.h"
#include "ECoatResultReader.h"
#include "ParticleRenderer.h"
#include "ColorBar.h"
#include "BufferInfo.h"
#include "BaseModelIO.h"

class ECoatParticleDataMgr
{
private:
	int _sw;
	int _sh;
	bool _visible;
	int _drawType;
	bool _stlExist;
	bool _triangleIDsExist;

	ECoatAssetsBuilder* _assetsBuilder;
	ECoatResultReader* _resultFileReader;
	ColorBar* _colorBar;

	ParticleRenderer* _particleRenderer;
	char* GetParticleColorBuf(int frameNum, unsigned int* bufSize);
	BufferInfo GenerateNormals(BaseModelIO* stlReader);

public:
	enum
	{
		DRAW_AS_CUBES,
		DRAW_AS_CUBES_WITH_LIGHTING,
		DRAW_AS_STL
	};

	ECoatParticleDataMgr(int sw, int sh, ECoatAssetsBuilder* resultFileReader, ColorBar* colorBar);
	~ECoatParticleDataMgr();

	void SetVisible(bool visible);
	void SetDrawAs(int drawType);
	glm::vec3 GetBBoxCenterAfterTransform();
	void OnSizeChange(int sw, int sh);
	bool GetParticleColor(float mx, float my, unsigned char* color);
	void ApplyContour(int frameNum);
	void Draw(bool drawAllParticles);
};

#endif
