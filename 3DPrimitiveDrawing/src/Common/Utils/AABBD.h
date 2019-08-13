#ifndef AABBD_H
#define AABBD_H

#include "glm/glm.hpp"

class AABBD
{
private:

public:
	double minX;
	double maxX;
	double minY;
	double maxY;
	double minZ;
	double maxZ;

	AABBD();
	AABBD(double minX, double maxX, double minY, double maxY, double minZ, double maxZ);
	~AABBD();

	void Set(double minX, double maxX, double minY, double maxY, double minZ, double maxZ);

	double CenterX();
	double CenterY();
	double CenterZ();

	double W();
	double H();
	double D();

	glm::dvec3 Center();
	glm::dvec3 Min();
	glm::dvec3 Max();

	void print();
};

#endif
