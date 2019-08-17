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
	this->parVertexArr = parVertexArr;

	long startTime = Platform::GetTimeInMillis();

	/*
	AABB parBBox = BufferTransformUtils::CalcAABB(parVertexArr, parVertexArrSize);
	AABB stlBBox = BufferTransformUtils::CalcAABB(stlVertexArr, stlVertexArrSize);

	BufferTransformUtils::Subtract(parVertexArr, parVertexArrSize, parBBox.Min());
	BufferTransformUtils::Subtract(stlVertexArr, stlVertexArrSize, stlBBox.Min());

	parBBox = BufferTransformUtils::CalcAABB(parVertexArr, parVertexArrSize);
	stlBBox = BufferTransformUtils::CalcAABB(stlVertexArr, stlVertexArrSize);

	glm::vec3 parCen = parBBox.Center();
	glm::vec3 stlCen = stlBBox.Center();
	glm::vec3 parCenMove(stlCen - parCen);

	BufferTransformUtils::Add(parVertexArr, parVertexArrSize, parCenMove);

	parBBox = BufferTransformUtils::CalcAABB(parVertexArr, parVertexArrSize);

	printf("\nTime taken for buffer transform: %ld", Platform::GetTimeInMillis() - startTime);
	startTime = Platform::GetTimeInMillis();


	double totBoxes = 300 * 300 * 300;

	double vol = parBBox.w * parBBox.h * parBBox.d;
	float boxVol = (float)(vol / totBoxes);
	float boxLen = pow(boxVol, 1.0f / 3.0f);

	parBBox.AddMargin(boxLen * 5);

	BufferTransformUtils::Subtract(parVertexArr, parVertexArrSize, parBBox.Min());
	BufferTransformUtils::Subtract(stlVertexArr, stlVertexArrSize, parBBox.Min());

	//vol = parBBox.w * parBBox.h * parBBox.d;
	//boxVol = (float)(vol / totBoxes);
	//boxLen = pow(boxVol, 1.0f / 3.0f);

	unsigned int numCols = (parBBox.w / boxLen) + 5;
	unsigned int numRows = (parBBox.h / boxLen) + 5;
	unsigned int numDeps = (parBBox.d / boxLen) + 5;

	*/


	//Finding bounding boxes of stl and particle data.
	AABB parBBox = BufferTransformUtils::CalcAABB(parVertexArr, parVertexArrSize);
	AABB stlBBox = BufferTransformUtils::CalcAABB(stlVertexArr, stlVertexArrSize);

	//Moving particle center to stl center.
	glm::vec3 parCenMove(stlBBox.Center() - parBBox.Center());
	BufferTransformUtils::Add(parVertexArr, parVertexArrSize, parCenMove);

	//Now find the bounding box of stl and particle data again.
	parBBox = BufferTransformUtils::CalcAABB(parVertexArr, parVertexArrSize);
	stlBBox = BufferTransformUtils::CalcAABB(stlVertexArr, stlVertexArrSize);

	AABB cellsBBox;
	cellsBBox.Set(&parBBox, &stlBBox);

	double totBoxes = 300 * 300 * 300;
	double vol = cellsBBox.w * cellsBBox.h * cellsBBox.d;
	float boxVol = (float)(vol / totBoxes);
	float boxLen = pow(boxVol, 1.0f / 3.0f);

	cellsBBox.AddMargin(boxLen*5);

	vol = cellsBBox.w * cellsBBox.h * cellsBBox.d;
	boxVol = (float)(vol / totBoxes);
	boxLen = pow(boxVol, 1.0f / 3.0f);

	BufferTransformUtils::Subtract(parVertexArr, parVertexArrSize, cellsBBox.Min());
	BufferTransformUtils::Subtract(stlVertexArr, stlVertexArrSize, cellsBBox.Min());

	unsigned int numCols = (cellsBBox.w / boxLen) + 5;
	unsigned int numRows = (cellsBBox.h / boxLen) + 5;
	unsigned int numDeps = (cellsBBox.d / boxLen) + 5;


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

	printf("\nTime taken for pushing particles to cells : %ld",Platform::GetTimeInMillis() - startTime);
	startTime = Platform::GetTimeInMillis();

	int blackCount = 0;

	int stlNumVertex = stlVertexArrSize / 3;
	unsigned char lastColor[3];
	lastColor[0] = 0;
	lastColor[1] = 0;
	lastColor[2] = 0;

	int rad = 1;

	for (int i = 0; i < stlNumVertex; i++)
	{
		float x = stlVertexArr[i * 3 + 0];
		float y = stlVertexArr[i * 3 + 1];
		float z = stlVertexArr[i * 3 + 2];

		glm::vec3 vertex(x, y, z);

		int cellCol = x / boxLen;
		int cellRow = y / boxLen;
		int cellDep = z / boxLen;

		int nearParIndex = -1;
		float minDist = -1.0f;

		for (int dep = cellDep - rad; dep <= cellDep + rad; dep++)
		{
			if (dep < 0 || dep >= numDeps)
				continue;

			for (int row = cellRow - rad; row <= cellRow + rad; row++)
			{
				if (row < 0 || row >= numRows)
					continue;

				for (int col = cellCol - rad; col <= cellCol + rad; col++)
				{
					if (col < 0 || col >= numCols)
						continue;

					int cellIndex = dep * (numRows * numCols) + row * numCols + col;
					UIntArray* cell = cells[cellIndex];

					float dist = 0.0f;
					int parIndex = -1;

					FindNearestParticle(cell, vertex, &dist, &parIndex);

					if (nearParIndex == -1 || dist < minDist)
					{
						if (parIndex != -1)
						{
							minDist = dist;
							nearParIndex = parIndex;
						}
					}
				}
			}
		}

		if(nearParIndex != -1)
		{
			stlColorBuf[i * 3 + 0] = lastColor[0] = parColorBuf[nearParIndex * 3 + 0];
			stlColorBuf[i * 3 + 1] = lastColor[1] = parColorBuf[nearParIndex * 3 + 1];
			stlColorBuf[i * 3 + 2] = lastColor[2] = parColorBuf[nearParIndex * 3 + 2];
		}
		else
		{			
			stlColorBuf[i * 3 + 0] = lastColor[0];
			stlColorBuf[i * 3 + 1] = lastColor[1];
			stlColorBuf[i * 3 + 2] = lastColor[2];

			blackCount++;
		}
	}

	printf("\nTime taken for fetching color to vertex: %ld", Platform::GetTimeInMillis() - startTime);
	printf("\nBlack count : %d", blackCount);
}


void ContourMap::FindNearestParticle(UIntArray* cell, glm::vec3& ver, float* _minDist, int* _nearParIndex)
{
	//TODO: Ideally it should not be null as every vertex should have one particle.
	if (cell != nullptr)
	{
		unsigned int cellElementsSize = cell->size();
		unsigned int* cellElements = (unsigned int*)cell->getArray();
		unsigned int nearestParIndex = -1;
		float minDist = 0.0f;

		for (int i = 0; i < cellElementsSize; i++)
		{
			unsigned int parIndex = cellElements[i];

			float parX = parVertexArr[parIndex * 3 + 0];
			float parY = parVertexArr[parIndex * 3 + 1];
			float parZ = parVertexArr[parIndex * 3 + 2];

			float dx = (parX - ver.x);
			float dy = (parY - ver.y);
			float dz = (parZ - ver.z);

			float dist = dx*dx + dy*dy + dz*dz;

			if (dist < minDist || i == 0)
			{
				minDist = dist;
				nearestParIndex = parIndex;
			}
		}

		_minDist[0] = minDist;
		_nearParIndex[0] = nearestParIndex;
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