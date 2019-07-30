#ifndef STLReader_H
#define STLReader_H

#include "FloatArray.h"
#include "ModelInfo.h"

class STLReader : public ModelInfo
{
private:
	std::string _folderPath;

	FloatArray* _vertexFloatArr;
	FloatArray* _normalFloatArr;

	void ReadSTLFile(std::string folderPath);

public:
	STLReader(std::string folderPath);

	const char* GetVertexBuffer() override;
	const char* GetNormalBuffer() override;

	unsigned int GetVertexBufferSize() override;
	unsigned int GetNormalBufferSize() override;

	void WriteBinaryToFile();
	void WriteBinaryToFile(std::string folderPath);

	~STLReader();
};

#endif