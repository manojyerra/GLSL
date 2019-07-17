#include "Triangle.h"
#include "GLBuffer.h"

Triangle::Triangle(glm::vec3& v1, glm::vec3& v2, glm::vec3& v3)
{
	GLBuffer* buffer = new GLBuffer(50, true, false, false);

	buffer->glBegin(GL_TRIANGLES);
	buffer->glColor3ub(255,0,0);
	buffer->glVertex3f(v1.x, v1.y, v1.z);
	buffer->glVertex3f(v2.x, v2.y, v2.z);
	buffer->glVertex3f(v3.x, v3.y, v3.z);

	ModelInfo createInfo;
	createInfo.SetVertexBuffer(buffer->GetVertexBuffer(), buffer->GetVertexBufferSize());
	createInfo.SetColorBuffer(buffer->GetColorBuffer(), buffer->GetColorBufferSize());

	_meshRenderer = new GLMeshRenderer(&createInfo);
	_meshRenderer->SetShader(GLMeshRenderer::COLOR_SHADER);

	delete buffer;
}

void Triangle::Draw()
{
	_meshRenderer->Draw();
}

Triangle::~Triangle()
{
	if (_meshRenderer)
	{
		delete _meshRenderer;
		_meshRenderer = NULL;
	}
}