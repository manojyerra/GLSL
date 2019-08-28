#include "ECoatParticleDataMgr.h"
#include "Platform.h"
#include "GLFBOManager.h"

ECoatParticleDataMgr::ECoatParticleDataMgr(int sw, int sh, ECoatAssetsBuilder* assetsBuilder, ColorBar* colorBar)
{
	_sw = sw;
	_sh = sh;
	_visible = true;

	_assetsBuilder = assetsBuilder;
	_colorBar = colorBar;
	_particleRenderer = nullptr;
	_resultFileReader = _assetsBuilder->GetResultReader();

	BufferInfo parVerBufInfo = _resultFileReader->GetParticleBufferWorkpiece(1);
	BufferInfo parNorBufInfo = GenerateNormals(_assetsBuilder->GetSolidSTLReader());

	if(parNorBufInfo.HasData())
		_particleRenderer = new ParticleRenderer(&parVerBufInfo, &parNorBufInfo);
	else
		_particleRenderer = new ParticleRenderer(&parVerBufInfo);

	free(parVerBufInfo.buffer);
	free(parNorBufInfo.buffer);

	ApplyContour(1);
}

BufferInfo ECoatParticleDataMgr::GenerateNormals(BaseModelIO* stlReader)
{
	BufferInfo parNormalBuf;

	if (stlReader)
	{
		BufferInfo triIDBufInfo = _resultFileReader->GetTriangleIDBufferWorkpiece(1);

		if (triIDBufInfo.HasData())
		{
			unsigned int* triIDArr = (unsigned int*)triIDBufInfo.buffer;
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

			parNormalBuf.buffer = (char*)parNormalArr;
			parNormalBuf.size = stlReader->GetVertexBufferSize();

			free(triIDBufInfo.buffer);
		}
	}

	return parNormalBuf;
}

void ECoatParticleDataMgr::SetVisible(bool visible)
{
	_visible = visible;
}

glm::vec3 ECoatParticleDataMgr::GetBBoxCenterAfterTransform()
{
	return _particleRenderer->GetBBoxCenterAfterTransform();
}

void ECoatParticleDataMgr::OnSizeChange(int sw, int sh)
{
	_sw = sw;
	_sh = sh;
}

bool ECoatParticleDataMgr::GetParticleColor(float mx, float my, unsigned char* color)
{
	GLFBOManager::GetInstance()->BindDefaultFBO();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_particleRenderer->DrawForPicking();

	glReadPixels((GLint)mx, _sh - (GLint)my, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color);

	GLFBOManager::GetInstance()->UnBindDefaultFBO();

	bool isWhite = (color[0] == 255 && color[1] == 255 && color[2] == 255);

	return !isWhite;
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

	_colorBar->SetMinMax(minThick, maxThick);

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

	_particleRenderer->SetPosition(frameInfo.trans);
	_particleRenderer->SetRotation(frameInfo.rot);

	return colorBuf;
}

void ECoatParticleDataMgr::Draw(bool drawAllParticles)
{
	if(_visible)
	{
		if(drawAllParticles)
			_particleRenderer->DrawAllParticles();
		else
			_particleRenderer->DrawFewParticles();
	}
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