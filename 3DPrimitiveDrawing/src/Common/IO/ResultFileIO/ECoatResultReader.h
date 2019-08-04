#ifndef ECoatResultReader_H
#define ECoatResultReader_H
#include <H5Cpp.h>

class ECoatResultReader
{
private:
	H5::H5File* _h5File;

public:
	ECoatResultReader(std::string resultFilePath);
	~ECoatResultReader();

	char* GetParticleBuffer(unsigned int frameNum, unsigned int* dataSize);
};

#endif
