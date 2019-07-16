#include "BufferTransformUtils.h"
#include "math.h"

glm::vec3 BufferTransformUtils::CalcCenter(float* vertexBuf, int arrSize)
{
	float minX = vertexBuf[0];
	float minY = vertexBuf[1];
	float minZ = vertexBuf[2];

	float maxX = vertexBuf[0];
	float maxY = vertexBuf[1];
	float maxZ = vertexBuf[2];

	for(int i=3; i<arrSize; i+=3)
	{
		if(vertexBuf[i+0] < minX)	minX = vertexBuf[i+0];
		if(vertexBuf[i+1] < minY)	minY = vertexBuf[i+1];
		if(vertexBuf[i+2] < minZ)	minZ = vertexBuf[i+2];

		if(vertexBuf[i+0] > maxX)	maxX = vertexBuf[i+0];
		if(vertexBuf[i+1] > maxY)	maxY = vertexBuf[i+1];
		if(vertexBuf[i+2] > maxZ)	maxZ = vertexBuf[i+2];
	}

	return glm::vec3((minX + maxX)/2.0f, (minY + maxY)/2.0f, (minZ + maxZ)/2.0f);
}

void BufferTransformUtils::CalcMinMaxPoints(std::vector<glm::vec3>* points3D, glm::vec3* min, glm::vec3* max)
{
	float minX = points3D->at(0).x;
	float minY = points3D->at(0).y;
	float minZ = points3D->at(0).z;

	float maxX = points3D->at(0).x;
	float maxY = points3D->at(0).y;
	float maxZ = points3D->at(0).z;

	unsigned int size = points3D->size();

	for (int i = 1; i < size; i++)
	{
		if (points3D->at(i).x < minX) minX = points3D->at(i).x;
		if (points3D->at(i).y < minY) minY = points3D->at(i).y;
		if (points3D->at(i).z < minZ) minZ = points3D->at(i).z;

		if (points3D->at(i).x > maxX) maxX = points3D->at(i).x;
		if (points3D->at(i).y > maxY) maxY = points3D->at(i).y;
		if (points3D->at(i).z > maxZ) maxZ = points3D->at(i).z;
	}

	min->x = minX;
	min->y = minY;
	min->z = minZ;

	max->x = maxX;
	max->y = maxY;
	max->z = maxZ;
}

void BufferTransformUtils::RotateBufXYZ(float* vertexBuf, int arrSize, float xAng, float yAng, float zAng)
{
	float DEG_RAD = 0.0174532925194f;

	float cosOfXAng = cos(xAng * DEG_RAD);
	float cosOfYAng = cos(yAng * DEG_RAD);
	float cosOfZAng = cos(zAng * DEG_RAD);

	float sinOfXAng = sin(xAng * DEG_RAD);
	float sinOfYAng = sin(yAng * DEG_RAD);
	float sinOfZAng = sin(zAng * DEG_RAD);

	for(int i=0; i<arrSize; i+=3)
	{
		float x = vertexBuf[i+0];
		float y = vertexBuf[i+1];
		float z = vertexBuf[i+2];

		float x1 = x;
		float y1 = y*cosOfXAng - z*sinOfXAng;
		float z1 = y*sinOfXAng + z*cosOfXAng;

		float y2 = y1;
		float z2 = z1*cosOfYAng - x1*sinOfYAng;
		float x2 = z1*sinOfYAng + x1*cosOfYAng;

		vertexBuf[i+0] = x2*cosOfZAng - y2*sinOfZAng;
		vertexBuf[i+1] = x2*sinOfZAng + y2*cosOfZAng;
		vertexBuf[i+2] = z2;
	}
}

