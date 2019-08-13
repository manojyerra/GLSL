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

	AABB();
	AABB(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
	~AABB();

	void Set(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);

	float CenterX();
	float CenterY();
	float CenterZ();

	float W();
	float H();
	float D();

	glm::vec3 Center();
	glm::vec3 Min();
	glm::vec3 Max();
	glm::vec3 Size();

	void print();
};

#endif
