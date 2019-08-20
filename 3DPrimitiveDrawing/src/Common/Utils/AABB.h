#ifndef AABB_H
#define AABB_H

#include "glm/glm.hpp"

class AABB
{
private:

public:
	float minX;
	float maxX;
	float minY;
	float maxY;
	float minZ;
	float maxZ;

	float w;
	float h;
	float d;

	AABB();
	AABB(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
	~AABB();

	void Set(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
	void Set(AABB* b1, AABB* b2);
	void AddMarginByPercent(float percent);
	void AddMargin(float len);

	glm::vec3 Center();

	float CenterX();
	float CenterY();
	float CenterZ();

	glm::vec3 Min();
	glm::vec3 Max();
	glm::vec3 Size();

	void print();
};

#endif
