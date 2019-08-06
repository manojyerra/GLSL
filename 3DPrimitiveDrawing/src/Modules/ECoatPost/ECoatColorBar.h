#ifndef ECoatColorBar_H
#define ECoatColorBar_H

#include <vector>
#include <glm/glm.hpp>
#include "GLMeshRenderer.h"

class ECoatColorBar
{
private:
	std::vector<glm::vec3> _allColorsVec;
	unsigned int _totArrElements;

	unsigned int _totColors;
	std::vector<glm::vec3> _mainColorsVec;
	std::vector<glm::vec3> _mainColorsReverseVec;

	GLMeshRenderer* _meshRenderer;

	void GenerateGeometry(int x, int y, int w, int h);

public:
	ECoatColorBar();
	~ECoatColorBar();

	void Draw();
};

#endif