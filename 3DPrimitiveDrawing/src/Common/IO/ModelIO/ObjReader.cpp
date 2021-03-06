#include "ObjReader.h"
#include "FileReader.h"
#include "ScanLineUtils.h"
#include "VoidPtrArray.h"
#include "Platform.h"
#include "StringUtils.h"

ObjReader::ObjReader(std::string filePath)
{
	_filePath = filePath;
	_imageBuffer = NULL;

	_vertexFloatArr = NULL;
	_uvFloatArr = NULL;
	_normalFloatArr = NULL;

	long startTime = Platform::GetTimeInMillis();
	Platform::debugPrint("\nBegin: Loading %s, ", _filePath.c_str());
	ReadObjFile(_filePath);
	
	std::string texturePath = _filePath.substr(0, _filePath.length() - 3).append("png");
	LoadTextures(texturePath);

	Platform::debugPrint("\nEnd: Loading %s, Load Time : %ld milliseconds\n", _filePath.c_str(), Platform::GetTimeInMillis() - startTime);
}

void ObjReader::ReadObjFile(std::string filePath)
{
	FileReader fileReader(filePath, "rb");

	float vx, vy, vz;
	float tx, ty;
	float nx, ny, nz;

	std::vector<glm::vec3> vertexVec;
	std::vector<glm::vec3> uvVec;
	std::vector<glm::vec3> normalVec;

	VoidPtrArray facesArr(1024 * 1024);

	char* line = NULL;
	bool isFirstVertex = true;

	while ((line = fileReader.ReadLine()) != NULL)
	{
		if (line[0] == 'f' && line[1] == ' ')
		{
			facesArr.push_back(line);
		}
		else
		{
			if (line[0] == 'v' && line[1] == ' ')
			{
				ScanLineUtils::scan_vertex(line, &vx, &vy, &vz);
				vertexVec.push_back(glm::vec3(vx, vy, vz));

				if(isFirstVertex)
				{
					bBox.minX = bBox.maxX = vx;
					bBox.minY = bBox.maxY = vy;
					bBox.minZ = bBox.maxZ = vz;

					isFirstVertex = false;
				}
				else
				{
					if(vx < bBox.minX)			bBox.minX = vx;
					else if(vx > bBox.maxX)		bBox.maxX = vx;

					if (vy < bBox.minY)			bBox.minY = vy;
					else if (vy > bBox.maxY)	bBox.maxY = vy;

					if (vz < bBox.minZ)			bBox.minZ = vz;
					else if (vz > bBox.maxZ)	bBox.maxZ = vz;
				}
			}
			else if (line[0] == 'v' && line[1] == 't')
			{
				ScanLineUtils::scan_uv(line, &tx, &ty);
				uvVec.push_back(glm::vec3(tx, 1 - ty, 0));
			}
			else if (line[0] == 'v' && line[1] == 'n')
			{
				ScanLineUtils::scan_normal(line, &nx, &ny, &nz);
				normalVec.push_back(glm::vec3(nx, ny, nz));
			}
			
			//free(line);
		}
	}

	int v[3];
	int t[3];
	int n[3];

	for (int i = 0; i < 3; i++)
	{
		v[i] = 0;
		t[i] = 0;
		n[i] = 0;
	}

	bool uvsExist = uvVec.size() > 1;
	bool normalsExist = normalVec.size() > 1;

	unsigned int initSize = 1024 * 1024;

	_vertexFloatArr = new FloatArray(initSize * 4);

	unsigned int facesStrArrSize = facesArr.size();
	void** facesStrArrPtr = facesArr.getArray();

	if (uvsExist && normalsExist)
	{
		_uvFloatArr = new FloatArray(initSize * 3);
		_normalFloatArr = new FloatArray(initSize * 4);

		for (unsigned int arrIndex = 0; arrIndex < facesStrArrSize; arrIndex++)
		{
			char* line = (char*)facesStrArrPtr[arrIndex];

			ScanLineUtils::scanFace_VTN(line, &v[0], &t[0], &n[0], &v[1], &t[1], &n[1], &v[2], &t[2], &n[2]);

			for (int i = 0; i < 3; i++)
			{
				_vertexFloatArr->push_back_3(vertexVec[v[i] - 1]);
				_uvFloatArr->push_back_2(uvVec[t[i] - 1]);
				_normalFloatArr->push_back_3(normalVec[n[i] - 1]);
			}

			//free(line);
		}
	}
	else if (uvsExist && !normalsExist)
	{
		_uvFloatArr = new FloatArray(initSize * 3);

		for (unsigned int arrIndex = 0; arrIndex < facesStrArrSize; arrIndex++)
		{
			char* line = (char*)facesStrArrPtr[arrIndex];

			ScanLineUtils::scanFace_VT(line, &v[0], &t[0], &v[1], &t[1], &v[2], &t[2]);

			for (int i = 0; i < 3; i++)
			{
				_vertexFloatArr->push_back_3(vertexVec[v[i] - 1]);
				_uvFloatArr->push_back_2(uvVec[t[i] - 1]);
			}

			//free(line);
		}
	}
	else if (!uvsExist && normalsExist)
	{
		_normalFloatArr = new FloatArray(initSize * 4);

		for (unsigned int arrIndex = 0; arrIndex < facesStrArrSize; arrIndex++)
		{
			char* line = (char*)facesStrArrPtr[arrIndex];

			ScanLineUtils::scanFace_VN(line, &v[0], &n[0], &v[1], &n[1], &v[2], &n[2]);

			for (int i = 0; i < 3; i++)
			{
				_vertexFloatArr->push_back_3(vertexVec[v[i] - 1]);
				_normalFloatArr->push_back_3(normalVec[n[i] - 1]);
			}

			//free(line);
		}
	}
	else if (!uvsExist && !normalsExist)
	{
		for (unsigned int arrIndex = 0; arrIndex < facesStrArrSize; arrIndex++)
		{
			char* line = (char*)facesStrArrPtr[arrIndex];

			ScanLineUtils::scanFace_V(line, &v[0], &v[1], &v[2]);

			for (int i = 0; i < 3; i++)
			{
				_vertexFloatArr->push_back_3(vertexVec[v[i] - 1]);
			}

			//free(line);
		}
	}
}

