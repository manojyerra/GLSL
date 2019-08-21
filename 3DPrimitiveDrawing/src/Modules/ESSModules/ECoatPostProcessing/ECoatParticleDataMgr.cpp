#include "ECoatParticleDataMgr.h"
#include "Platform.h"

ECoatParticleDataMgr::ECoatParticleDataMgr(ECoatAssetsBuilder* assetsBuilder, ColorBar* colorBar)
{
	_assetsBuilder = assetsBuilder;
	_colorBar = colorBar;
	_resultFileReader = _assetsBuilder->GetResultReader();

	BufferInfo parVerBufInfo = _resultFileReader->GetParticleBufferWorkpiece(1);

	_particleRenderer = new ParticleRenderer(parVerBufInfo.buffer, parVerBufInfo.size);
	ApplyContour(1);
}

BufferInfo ECoatParticleDataMgr::GenerateNormals(STLReader* stlReader)
{
	BufferInfo triIDBufInfo = _resultFileReader->GetTriangleIDBufferWorkpiece(1);
	float* triIDArr = (float*)triIDBufInfo.buffer;
	unsigned int numTriIDS = triIDBufInfo.size / 4;

	float* stlNormalsArr = (float*)stlReader->GetNormalBuffer();
	unsigned int stlNormalArrSize = stlReader->GetNormalBufferSize() / 4;

	float* parNormalArr = (float*)malloc(stlReader->GetVertexBufferSize());
	memset(parNormalArr, '\0', stlReader->GetVertexBufferSize());

	for (int i = 0; i < numTriIDS; i++)
	{
		unsigned int triID = triIDArr[i];

		int stlNorIndex = triID * 9;
		int parNorIndex = i * 3;

		//parNormalArr[parNorIndex + 0] = stlNormalsArr[stlNorIndex + 0];
		//parNormalArr[parNorIndex + 1] = stlNormalsArr[stlNorIndex + 1];
		//parNormalArr[parNorIndex + 2] = stlNormalsArr[stlNorIndex + 2];

		memcpy(&parNormalArr[parNorIndex], &stlNormalsArr[stlNorIndex], 12);
	}

	return BufferInfo((char*)parNormalArr, stlReader->GetVertexBufferSize());
}

void ECoatParticleDataMgr::ApplyContour(int frameNum)
{
	long startTime = Platform::GetTimeInMillis();
	unsigned int colorBufSize = 0;
	unsigned char* colorBuf = (unsigned char*)GetParticleColorBuf(frameNum, &colorBufSize);
	
	_particleRenderer->UpdateColorBuffer((char*)colorBuf, colorBufSize);
	free(colorBuf);
	Platform::debugPrint("\nTime for applying contour : %ld milliseconds", Platform::GetTimeInMillis() - startTime);
}

char* ECoatParticleDataMgr::GetParticleColorBuf(int frameNum, unsigned int* bufSize)
{
	FrameInfo frameInfo = _resultFileReader->GetThicknessBuffer(frameNum);

	float minThick = frameInfo.minThickness;
	float maxThick = frameInfo.maxThickness;
	float totDiffThick = maxThick - minThick;
	unsigned int numThicknessVals = frameInfo.bufferSize / sizeof(float);
	float* thicknessBuf = (float*)frameInfo.buffer;

	unsigned int colorBufSize = numThicknessVals * 3;
	char* colorBuf = (char*)malloc(colorBufSize);

	float* allColorsVecR = _colorBar->allColorsVecR;
	float* allColorsVecG = _colorBar->allColorsVecG;
	float* allColorsVecB = _colorBar->allColorsVecB;

	float factor = (float)_colorBar->GetTotNumColors() / totDiffThick;

	for (int i = 0; i < numThicknessVals; i++)
	{
		int index = (int)((thicknessBuf[i] - minThick) * factor);

		int ii = i * 3;
		colorBuf[ii + 0] = allColorsVecR[index] * 255;
		colorBuf[ii + 1] = allColorsVecG[index] * 255;
		colorBuf[ii + 2] = allColorsVecB[index] * 255;
	}

	bufSize[0] = colorBufSize;
	free(thicknessBuf);

	return colorBuf;
}

