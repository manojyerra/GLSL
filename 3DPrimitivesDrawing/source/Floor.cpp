#include "Floor.h"
#include "DefinesAndIncludes.h"


Floor::Floor()
{
	_visible = true;
	_axisVisible = true;
	_gridVisible = false;
	_gridLinesVisible = true;
	_numVertex = 0;

	shaderProgram = new ShaderProgram("shaders/passThroughShader.vs", "shaders/passThroughShader.fs");

	GLBatch* batch = new GLBatch(50, false, false, false);

	AddVertex(batch);

	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, batch->GetVertexCount() * 3 * 4, batch->GetVertexArr(), GL_STATIC_DRAW);

	if(batch)
	{
		delete batch;
		batch = NULL;
	}
}

void Floor::AddVertex(GLBatch* batch)
{
	//float lineWidth_bk = GLUtil::GLLineWidth(1.0f);
	//GLboolean glLighting = GLUtil::GLEnable(GL_LIGHTING, false);
	//unsigned int prevColor = GLUtil::GLColor(0xffffffff);
	//GLboolean blend = GLUtil::GLEnable(GL_BLEND, true);

	float start = -16;
	float end = 16;
	float gap = 1;

	if(_axisVisible)
	{
		glLineWidth(3.0f);		//this need to be convert into shaders

		batch->glBegin(GL_LINES);

			batch->glColor4ub(255,	0,0,255);

			batch->glVertex3f(start,	0.0f,	0.0f);
			batch->glVertex3f(end,		0.0f,	0.0f);

			batch->glColor4ub(0,0,255,255);

			batch->glVertex3f(0.0f,	0.0f,	start);
			batch->glVertex3f(0.0f,	0.0f,	end);

			batch->glColor4ub(0,255,0,255);

			batch->glVertex3f(0.0f,	0.0f,	(start+end)/2.0f);
			batch->glVertex3f(0.0f,	end,	0);

		//batch->glEnd();
	}


	if(_gridLinesVisible)
	{
		glLineWidth(1.0f);

		//batch->glBegin(GL_LINES);
		batch->glColor4ub(255,255,255,60);

		for(int i=(int)start; i<=end; i+=(int)gap)
		{
			if(i == 0)
				continue;

			batch->glVertex3f(start,	0.01f,	(float)i);
			batch->glVertex3f(end,		0.01f,	(float)i);
		}

		for(int i=(int)start; i<=end; i+=(int)gap)
		{
			if(i == 0)
				continue;

			batch->glVertex3f((float)i,	0.01f,	start);
			batch->glVertex3f((float)i,	0.01f,	end);
		}
		//glEnd();
	}


	//if(_gridVisible)
	//{
	//	glBegin(GL_QUADS);

	//	int c1 = 255;
	//	int c2 = 158;
	//	for(int outer = (int)start; outer < end; outer++)
	//	{
	//		int temp = c1;
	//		c1 = c2;
	//		c2 = temp;
	//		for(int i=(int)start; i<end; i+=(int)gap)
	//		{
	//			if(i%2 == 0)
	//				glColor4ub(c1,c2,158,255);
	//			else
	//				glColor4ub(c2,c1,158,255);

	//			glVertex3f((float)i,		0, outer*gap);
	//			glVertex3f((float)i+gap,	0, outer*gap);
	//			glVertex3f((float)i+gap,	0, (outer+1)*gap);
	//			glVertex3f((float)i,		0, (outer+1)*gap);
	//		}
	//	}
	//	glEnd();

	//	/*
	//	glColor4ub(255,255,255,255);
	//	glBegin(GL_TRIANGLES);
	//		glVertex3f(start,0,start);
	//		glVertex3f(start,0,end);
	//		glVertex3f(end,0,end);
	//		glVertex3f(start,0,start);
	//		glVertex3f(end,0,end);
	//		glVertex3f(end,0,start);
	//	glEnd();
	//	*/
	//}

	//GLUtil::GLColor(prevColor);
	//GLUtil::GLLineWidth(lineWidth_bk);
	//GLUtil::GLEnable(GL_LIGHTING, glLighting);
	//GLUtil::GLEnable(GL_BLEND, blend);

	_numVertex = batch->GetVertexCount();
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

	shaderProgram->SetUniform4fv("uColor", 1.0f, 0.0f, 0.0f, 1.0f);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	//glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_LINES, 0, _numVertex); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);

	shaderProgram->End();

	glPopMatrix();
}
