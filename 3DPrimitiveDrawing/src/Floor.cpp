#include "Floor.h"
#include "GLState.h"
#include "ShaderProgramsManager.h"
#include "Cam.h"
#include "GLBatch.h"

Floor::Floor()
{
	_visible = true;
	_gridVisible = false;
	_axisVisible = true;
	_gridLinesVisible = true;
	_smallGridLinesVisible = false;
	_bigGridLinesVisible = true;

	_axisRenderer = nullptr;
	_gridRectRenderer = nullptr;
	_gridRenderer = nullptr;
	_gridLinesRenderer = nullptr;
	_smallGridLinesRenderer = nullptr;
	_bigGridLinesRenderer = nullptr;

	_gridType = GRID_SINGLE_RECT;

	float start = -20;
	float end = 20;

	GenerateGrid(start, end, 1.0f);
	GenerateGridRect(start, end);
	GenerateSmallGridLines(start, end, 0.1f);
	GenerateGridLines(start, end, 1.0f);
	GenerateBigGridLines(start, end, 5.0f);
	GenerateAxis(start, end);
}

void Floor::GenerateGrid(float start, float end, float gap)
{
	GLBatch* glBatch = new GLBatch(100, true, false, false);
	glBatch->glBegin();

	int c1 = 120;
	int c2 = 130;

	for (int outer = (int)start; outer < end; outer++)
	{
		int temp = c1;
		c1 = c2;
		c2 = temp;
		for (int i = (int)start; i < end; i += (int)gap)
		{
			if (i % 2 == 0)
				glBatch->glColor3ub(c1, c1, c1);
			else
				glBatch->glColor3ub(c2, c2, c2);

			glBatch->glVertex3f((float)i, 0, outer*gap);
			glBatch->glVertex3f((float)i + gap, 0, outer*gap);
			glBatch->glVertex3f((float)i, 0, (outer + 1)*gap);

			glBatch->glVertex3f((float)i + gap, 0, outer*gap);
			glBatch->glVertex3f((float)i, 0, (outer + 1)*gap);
			glBatch->glVertex3f((float)i + gap, 0, (outer + 1)*gap);
		}
	}

	BaseModelIO BaseModelIO;
	BaseModelIO.SetVertexBuffer(glBatch->GetVertexBuffer(), glBatch->GetVertexBufferSize());
	BaseModelIO.SetColorBuffer(glBatch->GetColorBuffer(), glBatch->GetColorBufferSize());
	_gridRenderer = new GLMeshRenderer(&BaseModelIO, COLOR_SHADER);

	delete glBatch;
}

void Floor::GenerateGridRect(float start, float end)
{
	auto glBatch = new GLBatch(100, false, false, false);

	glBatch->glBegin();
	glBatch->glVertex3f(start, 0, -end);
	glBatch->glVertex3f(-start, 0, -end);
	glBatch->glVertex3f(start, 0, end);
	glBatch->glVertex3f(-start, 0, end);

	BaseModelIO BaseModelIO;
	BaseModelIO.SetVertexBuffer(glBatch->GetVertexBuffer(), glBatch->GetVertexBufferSize());

	_gridRectRenderer = new GLMeshRenderer(&BaseModelIO, BASIC_SHADER);
	_gridRectRenderer->SetPrimitiveType(GLMeshRenderer::triangleStrip);
	BasicShader* shader = (BasicShader*)_gridRectRenderer->GetCurrentShader();
	shader->SetColor(glm::vec3(130.0f/255.0f, 130.0f / 255.0f, 130.0f / 255.0f));

	delete glBatch;
}