void ECoatParticleDataMgr::Draw(bool drawAllParticles)
{
	if(drawAllParticles)
		_particleRenderer->DrawAllParticles();
	else
		_particleRenderer->DrawFewParticles();
}

ECoatParticleDataMgr::~ECoatParticleDataMgr()
{
	if (_particleRenderer)
	{
		delete _particleRenderer;
		_particleRenderer = nullptr;
	}
}



/*
	STLReaderWithThreads reader("AdvancedRendererInputFiles/checkedAuto_solidMesh_exported.stl");
	//STLReaderWithThreads reader("data/OpelGlandX.stl");

	//float* stlVertexArr = (float*)reader.GetVertexBuffer();
	//unsigned int stlVertexArrSize = reader.GetVertexBufferSize() / 4;
	//unsigned int stlVertexCount = reader.GetVertexBufferSize() / 12;

	//unsigned int stlColorBufLen = stlVertexCount * 3;
	//unsigned char* stlColorBuf = (unsigned char*)malloc(stlColorBufLen);
	//memset(stlColorBuf, '\0', stlColorBufLen);

	unsigned int parVertexBufSize;
	char* parVertexBuf = _resultReader->GetParticleBufferWorkpiece(1, &parVertexBufSize);

	unsigned int parColorBufLen = 0;
	unsigned char* parColorBuf = (unsigned char*)GetParticleColorBuf(80, &parColorBufLen);

	//Begin : Generate particle normals data

	unsigned int parTriIDBufSize;
	char* parTriIDBuf = _resultReader->GetTriangleIDBufferWorkpiece(1, &parTriIDBufSize);

	float* stlNormalsArr = (float*)reader.GetNormalBuffer();
	unsigned int stlNormalArrSize = reader.GetNormalBufferSize() / 4;

	unsigned int* triIDArr = (unsigned int*)parTriIDBuf;
	unsigned int numTriIDS = parTriIDBufSize / 4;

	long startTime = Platform::GetTimeInMillis();

	float* parNormalArr = (float*)malloc(parVertexBufSize);
	memset(parNormalArr, '\0', parVertexBufSize);

	for (int i = 0; i < numTriIDS; i++)
	{
		unsigned int triID = triIDArr[i];

		int stlNorIndex = triID * 9;
		int parNorIndex = i * 3;

		//parNormalArr[parNorIndex + 0] = stlNormalsArr[stlNorIndex + 0];
		//parNormalArr[parNorIndex + 1] = stlNormalsArr[stlNorIndex + 1];
		//parNormalArr[parNorIndex + 2] = stlNormalsArr[stlNorIndex + 2];

		memcpy(&parNormalArr[parNorIndex], &stlNormalsArr[stlNorIndex], 12);
	}

	printf("\nTimeTaken for copying normals: %ld", (Platform::GetTimeInMillis()-startTime));

	//End : Generate particle normals data


	////ContourMap colorMap(stlVertexArr, stlVertexArrSize, stlColorBuf, stlColorBufLen, (float*)parVertexBuf, parVertexBufSize / 4, parColorBuf, parColorBufLen);

	//BaseModelIO modelIO;
	//modelIO.SetVertexBuffer(reader.GetVertexBuffer(), reader.GetVertexBufferSize());
	//modelIO.SetNormalBuffer(reader.GetNormalBuffer(), reader.GetNormalBufferSize());
	//modelIO.SetColorBuffer((const char*)stlColorBuf, stlColorBufLen);

	//_carBody = new GLMeshRenderer(&modelIO, PBR_SHADER);

	_particleRenderer = new ParticleRenderer(parVertexBuf, parVertexBufSize, (char*)parNormalArr, parVertexBufSize);
	_particleRenderer->UpdateColorBuffer((char*)parColorBuf, parColorBufLen);

*/