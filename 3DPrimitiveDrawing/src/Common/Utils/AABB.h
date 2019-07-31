#ifndef AABB_H
#define AABB_H

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

	void print();
};

#endif