void Floor::GenerateSmallGridLines(float start, float end, float gap)
{
	auto glBatch = new GLBatch(100, false, false, false);
	glBatch->glBegin();

	int count = 0;

	for (float i = start; i <= end; i += gap)
	{
		if (count % 10 != 0)
		{
			glBatch->glVertex3f(start, 0.0f, (float)i);
			glBatch->glVertex3f(end, 0.0f, (float)i);
		}

		count++;
	}

	count = 0;
	for (float i = start; i <= end; i += gap)
	{
		if (count % 10 != 0)
		{
			glBatch->glVertex3f((float)i, 0.0f, start);
			glBatch->glVertex3f((float)i, 0.0f, end);
		}

		count++;
	}

	BaseModelIO BaseModelIO;
	BaseModelIO.SetVertexBuffer(glBatch->GetVertexBuffer(), glBatch->GetVertexBufferSize());
	BaseModelIO.SetColorBuffer(glBatch->GetColorBuffer(), glBatch->GetColorBufferSize());
	_smallGridLinesRenderer = new GLMeshRenderer(&BaseModelIO, BASIC_SHADER);
	_smallGridLinesRenderer->SetPrimitiveType(GLMeshRenderer::lines);

	float colorValue = 117.0f / 255.0f;
	BasicShader* basicShader = (BasicShader*)_smallGridLinesRenderer->GetCurrentShader();
	basicShader->SetColor(glm::vec3(colorValue, colorValue, colorValue));

	delete glBatch;
}

void Floor::GenerateGridLines(float start, float end, float gap)
{
	auto glBatch = new GLBatch(100, false, false, false);
	
	glBatch->glBegin();

	int count = 0;

	for (int i = (int)start; i <= end; i += (int)gap)
	{
		if(count % 5 != 0)
		{
			glBatch->glVertex3f(start, 0.0f, (float)i);
			glBatch->glVertex3f(end, 0.0f, (float)i);
		}

		count++;
	}

	count = 0;

	for (int i = (int)start; i <= end; i += (int)gap)
	{
		if (count % 5 != 0)
		{
			glBatch->glVertex3f((float)i, 0.0f, start);
			glBatch->glVertex3f((float)i, 0.0f, end);
		}

		count++;
	}

	glBatch->glColor3ub(145, 145, 145);


	BaseModelIO BaseModelIO;
	BaseModelIO.SetVertexBuffer(glBatch->GetVertexBuffer(), glBatch->GetVertexBufferSize());
	_gridLinesRenderer = new GLMeshRenderer(&BaseModelIO, BASIC_SHADER);
	_gridLinesRenderer->SetPrimitiveType(GLMeshRenderer::lines);

	float colorValue = 145.0f / 255.0f;
	BasicShader* shader = (BasicShader*)_gridLinesRenderer->GetCurrentShader();
	shader->SetColor(glm::vec3(colorValue, colorValue, colorValue));

	delete glBatch;
}

void Floor::GenerateBigGridLines(float start, float end, float gap)
{
	auto glBatch = new GLBatch(100, false, false, false);
	glBatch->glBegin();

	for (int i = (int)start; i <= end; i += (int)gap)
	{
		if (i == 0)
			continue;

		glBatch->glVertex3f(start, 0.0f, (float)i);
		glBatch->glVertex3f(end, 0.0f, (float)i);
	}

	for (int i = (int)start; i <= end; i += (int)gap)
	{
		if (i == 0)
			continue;

		glBatch->glVertex3f((float)i, 0.0f, start);
		glBatch->glVertex3f((float)i, 0.0f, end);
	}

	BaseModelIO BaseModelIO;
	BaseModelIO.SetVertexBuffer(glBatch->GetVertexBuffer(), glBatch->GetVertexBufferSize());
	_bigGridLinesRenderer = new GLMeshRenderer(&BaseModelIO, BASIC_SHADER);
	_bigGridLinesRenderer->SetPrimitiveType(GLMeshRenderer::lines);

	float colorValue = 181.0f / 255.0f;
	BasicShader* shader = (BasicShader*)_bigGridLinesRenderer->GetCurrentShader();
	shader->SetColor(glm::vec3(colorValue, colorValue, colorValue));

	delete glBatch;
}

void Floor::GenerateAxis(float start, float end)
{
	GLBatch* glBatch = new GLBatch(100, true, false, false);
	glBatch->glBegin();

	glBatch->glColor3ub(255, 0, 0);
	glBatch->glVertex3f(start, 0.0f, 0.0f);
	glBatch->glVertex3f(end, 0.0f, 0.0f);

	glBatch->glColor3ub(0, 0, 255);
	glBatch->glVertex3f(0.0f, 0.0f, start);
	glBatch->glVertex3f(0.0f, 0.0f, end);

	//glBatch->glColor3ub(0, 255, 0);
	//glBatch->glVertex3f(0.0f, 0.0f, (start + end) / 2.0f);
	//glBatch->glVertex3f(0.0f, end, 0);

	BaseModelIO BaseModelIO;
	BaseModelIO.SetVertexBuffer(glBatch->GetVertexBuffer(), glBatch->GetVertexBufferSize());
	BaseModelIO.SetColorBuffer(glBatch->GetColorBuffer(), glBatch->GetColorBufferSize());
	_axisRenderer = new GLMeshRenderer(&BaseModelIO, COLOR_SHADER);
	_axisRenderer->SetPrimitiveType(GLMeshRenderer::lines);
	_axisRenderer->SetAlpha(0.7f);

	delete glBatch;
}

