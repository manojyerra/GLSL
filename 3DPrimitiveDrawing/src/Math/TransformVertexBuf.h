#ifndef TransformVertexBuf_H
#define TransformVertexBuf_H

#include "glm/glm.hpp"

class TransformVertexBuf
{
public:
	static glm::vec3 CalcCenter(float* vertexBuf, int arrSize);
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
