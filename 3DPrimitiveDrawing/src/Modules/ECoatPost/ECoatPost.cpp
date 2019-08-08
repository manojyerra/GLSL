#include "ECoatPost.h"
#include "Cam.h"
#include "Cam2D.h"
#include "ParticleRenderer.h"
#include "ECoatResultReader.h"
#include "Input.h"
#include "ContourMap.h"
#include "ObjReader.h"
#include "STLReader.h"

ECoatPost::ECoatPost(unsigned int sw, unsigned int sh, int argc, char** argv)
{
	_sw = sw;
	_sh = sh;

	SetGLStates();

	float zNear = 0.15f;
	float zFar = 1000000.0f;
	float zNearPlaneW = 0.25f;

	Cam::GetInstance()->Init(_sw, _sh, zNear, zFar, zNearPlaneW);
	Cam2D::GetInstance()->Init(_sw, _sh);

	_floor = new Floor();
	_meshManager = new GLMeshManager(_sw, _sh);

	_assetsBuilder = new ECoatAssetsBuilder(&ECoatAssetsReader("AdvancedRenderer_JLR_ECoating.json"), _meshManager);
	_resultReader = _assetsBuilder->GetResultReader();
	_colorBar = new ECoatColorBar();

	_timeLineFrame = new TimeLineFrame(0, 0, 300, 700, _resultReader->GetFrameCount(), this);




	unsigned int dataSize;
	char* vertexBuf = _resultReader->GetParticleBufferWorkpiece(1, &dataSize);
	


	STLReader reader("AdvancedRendererInputFiles/checkedAuto_solidMesh_exported.stl");

	float* stlVertexArr = (float*)reader.GetVertexBuffer();
	unsigned int stlVertexArrSize = reader.GetVertexBufferSize() / 4;
	unsigned int stlVertexCount = reader.GetVertexBufferSize() / 12;

	unsigned int stlColorBufLen = stlVertexCount * 3;
	unsigned char* stlColorBuf = (unsigned char*)malloc(stlColorBufLen);
	memset(stlColorBuf, '\0', stlColorBufLen);

	
	float* parVertexArr = (float*)vertexBuf;
	unsigned int parVertexArrSize = dataSize / 4;

	unsigned int parColorBufLen = 0;
	unsigned char* parColorBuf = (unsigned char*)GetParticleColorBuf(305, &parColorBufLen);

	ContourMap colorMap(stlVertexArr, stlVertexArrSize, stlColorBuf, stlColorBufLen, parVertexArr, parVertexArrSize, parColorBuf, parColorBufLen);

	BaseModelIO modelIO;
	modelIO.SetVertexBuffer(reader.GetVertexBuffer(), reader.GetVertexBufferSize());
	modelIO.SetColorBuffer((const char*)stlColorBuf, stlColorBufLen);

	_carBody = new GLMeshRenderer(&modelIO, COLOR_SHADER);

	_particleRenderer = new ParticleRenderer(vertexBuf, dataSize);

	free(vertexBuf);

	ApplyContour(1);

	SetScreenSize(_sw, _sh);
}

void ECoatPost::SetScreenSize(unsigned int sw, unsigned int sh)
{
	_sw = sw;
	_sh = sh;

	glViewport(0, 0, _sw, _sh);

	Cam::GetInstance()->SetScreenSize(_sw, _sh);
	Cam2D::GetInstance()->SetScreenSize(_sw, _sh);
}

void ECoatPost::SetGLStates()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ECoatPost::OnModuleChange(unsigned int sw, unsigned int sh)
{
}

void ECoatPost::Update(float deltaTime) 
{
}

void ECoatPost::Draw()
{
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Cam* cam = Cam::GetInstance();
	cam->SetPerspectiveProjection();
	cam->SetViewMatrix();
	cam->UpdateCamera();

	_floor->Draw();

	for (int i = 0; i < _meshManager->Size(); i++)
	{
		_meshManager->Get(i)->Draw();
	}

	_carBody->Draw();

	_particleRenderer->DrawAllParticles();
	_colorBar->Draw();
}

