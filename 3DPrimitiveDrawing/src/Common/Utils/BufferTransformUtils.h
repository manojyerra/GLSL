#ifndef BufferTransformUtils_H
#define BufferTransformUtils_H

#include "glm/glm.hpp"
#include "AABB.h"
#include "AABBD.h"
#include <vector>

class BufferTransformUtils
{
public:
	static glm::vec3 CalcCenter(float* vertexBuf, unsigned int arrSize);
	static AABB CalcAABB(float* vertexBuf, unsigned int arrSize);
	static AABBD CalcAABBD(double* vertexBuf, unsigned long arrSize);

	static void CalcMinMaxPoints(std::vector<glm::vec3>* point3D, glm::vec3* min, glm::vec3* max);

	static void RotateBufXYZ(float* vertexBuf, int arrSize, float xAng, float yAng, float zAng);
	static void RotateBufZYX(float* vertexBuf, int arrSize, float zAng, float yAng, float xAng);
	static void RotateBufX(float* vertexBuf, int arrSize, float xAng);
	static void RotateBufY(float* vertexBuf, int arrSize, float yAng);
	static void RotateBufZ(float* vertexBuf, int arrSize, float zAng);
	static void Subtract(float* vertexBuf, int arrSize, glm::vec3 move);
	static void Add(float* vertexBuf, int arrSize, glm::vec3 move);
	static void MulBufWithMatrix(float* vertexBuf, int bufSize, float* matrix);
};

#endif
