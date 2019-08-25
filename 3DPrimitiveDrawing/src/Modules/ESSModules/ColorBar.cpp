#include "ColorBar.h"
#include "GLBatch.h"
#include "GLState.h"

ColorBar::ColorBar(int sw, int sh)
{
	_totColors = 4;
	_totArrElements = 1000;
	
	_mainColorsVec.push_back(glm::vec3(0.0, 0.0, 1.0)); //blue
	_mainColorsVec.push_back(glm::vec3(0.0, 1.0, 1.0)); //cyan
	_mainColorsVec.push_back(glm::vec3(0.0, 1.0, 0.0)); //green
	_mainColorsVec.push_back(glm::vec3(1.0, 1.0, 0.0)); //yellow
	_mainColorsVec.push_back(glm::vec3(1.0, 0.0, 0.0)); //red

	_mainColorsReverseVec.push_back(glm::vec3(1.0, 0.0, 0.0)); //red
	_mainColorsReverseVec.push_back(glm::vec3(1.0, 1.0, 0.0)); //yellow
	_mainColorsReverseVec.push_back(glm::vec3(0.0, 1.0, 0.0)); //green
	_mainColorsReverseVec.push_back(glm::vec3(0.0, 1.0, 1.0)); //cyan
	_mainColorsReverseVec.push_back(glm::vec3(0.0, 0.0, 1.0)); //blue

	_meshRenderer = NULL;

	allColorsVecR = new float[_totArrElements + 2];
	allColorsVecG = new float[_totArrElements + 2];
	allColorsVecB = new float[_totArrElements + 2];

	memset(allColorsVecR, 0, (_totArrElements + 2) * sizeof(float));
	memset(allColorsVecG, 0, (_totArrElements + 2) * sizeof(float));
	memset(allColorsVecB, 0, (_totArrElements + 2) * sizeof(float));

	_w = 50;
	_h = 600;

	GenerateGeometry(_w, _h);
	CalcPosition((float)sw, (float)sh);

	SetMinMaxThickness(0.0f, 1.0f);
}

unsigned int ColorBar::GetTotNumColors()
{
	return _totArrElements;
}

void ColorBar::CalcPosition(float sw, float sh)
{
	_x = sw * 0.85f;
	_y = (sh - _h) / 2.0f;

	_meshRenderer->SetPos(_x, _y, 0);
}

void ColorBar::OnSizeChange(int sw, int sh)
{
	CalcPosition((float)sw, (float)sh);
}

void ColorBar::GenerateGeometry(int w, int h)
{
	int x = 0;
	int y = 0;

	int numBlocks = _mainColorsReverseVec.size() - 1;
	float eachRectH = (float)h / (float)numBlocks;

	GLBatch batch(128, true, false, false);
	batch.glBegin();

	for (int i = 0; i < numBlocks+1; i++)
	{
		float y1 = y + (i * eachRectH);
		glm::vec3 col = _mainColorsReverseVec[i];

		batch.glColor3f(col);
		batch.glVertex3f(x, y1, 0);
		batch.glVertex3f(x+w, y1, 0);
	}

	BaseModelIO baseModeIO;
	baseModeIO.SetVertexBuffer(batch.GetVertexBuffer(), batch.GetVertexBufferSize());
	baseModeIO.SetColorBuffer(batch.GetColorBuffer(), batch.GetColorBufferSize());

	_meshRenderer = new GLMeshRenderer(&baseModeIO, COLOR_SHADER);
	_meshRenderer->SetPrimitiveType(GLMeshRenderer::triangleStrip);
	ColorShader* colorShader = (ColorShader*)_meshRenderer->GetCurrentShader();
	colorShader->Set2DCamera(true);

	int eachBlockArrElements = _totArrElements / numBlocks;

	int count = 0;
	int start = 0;
	glm::vec3 lastColor;

	for(int j=0; j<numBlocks; j++)
	{
		glm::vec3 col1 = _mainColorsVec[j];
		glm::vec3 col2 = _mainColorsVec[j+1];

		float rDiff = col2.r - col1.r;
		float gDiff = col2.g - col1.g;
		float bDiff = col2.b - col1.b;

		for (int i = start; i < start + eachBlockArrElements; i++)
		{
			float colH = i - start;

			float r = col1.r + colH * rDiff / (float)eachBlockArrElements;
			float g = col1.g + colH * gDiff / (float)eachBlockArrElements;
			float b = col1.b + colH * bDiff / (float)eachBlockArrElements;

			lastColor = glm::vec3(r, g, b);

			allColorsVecR[count] = r;
			allColorsVecG[count] = g;
			allColorsVecB[count] = b;

			//_allColorsVec[count] = lastColor;

			//batch.glColor3f(lastColor);
			//batch.glVertex3f(x, y+count, 0);
			//batch.glVertex3f(x+w, y + count, 0);

			count++;
		}

		start = count;
	}

	for (int i = count; i < _totArrElements+2; i++)
	{
		allColorsVecR[i] = lastColor.r;
		allColorsVecG[i] = lastColor.g;
		allColorsVecB[i] = lastColor.b;
	}
}

void ColorBar::SetMinMaxThickness(float minThickness, float maxThickness)
{
	_minThickness = minThickness;
	_maxThickness = maxThickness;

	_totDiffThickness = _maxThickness - _minThickness;
}

void ColorBar::GetColor(float thickness, float* r, float* g, float* b)
{
	float currDiffThick = thickness - _minThickness;
	int index = (int)(currDiffThick * _totArrElements / _totDiffThickness);

	r[0] = allColorsVecR[index];
	g[0] = allColorsVecG[index];
	b[0] = allColorsVecB[index];
}

void ColorBar::Draw()
{
	if (_meshRenderer)
	{
		bool depth = GLState::GLEnable(GL_DEPTH_TEST, false);
		bool cullFace = GLState::GLEnable(GL_CULL_FACE, false);

		_meshRenderer->Draw();

		GLState::GLEnable(GL_DEPTH_TEST, depth);
		GLState::GLEnable(GL_CULL_FACE, cullFace);
	}
}

ColorBar::~ColorBar()
{
	if (_meshRenderer)
	{
		delete _meshRenderer;
		_meshRenderer = nullptr;
	}

	if (allColorsVecR)
	{
		delete[] allColorsVecR;
		allColorsVecR = nullptr;
	}

	if (allColorsVecG)
	{
		delete[] allColorsVecG;
		allColorsVecG = nullptr;
	}

	if (allColorsVecB)
	{
		delete[] allColorsVecB;
		allColorsVecB = nullptr;
	}
}
