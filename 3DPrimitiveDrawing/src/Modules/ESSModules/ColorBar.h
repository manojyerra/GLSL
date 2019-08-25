#ifndef ECoatColorBar_H
#define ECoatColorBar_H

#include <vector>
#include <glm/glm.hpp>
#include "GLMeshRenderer.h"

class ColorBar
{
private:
	unsigned int _totArrElements;

	unsigned int _totColors;
	std::vector<glm::vec3> _mainColorsVec;
	std::vector<glm::vec3> _mainColorsReverseVec;

	float _minThickness;
	float _maxThickness;
	float _totDiffThickness;

	float _x;
	float _y;
	float _w;
	float _h;

	GLMeshRenderer* _meshRenderer;

	void GenerateGeometry(int w, int h);
	void CalcPosition(float sw, float sh);

public:
	float* allColorsVecR;
	float* allColorsVecG;
	float* allColorsVecB;

	ColorBar(int sw, int sh);
	~ColorBar();

	void OnSizeChange(int sw, int sh);
	void SetMinMaxThickness(float minThickness, float maxThickness);
	void GetColor(float thickness, float* r, float* g, float* b);
	unsigned int GetTotNumColors();
	void Draw();
};

#endif