void ObjReader::WriteBinaryToFile()
{
	std::string folderPath = StringUtils::getFolderNameFromPath(_filePath);
	WriteBinaryToFile(folderPath);
}

void ObjReader::WriteBinaryToFile(std::string folderPath)
{
	if (_vertexFloatArr)
	{
		std::string bufFilePath = folderPath + "/vertex.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(_vertexFloatArr->getArray(), sizeof(float), _vertexFloatArr->size(), bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}

	if (_uvFloatArr)
	{
		std::string bufFilePath = folderPath + "/uv.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(_uvFloatArr->getArray(), sizeof(float), _uvFloatArr->size(), bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}

	if (_normalFloatArr)
	{
		std::string bufFilePath = folderPath + "/normal.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(_normalFloatArr->getArray(), sizeof(float), _normalFloatArr->size(), bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}
}

void ObjReader::LoadTextures(std::string folderPath)
{
	std::string filePath = folderPath + "/texture.png";

	if (FileReader::IsFileExists(filePath))
	{
		_imageBuffer  = new ImageBuffer(filePath);
	}
}

const char* ObjReader::GetVertexBuffer()
{
	return (_vertexFloatArr ? (const char*)_vertexFloatArr->getArray() : NULL);
}

const char* ObjReader::GetUVBuffer()
{
	return (_uvFloatArr ? (const char*)_uvFloatArr->getArray() : NULL);
}

const char* ObjReader::GetNormalBuffer()
{
	return (_normalFloatArr ? (const char*)_normalFloatArr->getArray() : NULL);
}

ImageBuffer* ObjReader::GetImageBuffer()
{
	return _imageBuffer;
}

//TODO : Think of having sizeInBytes method in FloatArray class instead of multiplying it with sizeof(float)
unsigned int ObjReader::GetVertexBufferSize()
{
	return _vertexFloatArr ? _vertexFloatArr->size() * sizeof(float) : 0;
}

unsigned int ObjReader::GetUVBufferSize()
{
	return _uvFloatArr ? _uvFloatArr->size() * sizeof(float) : 0;
}

unsigned int ObjReader::GetNormalBufferSize()
{
	return _normalFloatArr ? _normalFloatArr->size() * sizeof(float) : 0;
}

ObjReader::~ObjReader()
{
	if (_imageBuffer)
	{
		delete _imageBuffer;
		_imageBuffer = NULL;
	}

	if (_vertexFloatArr)
	{
		delete _vertexFloatArr;
		_vertexFloatArr = NULL;
	}

	if (_uvFloatArr)
	{
		delete _uvFloatArr;
		_uvFloatArr = NULL;
	}

	if (_normalFloatArr)
	{
		delete _normalFloatArr;
		_normalFloatArr = NULL;
	}
}
