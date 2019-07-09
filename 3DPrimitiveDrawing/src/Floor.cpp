#include "Floor.h"
#include "Macros.h"
#include "GLUtil.h"
#include "ShadersManager.h"
#include "Cam.h"

Floor::Floor()
{
	_visible = true;
	_axisVisible = true;
	_gridLinesVisible = true;
	_gridVisible = false;

	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram("shaders/ColorArray/ColorArray.vs", "shaders/ColorArray/ColorArray.fs");

	float start = -16;
	float end = 16;
	float gap = 1;

	_axisBuffer = new GLBuffer(100, true, false, false);
	_gridBuffer = new GLBuffer(100, true, false, false);;
	_gridLinesBuffer = new GLBuffer(100, true, false, false);;

	_axisBuffer->glBegin(GL_LINES);

		_axisBuffer->glColor3ub(255,0,0);
		_axisBuffer->glVertex3f(start,	0.0f,	0.0f);
		_axisBuffer->glVertex3f(end,	0.0f,	0.0f);

		_axisBuffer->glColor3ub(0,0,255);
		_axisBuffer->glVertex3f(0.0f,	0.0f,	start);
		_axisBuffer->glVertex3f(0.0f,	0.0f,	end);

		_axisBuffer->glColor3ub(0,255,0);
		_axisBuffer->glVertex3f(0.0f,	0.0f,	(start+end)/2.0f);
		_axisBuffer->glVertex3f(0.0f,	end,	0);

	_axisBuffer->glEnd();


	_gridLinesBuffer->glBegin(GL_LINES);
	_gridLinesBuffer->glColor3ub(255,0,0);

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
	_gridLinesBuffer->glEnd();


	_gridBuffer->glBegin(GL_TRIANGLES);

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
	_gridBuffer->glEnd();
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
	if(_visible == false)
		return;

	float lineWidth = GLUtil::GLLineWidth(1.0f);
	GLboolean blend = GLUtil::GLEnable(GL_BLEND, true);

	_shaderProgram->Begin();

	GLint projMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "projMat");
	GLint viewMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "viewMat");
	GLint modelMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "modelMat");

	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, Cam::GetInstance()->projMat.m);
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, Cam::GetInstance()->viewMat.m);
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, _modelMat.m);

	if(_axisVisible)
	{
		glLineWidth(2.0f);

		GLuint colorID = glGetAttribLocation(_shaderProgram->ProgramID(), "color");
		glEnableVertexAttribArray(colorID);
		glBindBuffer(GL_ARRAY_BUFFER, _axisBuffer->GetColorBufferID());
		glVertexAttribPointer( colorID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		GLuint vertexID = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
		glEnableVertexAttribArray(vertexID);
		glBindBuffer(GL_ARRAY_BUFFER, _axisBuffer->GetVertexBufferID());
		glVertexAttribPointer( vertexID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_LINES, 0, _axisBuffer->GetVertexCount());
		//disable vertexattibarray here...
	}

	if(_gridLinesVisible)
	{
		glLineWidth(1.0f);

		GLuint colorID = glGetAttribLocation(_shaderProgram->ProgramID(), "color");
		glEnableVertexAttribArray(colorID);
		glBindBuffer(GL_ARRAY_BUFFER, _gridLinesBuffer->GetColorBufferID());
		glVertexAttribPointer( colorID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		GLuint vertexID = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
		glEnableVertexAttribArray(vertexID);
		glBindBuffer(GL_ARRAY_BUFFER, _gridLinesBuffer->GetVertexBufferID());
		glVertexAttribPointer( vertexID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_LINES, 0, _gridLinesBuffer->GetVertexCount());
		//disable vertexattibarray here...
	}

	if(_gridVisible)
	{
		GLuint colorID = glGetAttribLocation(_shaderProgram->ProgramID(), "color");
		glEnableVertexAttribArray(colorID);
		glBindBuffer(GL_ARRAY_BUFFER, _gridBuffer->GetColorBufferID());
		glVertexAttribPointer(colorID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		GLuint vertexID = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
		glEnableVertexAttribArray(vertexID);
		glBindBuffer(GL_ARRAY_BUFFER, _gridBuffer->GetVertexBufferID());
		glVertexAttribPointer( vertexID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, _gridBuffer->GetVertexCount());
		//disable vertexattibarray here...
	}

	_shaderProgram->End();

	GLUtil::GLLineWidth(lineWidth);
	GLUtil::GLEnable(GL_BLEND, blend);
}

Floor::~Floor()
{
	ShadersManager::GetInstance()->DeleteShaderProgram(_shaderProgram);

	if (_axisBuffer != NULL)
	{
		delete _axisBuffer;
	}

	if (_gridBuffer != NULL)
	{
		delete _gridBuffer;
	}

	if (_gridLinesBuffer != NULL)
	{
		delete _gridLinesBuffer;
	}
}
