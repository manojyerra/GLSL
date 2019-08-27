#include "AABB.h"
#include <cstdio>
#include "Platform.h"

AABB::AABB()
{
	minX = maxX = minY = maxY = minZ = maxZ = 0.0f;
	w = h = d = 0.0f;
}

AABB::AABB(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
{
	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
	this->minZ = minZ;
	this->maxZ = maxZ;

	w = maxX - minX;
	h = maxY - minY;
	d = maxZ - minZ;
}

void AABB::Set(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
{
	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
	this->minZ = minZ;
	this->maxZ = maxZ;

	w = maxX - minX;
	h = maxY - minY;
	d = maxZ - minZ;
}

void AABB::Set(AABB* b1, AABB* b2)
{
	this->minX = b1->minX < b2->minX ? b1->minX : b2->minX;
	this->minY = b1->minY < b2->minY ? b1->minY : b2->minY;
	this->minZ = b1->minZ < b2->minZ ? b1->minZ : b2->minZ;

	this->maxX = b1->maxX > b2->maxX ? b1->maxX : b2->maxX;
	this->maxY = b1->maxY > b2->maxY ? b1->maxY : b2->maxY;
	this->maxZ = b1->maxZ > b2->maxZ ? b1->maxZ : b2->maxZ;

	w = maxX - minX;
	h = maxY - minY;
	d = maxZ - minZ;
}

void AABB::AddMarginByPercent(float percent)
{
	float dw = w * percent / 100.0f;
	float dh = h * percent / 100.0f;
	float dd = d * percent / 100.0f;

	minX -= dw / 2.0f;
	minY -= dh / 2.0f;
	minZ -= dd / 2.0f;

	maxX += dw / 2.0f;
	maxY += dh / 2.0f;
	maxZ += dd / 2.0f;

	w = maxX - minX;
	h = maxY - minY;
	d = maxZ - minZ;
}

void AABB::AddMargin(float len)
{
	minX -= len / 2.0f;
	minY -= len / 2.0f;
	minZ -= len / 2.0f;

	maxX += len / 2.0f;
	maxY += len / 2.0f;
	maxZ += len / 2.0f;

	w = maxX - minX;
	h = maxY - minY;
	d = maxZ - minZ;
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

void AABB::Translate(glm::vec3 trans)
{
	minX += trans.x;
	minY += trans.y;
	minZ += trans.z;

	maxX += trans.x;
	maxY += trans.y;
	maxZ += trans.z;
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
	return glm::vec3(w, h, d);
}

void AABB::print()
{
	Platform::debugPrint("\n MinX = %f, MaxX = %f, MinY = %f, MaxY = %f, MinZ = %f, MaxZ = %f", minX, maxX, minY, maxY, minZ, maxZ);
}

AABB::~AABB()
{
}
