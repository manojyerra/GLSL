#ifndef ParticleReader_H
#define ParticleReader_H

#include "FileReader.h"
#include "ModelInfo.h"

class ParticleReader : public ModelInfo
{
private:
	FileReader* _vertexFileReader;
	unsigned char* _colorBuf;
	unsigned int _colorBufLen;

	unsigned char* _lowPolyVertexBuf;
	unsigned char* _lowPolyColorBuf;
	unsigned int _lowPolyVertexBufLen;
	unsigned int _lowPolyColorBufLen;

	void GenerateLowPolyData(const char* fileData, unsigned int length);

	int _retriveDataType;

public:
	static const int DATA_AS_IT_IS = 1;
	static const int DATA_AS_LOW_POLY = 2;

	ParticleReader(std::string folderPath);
	~ParticleReader();

	void SetRetriveDataType(int retriveType);

	const char* GetVertexBuffer() override;
	const char* GetColorBuffer() override;

	unsigned int GetVertexBufferSize() override;
	unsigned int GetColorBufferSize() override;
};

#endif