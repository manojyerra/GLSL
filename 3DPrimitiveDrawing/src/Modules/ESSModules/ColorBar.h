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

	GLMeshRenderer* _meshRenderer;

	void GenerateGeometry(int x, int y, int w, int h);

public:
	float* allColorsVecR;
	float* allColorsVecG;
	float* allColorsVecB;

	ColorBar();
	~ColorBar();

	void SetMinMaxThickness(float minThickness, float maxThickness);
	void GetColor(float thickness, float* r, float* g, float* b);

	void Draw();
};

#endif