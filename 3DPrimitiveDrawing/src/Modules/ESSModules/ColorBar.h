#ifndef ECoatColorBar_H
#define ECoatColorBar_H

#include <vector>
#include <glm/glm.hpp>
#include "GLMeshRenderer.h"

class ColorBar
{
private:
	int _totArrElements;

	int _totColors;
	std::vector<glm::vec3> _mainColorsVec;
	std::vector<glm::vec3> _mainColorsReverseVec;

	double _minVal;
	double _maxVal;
	double _totDiff;

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
	void SetMinMax(double min, double max);
	void GetColor(double val, float* r, float* g, float* b);
	double GetValue(float r, float g, float b);
	double GetValue(unsigned char r, unsigned char g, unsigned char b);
	int GetTotNumColors();

	void Draw();
};

#endif