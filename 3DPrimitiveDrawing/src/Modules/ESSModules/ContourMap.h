#ifndef ContourMap_H
#define ContourMap_H

#include "glm/glm.hpp"
#include "UIntArray.h"

class ContourMap
{
private:
	float* parVertexArr;

	void FindNearestParticle(UIntArray* cell, glm::vec3& vertex, float* _minDist, int* _nearParIndex);

public:
	ContourMap(float* stlVertexArr, unsigned int stlVertexCount, 
				unsigned char* stlColorBuf, unsigned int stlColorBufLen, 
				float* parVertexArr, unsigned int parVertexArrSize, 
				unsigned char* parColorBuf, unsigned int parColorBufLen);

	~ContourMap();
};

#endif
