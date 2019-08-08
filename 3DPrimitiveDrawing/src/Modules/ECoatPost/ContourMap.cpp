#include "ContourMap.h"
#include "FloatArray.h"
#include "UIntArray.h"
#include "BufferTransformUtils.h"
#include "Platform.h"

ContourMap::ContourMap(float* stlVertexArr, unsigned int stlVertexArrSize,
	unsigned char* stlColorBuf, unsigned int stlColorBufLen,
	float* parVertexArr, unsigned int parVertexArrSize,
	unsigned char* parColorBuf, unsigned int parColorBufLen)

{
	//FloatArray* particleBuf = new FloatArray();

	//particleBuf->push_back_3(0.00, 0.00, 1.0);
	//particleBuf->push_back_3(1.60, 2.40, 0.0);
	//particleBuf->push_back_3(2.20, 1.70, 0.0);
	//particleBuf->push_back_3(4.10, 1.60, 0.0);
	//particleBuf->push_back_3(4.30, 1.90, 0.0);
	//particleBuf->push_back_3(4.10, 2.80, 0.0);
	//particleBuf->push_back_3(5.20, 1.20, 0.0);
	//particleBuf->push_back_3(7.20, 0.80, 0.0);
	//particleBuf->push_back_3(7.80, 1.60, 0.0);
	//particleBuf->push_back_3(7.80, 1.90, 0.0);
	//particleBuf->push_back_3(9.00, 4.00, -1.0);

	//parVertexArr = (float*)particleBuf->getArray();
	//parVertexArrSize = particleBuf->size();


	//Pushing particles indices to cells (3 dimention array)

	AABB particleBBox = BufferTransformUtils::CalcAABB(parVertexArr, parVertexArrSize);
	AABB stlBBox = BufferTransformUtils::CalcAABB(stlVertexArr, stlVertexArrSize);

	glm::vec3 moveParticle(particleBBox.Center() - particleBBox.Min());
	glm::vec3 moveSTL(stlBBox.Center() - stlBBox.Min());

	BufferTransformUtils::Add(parVertexArr, parVertexArrSize, moveParticle);
	BufferTransformUtils::Add(stlVertexArr, stlVertexArrSize, moveSTL);

	float w = particleBBox.W();
	float h = particleBBox.H();
	float d = particleBBox.D();

	float sw = stlBBox.W();
	float sh = stlBBox.H();
	float sd = stlBBox.D();


	double vol = w * h * d;

	double totBoxes = 100*100*100;
	float boxVol = (float)(vol / totBoxes);
	float boxLen = pow(boxVol, 1.0f/3.0f);

	unsigned int numCols = (w / boxLen) + 2;
	unsigned int numRows = (h / boxLen) + 2;
	unsigned int numDeps = (d / boxLen) + 2;

	UIntArray** cells = new UIntArray*[numCols* numRows * numDeps];
	memset(cells, '\0', numCols * numRows * numDeps * sizeof(void*));

	int numParticles = parVertexArrSize / 3;

	for (int i = 0; i < numParticles; i++)
	{
		unsigned int col = parVertexArr[i * 3 + 0] / boxLen;
		unsigned int row = parVertexArr[i * 3 + 1] / boxLen;
		unsigned int dep = parVertexArr[i * 3 + 2] / boxLen;

		int index = dep*(numRows*numCols) + row * numCols + col;
		UIntArray* cell = cells[index];

		if (cell == nullptr)
		{
			UIntArray* newCell = new UIntArray(2);
			newCell->push_back(i);
			cells[index] = newCell;
		}
		else
		{
			cell->push_back(i);
		}
	}

	/*
	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			int index = row * numCols + col;
			UIntArray* cell = cells[index];

			if (cell)
			{
				printf("\n[%d,%d] ", col, row);
				cell->print();
			}
			else
			{
				printf("\nNULL");
			}
		}
	}
	*/


	int stlNumVertex = stlVertexArrSize / 3;

	for (int i = 0; i < stlNumVertex; i++)
	{
		float x = stlVertexArr[i * 3 + 0];
		float y = stlVertexArr[i * 3 + 1];
		float z = stlVertexArr[i * 3 + 2];

		unsigned int col = x / boxLen;
		unsigned int row = y / boxLen;
		unsigned int dep = z / boxLen;

		int index = dep * (numRows * numCols) + row * numCols + col;
		UIntArray* cell = cells[index];

		//TODO: Ideally it should not be null as every vertex should have one particle.
		if (cell != nullptr) 
		{
			unsigned int numEleInCell = cell->size();

			if (numEleInCell == 1)
			{
				unsigned int particleIndex = cell->_arr[0];

				stlColorBuf[i * 3 + 0] = parColorBuf[particleIndex * 3 + 0];
				stlColorBuf[i * 3 + 1] = parColorBuf[particleIndex * 3 + 1];
				stlColorBuf[i * 3 + 2] = parColorBuf[particleIndex * 3 + 2];
			}
			else
			{
				unsigned int particleIndex = cell->_arr[0];

				float nearPX = parVertexArr[particleIndex * 3 + 0];
				float nearPY = parVertexArr[particleIndex * 3 + 1];
				float nearPZ = parVertexArr[particleIndex * 3 + 2];

				float minDist = (nearPX - x) * (nearPX - x) + (nearPY - y) * (nearPY - y) + (nearPZ - z) * (nearPZ - z);

				unsigned int nearParticleIndex = particleIndex;

				for (int arrI = 1; arrI < numEleInCell; arrI++)
				{
					particleIndex = cell->_arr[arrI];

					float px = parVertexArr[particleIndex * 3 + 0];
					float py = parVertexArr[particleIndex * 3 + 1];
					float pz = parVertexArr[particleIndex * 3 + 2];

					float dist = (px - x) * (px - x) + (py - y) * (py - y) + (pz - z) * (pz - z);

					if (dist < minDist)
					{
						minDist = dist;
						nearParticleIndex = particleIndex;
					}
				}

				stlColorBuf[i * 3 + 0] = parColorBuf[nearParticleIndex * 3 + 0];
				stlColorBuf[i * 3 + 1] = parColorBuf[nearParticleIndex * 3 + 1];
				stlColorBuf[i * 3 + 2] = parColorBuf[nearParticleIndex * 3 + 2];

				printf("\nNear ParticlePos : %f, %f, %f", nearPX, nearPY, nearPZ);
			}
		}
	}
}

