#ifndef STLReader_H
#define STLReader_H

#include "FloatArray.h"
#include "BaseModelIO.h"

class STLReader : public BaseModelIO
{
private:
	std::string _filePath;

	FloatArray* _vertexFloatArr;
	FloatArray* _normalFloatArr;

	void ReadSTLFile(std::string filePath);

public:
	STLReader(std::string filePath);

	const char* GetVertexBuffer() override;
	const char* GetNormalBuffer() override;

	unsigned int GetVertexBufferSize() override;
	unsigned int GetNormalBufferSize() override;

	void WriteBinaryToFile();
	void WriteBinaryToFile(std::string folderPath);

	~STLReader();
};

#endif