#include "AABBD.h"
#include <cstdio>
#include "Platform.h"

AABBD::AABBD()
{
	minX = maxX = minY = maxY = minZ = maxZ = 0.0;
}

AABBD::AABBD(double minX, double maxX, double minY, double maxY, double minZ, double maxZ)
{
	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
	this->minZ = minZ;
	this->maxZ = maxZ;
}

void AABBD::Set(double minX, double maxX, double minY, double maxY, double minZ, double maxZ)
{
	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
	this->minZ = minZ;
	this->maxZ = maxZ;
}

double AABBD::CenterX()
{
	return (minX + maxX)/ 2.0;
}

double AABBD::CenterY()
{
	return (minY + maxY) / 2.0;
}

double AABBD::CenterZ()
{
	return (minZ + maxZ) / 2.0;
}

double AABBD::W()
{
	return (maxX - minX);
}

double AABBD::H()
{
	return (maxY - minY);
}

double AABBD::D()
{
	return (maxZ - minZ);
}

glm::dvec3 AABBD::Center()
{
	double cx = (minX + maxX) / 2.0;
	double cy = (minY + maxY) / 2.0;
	double cz = (minZ + maxZ) / 2.0;

	return glm::dvec3(cx, cy, cz);
}

glm::dvec3 AABBD::Min()
{
	return glm::dvec3(minX, minY, minZ);
}

glm::dvec3 AABBD::Max()
{
	return glm::dvec3(maxX, maxY, maxZ);
}

void AABBD::print()
{
	Platform::debugPrint("\n MinX = %lf, MaxX = %lf, MinY = %lf, MaxY = %lf, MinZ = %lf, MaxZ = %lf", minX, maxX, minY, maxY, minZ, maxZ);
}

AABBD::~AABBD()
{
	
}