ContourMap::~ContourMap()
{

}

/*

FloatArray* vertexBuf = new FloatArray();

vertexBuf->push_back_3(0.25, 0.25, 0.0);
vertexBuf->push_back_3(1.70, 2.25, 0.0);
vertexBuf->push_back_3(3.20, 0.70, 0.0);
vertexBuf->push_back_3(4.49, 1.99, 0.0);
vertexBuf->push_back_3(7.80, 0.20, 0.0);
vertexBuf->push_back_3(7.80, 1.70, 0.0);
vertexBuf->push_back_3(8.80, 0.20, 0.0);
vertexBuf->push_back_3(8.80, 3.70, 0.0);
vertexBuf->push_back_3(2.80, 3.20, 0.0);
vertexBuf->push_back_3(6.80, 2.20, 0.0);

FloatArray* particleBuf = new FloatArray();

particleBuf->push_back_3(0.00, 0.00, 0.0);
particleBuf->push_back_3(1.60, 2.40, 0.0);
particleBuf->push_back_3(2.20, 1.70, 0.0);
particleBuf->push_back_3(4.10, 1.60, 0.0);
particleBuf->push_back_3(4.30, 1.90, 0.0);
particleBuf->push_back_3(4.10, 2.80, 0.0);
particleBuf->push_back_3(5.20, 1.20, 0.0);
particleBuf->push_back_3(7.20, 0.80, 0.0);
particleBuf->push_back_3(7.80, 1.60, 0.0);
particleBuf->push_back_3(7.80, 1.90, 0.0);
particleBuf->push_back_3(9.00, 4.00, 0.0);

*/