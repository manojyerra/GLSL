#include "Floor.h"
#include "DefinesAndIncludes.h"
#include "GLUtil/GLUtil.h"

Floor::Floor()
{
	_visible = true;
	_axisVisible = true;
	_gridVisible = true;
	_gridLinesVisible = true;

	shaderProgram = new ShaderProgram("shaders/passThroughShader.vs", "shaders/passThroughShader.fs");

	float start = -16;
	float end = 16;
	float gap = 1;

	_axisBuffer = new GLBuffer(100, true, false, false);
	_gridBuffer = new GLBuffer(100, true, false, false);;
	_gridLinesBuffer = new GLBuffer(100, true, false, false);;

	_axisBuffer->glBegin(GL_LINES);

		_axisBuffer->glColor4ub(255,0,0,255);
		_axisBuffer->glVertex3f(start,	0.0f,	0.0f);
		_axisBuffer->glVertex3f(end,	0.0f,	0.0f);

		_axisBuffer->glColor4ub(0,0,255,255);
		_axisBuffer->glVertex3f(0.0f,	0.0f,	start);
		_axisBuffer->glVertex3f(0.0f,	0.0f,	end);

		_axisBuffer->glColor4ub(0,255,0,255);
		_axisBuffer->glVertex3f(0.0f,	0.0f,	(start+end)/2.0f);
		_axisBuffer->glVertex3f(0.0f,	end,	0);

	_axisBuffer->glEnd();


	_gridBuffer->glBegin(GL_LINES);
	_gridBuffer->glColor4ub(255,0,0,60);

	for(int i=(int)start; i<=end; i+=(int)gap)
	{
		if(i == 0)
			continue;

		_gridBuffer->glVertex3f(start,	0.01f,	(float)i);
		_gridBuffer->glVertex3f(end,		0.01f,	(float)i);
	}

	for(int i=(int)start; i<=end; i+=(int)gap)
	{
		if(i == 0)
			continue;

		_gridBuffer->glVertex3f((float)i,	0.01f,	start);
		_gridBuffer->glVertex3f((float)i,	0.01f,	end);
	}
	_gridBuffer->glEnd();


	_gridLinesBuffer->glBegin(GL_QUADS);

	int c1 = 255;
	int c2 = 158;
	for(int outer = (int)start; outer < end; outer++)
	{
		int temp = c1;
		c1 = c2;
		c2 = temp;
		for(int i=(int)start; i<end; i+=(int)gap)
		{
			if(i%2 == 0)
				glColor4ub(c1,c2,158,255);
			else
				glColor4ub(c2,c1,158,255);

			_gridLinesBuffer->glVertex3f((float)i,		0, outer*gap);
			_gridLinesBuffer->glVertex3f((float)i+gap,	0, outer*gap);
			_gridLinesBuffer->glVertex3f((float)i+gap,	0, (outer+1)*gap);
			_gridLinesBuffer->glVertex3f((float)i,		0, (outer+1)*gap);
		}
	}
	_gridLinesBuffer->glEnd();
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
	
	glPushMatrix();
	shaderProgram->Begin();
	//shaderProgram->SetUniform4fv("uColor", 0.0f, 1.0f, 0.0f, 1.0f);

	float lineWidth_bk = GLUtil::GLLineWidth(1.0f);
	GLboolean glLighting = GLUtil::GLEnable(GL_LIGHTING, false);
	unsigned int prevColor = GLUtil::GLColor(0xffffffff);
	GLboolean blend = GLUtil::GLEnable(GL_BLEND, true);

	if(_axisVisible)
	{
		glLineWidth(1.0f);
		_axisBuffer->Draw(shaderProgram->ProgramID());
	}

	if(_gridLinesVisible)
	{
		glLineWidth(1.0f);
		_gridBuffer->Draw(shaderProgram->ProgramID());
	}

	//if(_gridVisible)
	//{
	//	_gridLinesBuffer->Draw(shaderProgram->GetProgramObject());
	//}

	GLUtil::GLColor(prevColor);
	GLUtil::GLLineWidth(lineWidth_bk);
	GLUtil::GLEnable(GL_LIGHTING, glLighting);
	GLUtil::GLEnable(GL_BLEND, blend);

	shaderProgram->End();
	glPopMatrix();
}


	//glPushMatrix();

	//shaderProgram->Begin();

	//shaderProgram->SetUniform4fv("uColor", 1.0f, 0.0f, 0.0f, 1.0f);

	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	////glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//glDrawArrays(GL_LINES, 0, _numVertex); // Starting from vertex 0; 3 vertices total -> 1 triangle
	//glDisableVertexAttribArray(0);

	//shaderProgram->End();

	//glPopMatrix();