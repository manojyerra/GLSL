#include "Cone.h"
#include "MathUtils.h"
#include "GLUtil.h"
#include "BufferTransformUtils.h"
#include "Cylinder.h"
#include "ShaderProgramsManager.h"
#include "Cam.h"

Cone::Cone() : Shape(Shape::CONE)
{
	_r = 0;
	_h = 0;

	GenerateGeometry();
}

Cone::Cone(float* mat, float r, float h) : Shape(Shape::CONE)
{
	memcpy(m, mat, 16 * 4);

	_r = r;
	_h = h;

	GenerateGeometry();
}

Cone::Cone(float x, float y, float z, float r, float h) : Shape(Shape::CONE)
{
	m[12] = x;
	m[13] = y;
	m[14] = z;

	_r = r;
	_h = h;

	GenerateGeometry();
}

Cone::Cone(Cone* cone)
{
	memcpy(m, cone->m, 16 * 4);

	_r = cone->GetRadius();
	_h = cone->GetHeight();
	_id = cone->GetID();

	GenerateGeometry();
}

glm::vec3 Cone::GetPos()
{
	return glm::vec3(m[12], m[13], m[14]);
}

float Cone::GetRadius() { return _r; }
float Cone::GetHeight() { return _h; }

void Cone::SetRadius(float r)
{
	if (r > 0)
		_r = r;
}
void Cone::SetHeight(float h)
{
	if (h > 0)
		_h = h;
}

float Cone::Volume()
{
	return (1.0f / 3.0f) * PI_VAL * _r * _r * _h;
}

Cone Cone::CalcBoundingCone(float* vertexBuf, int arrSize)
{
	float* localVertexBuf = new float[arrSize];
	memcpy(localVertexBuf, vertexBuf, arrSize * sizeof(float));

	Cylinder cylinder = Cylinder::CalcBoundingCylinder(vertexBuf, arrSize);

	float cylMatInvert[16];
	GLMat::InvertMatrix(cylinder.GetGLMatrix(), cylMatInvert);

	cylMatInvert[13] += cylinder.GetHeight() / 2.0f;

	BufferTransformUtils::MulBufWithMatrix(localVertexBuf, arrSize, cylMatInvert);

	float height = cylinder.GetHeight() * 1.05f;
	float radius = CalcRadius(localVertexBuf, arrSize, height);
	float volume = (1.0f / 3.0f) * PI_VAL * radius * radius * height;

	float finalH = height;

	bool addX180Rot = false;

	for (int loop = 0; loop < 2; loop++)
	{
		if (loop == 1)
		{
			float cylinderH = cylinder.GetHeight();

			for (int i = 3; i < arrSize; i += 3)
				localVertexBuf[i + 1] = cylinderH - localVertexBuf[i + 1];
		}

		for (float i = 1.1f; i < 2.0f; i += 0.1f)
		{
			float currH = height * i;

			float r = CalcRadius(localVertexBuf, arrSize, currH);
			float v = (1.0f / 3.0f) * PI_VAL * r * r * currH;

			if (v < volume)
			{
				volume = v;
				radius = r;
				finalH = currH;

				if (loop == 1)
					addX180Rot = true;
			}
		}
	}

	delete[] localVertexBuf;

	float matInvert[16];
	GLMat::InvertMatrix(cylMatInvert, matInvert);

	GLMat mat;
	mat.multMatrixf(matInvert);

	if (addX180Rot)
	{
		mat.translatef(0, cylinder.GetHeight(), 0);
		mat.rotatef(180, 1, 0, 0);
	}

	mat.translatef(0, finalH / 2, 0);

	Cone cone(mat.m, radius, finalH);

	return cone;
}

float Cone::CalcRadius(float* vertexBuf, int arrSize, float height)
{
	float x = vertexBuf[0];
	float y = vertexBuf[1];
	float z = vertexBuf[2];

	float dy = abs(height - y);
	float xOnXZPlane = height * x / dy;
	float zOnXZPlane = height * z / dy;

	float rPow2 = xOnXZPlane * xOnXZPlane + zOnXZPlane * zOnXZPlane;

	for (int i = 3; i < arrSize; i += 3)
	{
		x = vertexBuf[i + 0];
		y = vertexBuf[i + 1];
		z = vertexBuf[i + 2];

		dy = height - y;
		xOnXZPlane = height * x / dy;
		zOnXZPlane = height * z / dy;

		float dist = xOnXZPlane * xOnXZPlane + zOnXZPlane * zOnXZPlane;

		if (rPow2 < dist)
			rPow2 = dist;
	}

	return sqrt(rPow2);
}

void Cone::Draw()
{
	if (!_visible)
		return;

	_scaleMat.m[0] = _r * 2;
	_scaleMat.m[5] = _h;
	_scaleMat.m[10] = _r * 2;

	GLMat modelMat(m);
	modelMat.multMatrixf(_scaleMat.m);

	_meshRenderer->SetModelMatrix(modelMat.m);
	_meshRenderer->Draw();
}

void Cone::GenerateGeometry()
{
	GLBatch* buffer = new GLBatch(100, true, false, false);

	float radius = 0.5f;
	float halfLength = 0.5f;
	float piVal = 3.14159265f;

	buffer->glBegin();

	if (_useRandomColors)
		_randomColor.Reset();

	for (int i = 20; i <= 360; i += 20)
	{
		float theta = i * piVal / 180.0f;
		float nextTheta = (i + 20)*piVal / 180.0f;

		if (_useRandomColors)
			buffer->glColor(_randomColor.NextColor());

		buffer->glVertex3f(0, halfLength, 0);

		buffer->glColor3ub(80, 80, 80);
		buffer->glVertex3f(radius*cos(nextTheta), -halfLength, radius*sin(nextTheta));
		buffer->glVertex3f(radius*cos(theta), -halfLength, radius*sin(theta));

		buffer->glVertex3f(0, -halfLength, 0);

		if (_useRandomColors)
			buffer->glColor(_randomColor.NextColor());

		buffer->glVertex3f(radius*cos(theta), -halfLength, radius*sin(theta));
		buffer->glVertex3f(radius*cos(nextTheta), -halfLength, radius*sin(nextTheta));
	}

	ModelInfo createInfo;
	createInfo.SetVertexBuffer(buffer->GetVertexBuffer(), buffer->GetVertexBufferSize());
	createInfo.SetColorBuffer(buffer->GetColorBuffer(), buffer->GetColorBufferSize());

	_meshRenderer = new GLMeshRenderer(&createInfo, GLMeshRenderer::COLOR_SHADER);

	delete buffer;
}

Cone::~Cone()
{
	if (_meshRenderer)
	{
		delete _meshRenderer;
		_meshRenderer = NULL;
	}
}