void BufferTransformUtils::RotateBufZYX(float* vertexBuf, int arrSize, float zAng, float yAng, float xAng)
{
	float DEG_RAD = 0.0174532925194f;

	float cosOfXAng = cos(xAng * DEG_RAD);
	float cosOfYAng = cos(yAng * DEG_RAD);
	float cosOfZAng = cos(zAng * DEG_RAD);

	float sinOfXAng = sin(xAng * DEG_RAD);
	float sinOfYAng = sin(yAng * DEG_RAD);
	float sinOfZAng = sin(zAng * DEG_RAD);

	for(int i=0; i<arrSize; i+=3)
	{
		float x = vertexBuf[i+0];
		float y = vertexBuf[i+1];
		float z = vertexBuf[i+2];

		float z1 = z;
		float x1 = x*cosOfZAng - y*sinOfZAng;
		float y1 = x*sinOfZAng + y*cosOfZAng;

		float y2 = y1;
		float z2 = z1*cosOfYAng - x1*sinOfYAng;
		float x2 = z1*sinOfYAng + x1*cosOfYAng;

		vertexBuf[i+0] = x2;
		vertexBuf[i+1] = y2*cosOfXAng - z2*sinOfXAng;
		vertexBuf[i+2] = y2*sinOfXAng + z2*cosOfXAng;
	}
}

void BufferTransformUtils::RotateBufX(float* vertexBuf, int arrSize, float xAng)
{
	float DEG_RAD = 0.0174532925194f;

	float cosVal = cos(xAng * DEG_RAD);
	float sinVal = sin(xAng * DEG_RAD);

	for(int i=0; i<arrSize; i+=3)
	{
		float y = vertexBuf[i+1];
		float z = vertexBuf[i+2];

		vertexBuf[i+1] = y*cosVal - z*sinVal;
		vertexBuf[i+2] = y*sinVal + z*cosVal;
	}
}

void BufferTransformUtils::RotateBufY(float* vertexBuf, int arrSize, float yAng)
{
	float DEG_RAD = 0.0174532925194f;

	float cosVal = cos(yAng * DEG_RAD);
	float sinVal = sin(yAng * DEG_RAD);

	for(int i=0; i<arrSize; i+=3)
	{
		float x = vertexBuf[i+0];
		float z = vertexBuf[i+2];

		vertexBuf[i+0] = z*sinVal + x*cosVal;
		vertexBuf[i+2] = z*cosVal - x*sinVal;
	}
}

void BufferTransformUtils::RotateBufZ(float* vertexBuf, int arrSize, float zAng)
{
	float DEG_RAD = 0.0174532925194f;

	float cosVal = cos(zAng * DEG_RAD);
	float sinVal = sin(zAng * DEG_RAD);

	for(int i=0; i<arrSize; i+=3)
	{
		float x = vertexBuf[i+0];
		float y = vertexBuf[i+1];

		vertexBuf[i+0] = x*cosVal - y*sinVal;
		vertexBuf[i+1] = x*sinVal + y*cosVal;
	}
}

void BufferTransformUtils::Add(float* vertexBuf, int arrSize, glm::vec3 move)
{
	for(int i=0; i<arrSize; i+=3)
	{
		vertexBuf[i+0] += move.x;
		vertexBuf[i+1] += move.y;
		vertexBuf[i+2] += move.z;
	}
}

void BufferTransformUtils::Subtract(float* vertexBuf, int arrSize, glm::vec3 move)
{
	Add(vertexBuf, arrSize, -move);
}

void BufferTransformUtils::MulBufWithMatrix(float* vertexBuf, int bufSize, float* matrix)
{
	float* a = matrix;

	for(int i=0; i<bufSize; i+=3)
	{
		float x = vertexBuf[i+0];
		float y = vertexBuf[i+1];
		float z = vertexBuf[i+2];
		
		vertexBuf[i+0] = a[0]*x + a[4]*y + a[8]*z + a[12];
		vertexBuf[i+1] = a[1]*x + a[5]*y + a[9]*z + a[13];
		vertexBuf[i+2] = a[2]*x + a[6]*y + a[10]*z + a[14];
	}
}
