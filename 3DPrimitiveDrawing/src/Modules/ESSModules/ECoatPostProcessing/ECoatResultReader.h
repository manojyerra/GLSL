#ifndef ECoatResultReader_H
#define ECoatResultReader_H
#include <H5Cpp.h>
#include "glm/vec3.hpp"
#include "BufferInfo.h"

class FrameInfo {
public:
	char* buffer;
	unsigned int bufferSize;
	glm::vec3 rot;
	glm::vec3 trans;
	glm::vec3 cellSize;
	float minThickness;
	float maxThickness;
};

class ECoatResultReader
{
private:
	H5::H5File* _h5File;
	float ReadFloatAttributes(H5::Group* group, const std::string& attr_name);

public:
	ECoatResultReader(std::string resultFilePath);
	~ECoatResultReader();

	FrameInfo GetThicknessBuffer(unsigned int frameNum);
	BufferInfo GetParticleBufferWorkpiece(unsigned int frameNum);
	BufferInfo GetParticleBufferWorkpiece(unsigned int frameNum, H5::PredType dataType);
	BufferInfo GetTriangleIDBufferWorkpiece(unsigned int frameNum);

	unsigned int GetTotalFrameCount();
};

#endif


//FrameInfo GetParticleBuffer(unsigned int frameNum);

