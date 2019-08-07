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

	glm::vec3 Center();

	void print();
};

#endif
