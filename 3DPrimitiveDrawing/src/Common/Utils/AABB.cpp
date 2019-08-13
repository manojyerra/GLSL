#include "AABB.h"
#include <cstdio>
#include "Platform.h"

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

float AABB::W()
{
	return (maxX - minX);
}

float AABB::H()
{
	return (maxY - minY);
}

float AABB::D()
{
	return (maxZ - minZ);
}

glm::vec3 AABB::Center()
{
	float cx = (minX + maxX) / 2.0f;
	float cy = (minY + maxY) / 2.0f;
	float cz = (minZ + maxZ) / 2.0f;

	return glm::vec3(cx, cy, cz);
}

glm::vec3 AABB::Min()
{
	return glm::vec3(minX, minY, minZ);
}

glm::vec3 AABB::Max()
{
	return glm::vec3(maxX, maxY, maxZ);
}

glm::vec3 AABB::Size()
{
	return glm::vec3(maxX - minX, maxY - minY, maxZ - minZ);
}

void AABB::print()
{
	Platform::debugPrint("\n MinX = %f, MaxX = %f, MinY = %f, MaxY = %f, MinZ = %f, MaxZ = %f", minX, maxX, minY, maxY, minZ, maxZ);
}

AABB::~AABB()
{
}
