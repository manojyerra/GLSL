#include "AABB.h"
#include <cstdio>

AABB::AABB()
{
	minX = maxX = minY = maxY = minZ = maxZ = 0.0f;
}

AABB::AABB(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
{
	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
	this->minZ = minZ;
	this->maxZ = maxZ;
}

void AABB::Set(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
{
	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
	this->minZ = minZ;
	this->maxZ = maxZ;
}

float AABB::CenterX()
{
	return (minX + maxX)/ 2.0f;
}

float AABB::CenterY()
{
	return (minY + maxY) / 2.0f;
}

float AABB::CenterZ()
{
	return (minZ + maxZ) / 2.0f;
}

void AABB::print()
{
	printf("\n MinX = %f, MaxX = %f, MinY = %f, MaxY = %f, MinZ = %f, MaxZ = %f", minX, maxX, minY, maxY, minZ, maxZ);
}

AABB::~AABB()
{
	
}