void ECoatPost::actionPerformed(SUIActionEvent e)
{
	SUIComponent* com = (SUIComponent*)e.GetComponent();

	if (com == _timeLineFrame->nextFrame)
	{
		auto selectedFrameIndex = _timeLineFrame->selectedFrame->GetSelectedIndex();
		selectedFrameIndex++;

		ApplyContour(selectedFrameIndex + 1);

		_timeLineFrame->selectedFrame->SetSelect(selectedFrameIndex);
	}
	else if (com == _timeLineFrame->selectedFrame)
	{
		auto selectedFrameIndex = _timeLineFrame->selectedFrame->GetSelectedIndex();
		ApplyContour(selectedFrameIndex+1);
	}
}

void ECoatPost::ApplyContour(int frameNum)
{
	long startTime = Platform::GetTimeInMillis();

	if (_particleRenderer)
	{
		FrameInfo frameInfo = _resultReader->GetParticleColorBuffer(frameNum);

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

		//TODO: 1000.0f should be taken from ColorBar class.
		float factor = 1000.0f / totDiffThick;

		for (int i = 0; i < numThicknessVals; i++)
		{
			int ii = i * 3;

			int index = (int)((thicknessBuf[i] - minThick) * factor);

			colorBuf[ii + 0] = allColorsVecR[index] * 255;
			colorBuf[ii + 1] = allColorsVecG[index] * 255;
			colorBuf[ii + 2] = allColorsVecB[index] * 255;
		}

		if (_particleRenderer)
		{
			glm::vec3 carCenter = _assetsBuilder->GetSolid()->GetAABB().Center();
			glm::vec3 particleCenter = _particleRenderer->GetBBoxCenter();

			glm::vec3 delta = carCenter - particleCenter;

			_particleRenderer->UpdateColorBuffer(colorBuf, colorBufSize);
			_particleRenderer->SetPosition(delta.x, delta.y, delta.z);
		}

		free(colorBuf);
		free(thicknessBuf);

		Platform::debugPrint("\nTime : %ld", Platform::GetTimeInMillis() - startTime);
	}
}

char* ECoatPost::GetParticleColorBuf(int frameNum, unsigned int* bufSize)
{
	FrameInfo frameInfo = _resultReader->GetParticleColorBuffer(frameNum);

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

	//TODO: 1000.0f should be taken from ColorBar class.
	float factor = 1000.0f / totDiffThick;

	for (int i = 0; i < numThicknessVals; i++)
	{
		int ii = i * 3;
		int index = (int)((thicknessBuf[i] - minThick) * factor);

		colorBuf[ii + 0] = allColorsVecR[index] * 255;
		colorBuf[ii + 1] = allColorsVecG[index] * 255;
		colorBuf[ii + 2] = allColorsVecB[index] * 255;
	}

	free(thicknessBuf);
	bufSize[0] = colorBufSize;

	return colorBuf;
}

ECoatPost::~ECoatPost()
{
	if (_particleRenderer)
	{
		delete _particleRenderer;
		_particleRenderer = NULL;
	}

	if (_timeLineFrame)
	{
		delete _timeLineFrame;
		_timeLineFrame = nullptr;
	}

	if (_floor)
	{
		delete _floor;
		_floor = nullptr;
	}

	if (_colorBar)
	{
		delete _colorBar;
		_colorBar = nullptr;
	}

	if (_meshManager)
	{
		delete _meshManager;
		_meshManager = nullptr;
	}
}



//
//glm::vec3 sCol(0.0f, 0.0f, 255.0f);
//glm::vec3 eCol(255.0f, 0.0f, 0.0f);
//
//float totDiffR = eCol.r - sCol.r;
//float totDiffG = eCol.g - sCol.g;
//float totDiffB = eCol.b - sCol.b;
//
//float factorR = totDiffR / totDiffThick;
//float factorG = totDiffG / totDiffThick;
//float factorB = totDiffB / totDiffThick;
//
//for (int i = 0; i < numThicknessVals; i++)
//{
//	int ii = i * 3;
//
//	colorBuf[ii + 0] = sCol.r + factorR * thickness;
//	colorBuf[ii + 1] = sCol.g + factorG * thickness;
//	colorBuf[ii + 2] = sCol.b + factorB * thickness;
//
//	float r = 0;
//	float g = 0;
//	float b = 0;
//
//	_colorBar->GetColor(thickness, &r, &g, &b);
//
//	colorBuf[ii + 0] = r * 255;
//	colorBuf[ii + 1] = g * 255;
//	colorBuf[ii + 2] = b * 255;
//}
//