#include "Floor.h"
#include "Macros.h"
#include "GLState.h"
#include "ShadersManager.h"
#include "Cam.h"
#include "GLBuffer.h"

Floor::Floor()
{
	_visible = true;
	_axisVisible = true;
	_gridLinesVisible = true;
	_gridVisible = false;

	float start = -16;
	float end = 16;
	float gap = 1;

	/////////////////////  Begin : Axis creation ////////////////////

	GLBuffer* _axisBuffer = new GLBuffer(100, true, false, false);
	_axisBuffer->glBegin();

	_axisBuffer->glColor3ub(255,0,0);
	_axisBuffer->glVertex3f(start,	0.0f,	0.0f);
	_axisBuffer->glVertex3f(end,	0.0f,	0.0f);

	_axisBuffer->glColor3ub(0,0,255);
	_axisBuffer->glVertex3f(0.0f,	0.0f,	start);
	_axisBuffer->glVertex3f(0.0f,	0.0f,	end);

	_axisBuffer->glColor3ub(0,255,0);
	_axisBuffer->glVertex3f(0.0f,	0.0f,	(start+end)/2.0f);
	_axisBuffer->glVertex3f(0.0f,	end,	0);

	ModelInfo axisMeshInfo;
	axisMeshInfo.SetVertexBuffer(_axisBuffer->GetVertexBuffer(), _axisBuffer->GetVertexBufferSize());
	axisMeshInfo.SetColorBuffer(_axisBuffer->GetColorBuffer(), _axisBuffer->GetColorBufferSize());
	_axisRenderer = new GLMeshRenderer(&axisMeshInfo, GLMeshRenderer::COLOR_SHADER);
	_axisRenderer->SetPrimitiveType(GLMeshRenderer::lines);
	delete _axisBuffer;

	/////////////////////  End : Axis creation ////////////////////



	/////////////////////  Begin : Grid lines creation ////////////////////

	GLBuffer* _gridLinesBuffer = new GLBuffer(100, true, false, false);
	_gridLinesBuffer->glBegin();
	_gridLinesBuffer->glColor3ub(75,75,75);

	for(int i=(int)start; i<=end; i+=(int)gap)
	{
		if(i == 0)
			continue;

		_gridLinesBuffer->glVertex3f(start,	0.01f,	(float)i);
		_gridLinesBuffer->glVertex3f(end,		0.01f,	(float)i);
	}

	for(int i=(int)start; i<=end; i+=(int)gap)
	{
		if(i == 0)
			continue;

		_gridLinesBuffer->glVertex3f((float)i,	0.01f,	start);
		_gridLinesBuffer->glVertex3f((float)i,	0.01f,	end);
	}
	
	ModelInfo gridLinesMeshInfo;
	gridLinesMeshInfo.SetVertexBuffer(_gridLinesBuffer->GetVertexBuffer(), _gridLinesBuffer->GetVertexBufferSize());
	gridLinesMeshInfo.SetColorBuffer(_gridLinesBuffer->GetColorBuffer(), _gridLinesBuffer->GetColorBufferSize());
	_gridLinesRenderer = new GLMeshRenderer(&gridLinesMeshInfo, GLMeshRenderer::COLOR_SHADER);
	_gridLinesRenderer->SetPrimitiveType(GLMeshRenderer::lines);

	delete _gridLinesBuffer;

	/////////////////////  Begin : Grid lines creation ////////////////////



	/////////////////////  Begin : Grid creation ////////////////////

	GLBuffer* _gridBuffer = new GLBuffer(100, true, false, false);
	_gridBuffer->glBegin();

	int c1 = 255;
	int c2 = 158;
	for (int outer = (int)start; outer < end; outer++)
	{
		int temp = c1;
		c1 = c2;
		c2 = temp;
		for (int i = (int)start; i < end; i += (int)gap)
		{
			if (i % 2 == 0)
				_gridBuffer->glColor3ub(c1, c2, 158);
			else
				_gridBuffer->glColor3ub(c2, c1, 158);

			_gridBuffer->glVertex3f((float)i,		0, outer*gap);
			_gridBuffer->glVertex3f((float)i+gap,	0, outer*gap);
			_gridBuffer->glVertex3f((float)i, 0, (outer + 1)*gap);

			_gridBuffer->glVertex3f((float)i + gap, 0, outer*gap);
			_gridBuffer->glVertex3f((float)i, 0, (outer + 1)*gap);
			_gridBuffer->glVertex3f((float)i+gap,	0, (outer+1)*gap);
		}
	}

	ModelInfo gridMeshInfo;
	gridMeshInfo.SetVertexBuffer(_gridBuffer->GetVertexBuffer(), _gridBuffer->GetVertexBufferSize());
	gridMeshInfo.SetColorBuffer(_gridBuffer->GetColorBuffer(), _gridBuffer->GetColorBufferSize());
	_gridRenderer = new GLMeshRenderer(&gridMeshInfo, GLMeshRenderer::COLOR_SHADER);

	delete _gridBuffer;

	/////////////////////  Begin : Grid creation ////////////////////	
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

void Floor::Draw()
{
	if (_visible)
	{
		float lineWidth = GLState::GLLineWidth(1.0f);
		GLboolean blend = GLState::GLEnable(GL_BLEND, true);

		if (_axisVisible)
		{
			GLState::GLLineWidth(2.0f);
			_axisRenderer->Draw();
		}

		if (_gridLinesVisible)
		{
			GLState::GLLineWidth(1.0f);
			_gridLinesRenderer->Draw();
		}

		if (_gridVisible)
		{
			_gridRenderer->Draw();
		}

		GLState::GLLineWidth(lineWidth);
		GLState::GLEnable(GL_BLEND, blend);
	}
}

Floor::~Floor()
{
	if (_axisRenderer)
	{
		delete _axisRenderer;
		_axisRenderer = NULL;
	}

	if (_gridRenderer)
	{
		delete _gridRenderer;
		_gridRenderer = NULL;
	}

	if (_gridLinesRenderer)
	{
		delete _gridLinesRenderer;
		_gridLinesRenderer = NULL;
	}
}