bool Floor::IsVisible()
{
	return _visible;
}

bool Floor::IsAxisVisible()
{
	return _axisVisible;
}

bool Floor::IsGridVisible()
{
	return _gridVisible;
}

bool Floor::IsGridLinesVisible()
{
	return _gridLinesVisible;
}

void Floor::SetVisible(bool visible)
{
	_visible = visible;
}

void Floor::SetAxisVisible(bool axisVisible)
{
	_axisVisible = axisVisible;
}

void Floor::SetGridVisible(bool gridVisible)
{
	_gridVisible = gridVisible;
}

void Floor::SetGridLinesVisible(bool gridLinesVisible)
{
	_gridLinesVisible = gridLinesVisible;
}

void Floor::SetGridType(int gridType)
{
	_gridType = gridType;
}

void Floor::ShowOnlyGridLines(bool onlyGridLines)
{
	_gridVisible = !onlyGridLines;
	_smallGridLinesVisible = !onlyGridLines;
}

void Floor::Draw()
{
	if (_visible)
	{
		GLState::GLEnable(GL_LINE_SMOOTH, true);
		float lineWidth = GLState::GLLineWidth(1.0f);
		GLboolean blend = GLState::GLEnable(GL_BLEND, true);
		GLboolean depth = GLState::GLEnable(GL_DEPTH_TEST, true);
		GLboolean culling = GLState::GLEnable(GL_CULL_FACE, false);

		if (_gridVisible)
		{
			if(_gridType == GRID_SINGLE_RECT)
			{
				if(_gridRectRenderer)
					_gridRectRenderer->Draw();
			}
			else if (_gridType == GRID_MULTI_RECT)
			{
				if(_gridRenderer)
					_gridRenderer->Draw();
			}
		}

		if(_gridVisible)
		{
			GLState::GLEnable(GL_DEPTH_TEST, false);
		}

		if (_smallGridLinesVisible && _smallGridLinesRenderer)
		{
			GLState::GLLineWidth(1.0f);
			_smallGridLinesRenderer->Draw();
		}

		if (_gridLinesVisible && _gridLinesRenderer)
		{
			GLState::GLLineWidth(1.0f);
			_gridLinesRenderer->Draw();
		}

		if (_bigGridLinesVisible && _bigGridLinesRenderer)
		{
			GLState::GLLineWidth(1.0f);
			_bigGridLinesRenderer->Draw();
		}

		if (_axisVisible && _axisRenderer)
		{
			GLState::GLLineWidth(2.0f);
			_axisRenderer->Draw();
		}

		GLState::GLEnable(GL_CULL_FACE, culling);
		GLState::GLEnable(GL_DEPTH_TEST, depth);
		GLState::GLLineWidth(lineWidth);
		GLState::GLEnable(GL_BLEND, blend);
	}
}

Floor::~Floor()
{
	if (_axisRenderer)
	{
		delete _axisRenderer;
		_axisRenderer = nullptr;
	}

	if (_gridRenderer)
	{
		delete _gridRenderer;
		_gridRenderer = nullptr;
	}

	if (_gridLinesRenderer)
	{
		delete _gridLinesRenderer;
		_gridLinesRenderer = nullptr;
	}

	if(_gridRectRenderer)
	{
		delete _gridRectRenderer;
		_gridRectRenderer = NULL;
	}

	if(_smallGridLinesRenderer)
	{
		delete _smallGridLinesRenderer;
		_smallGridLinesRenderer = NULL;
	}

	if(_bigGridLinesRenderer)
	{
		delete _bigGridLinesRenderer;
		_bigGridLinesRenderer = NULL;
	}
}
