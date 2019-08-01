#include "Sphere.h"
#include "GLUtil.h"
#include "BufferTransformUtils.h"
#include "ShaderProgramsManager.h"
#include "Cam.h"
#include "MathUtils.h"

Sphere::Sphere(float x, float y, float z, float r) : Shape(Shape::SPHERE)
{
	m[12] = x;
	m[13] = y;
	m[14] = z;
	_r = r;
	GenerateGeometry();
}

Sphere::Sphere(Sphere* sphere) : Shape(Shape::SPHERE)
{
	memcpy(m, sphere->m, 16 * 4);
	_r = sphere->GetRadius();
	GenerateGeometry();
}

Sphere::Sphere(float* mat, float r) : Shape(Shape::SPHERE)
{
	memcpy(m, mat, 16 * 4);
	_r = r;
	GenerateGeometry();
}

Sphere::Sphere(glm::vec3 pos, float r) : Shape(Shape::SPHERE)
{
	m[12] = pos.x;
	m[13] = pos.y;
	m[14] = pos.z;
	_r = r;
	GenerateGeometry();
}

Sphere Sphere::CalcBoundingSphere(float* vertexBuf, int arrSize)
{
	float* localVertexBuf = new float[arrSize];
	memcpy(localVertexBuf, vertexBuf, arrSize * sizeof(float));

	glm::vec3 center = BufferTransformUtils::CalcCenter(localVertexBuf, arrSize);
	BufferTransformUtils::Subtract(localVertexBuf, arrSize, center);

	float rPow2 = 0;

	for (int i = 0; i < arrSize; i += 3)
	{
		float x = localVertexBuf[i + 0];
		float y = localVertexBuf[i + 1];
		float z = localVertexBuf[i + 2];

		float lenPow2 = x * x + y * y + z * z;

		if (rPow2 < lenPow2)
			rPow2 = lenPow2;
	}

	delete[] localVertexBuf;

	return Sphere(center, sqrt(rPow2));
}

float Sphere::GetRadius()
{
	return _r;
}

void Sphere::SetRadius(float r)
{
	if (r > 0)
		_r = r;
}

float Sphere::Volume()
{
	return (4.0f / 3.0f) * PI_VAL * _r * _r * _r;
}

void Sphere::Draw()
{
	if (!_visible)
		return;

	GLMat modelMat(m);
	modelMat.multMatrixf(_scaleMat.m);

	_meshRenderer->SetModelMatrix(modelMat.m);
	_meshRenderer->Draw();
}

void Sphere::GenerateGeometry()
{
	GLBatch* buffer = new GLBatch(100, false, false, true);

	float radius = 0.5f;
	float piVal = 3.14159265f;

	buffer->glBegin();

	float degToRad = 0.017453f;

	int delta = 5;

	float x = radius;
	float y = 0;
	float z = 0;

	//_buffer->glColor(0xff0000ff);

	for (int zAngle = -90; zAngle < 90; zAngle += 2 * delta)
	{
		float Xz1 = 0;
		float Yz1 = 0;
		float Zz1 = 0;

		float Xz2 = 0;
		float Yz2 = 0;
		float Zz2 = 0;

		rot(3, (float)zAngle, x, y, z, &Xz1, &Yz1, &Zz1);
		rot(3, (float)zAngle + 2.0f * delta, x, y, z, &Xz2, &Yz2, &Zz2);

		for (int yAngle = 0; yAngle <= 360; yAngle += delta)
		{
			float x1, y1, z1;
			float x2, y2, z2;
			float x3, y3, z3;
			float x4, y4, z4;

			rot(2, (float)yAngle, Xz1, Yz1, Zz1, &x1, &y1, &z1);
			rot(2, (float)yAngle, Xz2, Yz2, Zz2, &x2, &y2, &z2);

			rot(2, (float)(yAngle + delta), Xz1, Yz1, Zz1, &x3, &y3, &z3);
			rot(2, (float)(yAngle + delta), Xz2, Yz2, Zz2, &x4, &y4, &z4);

			buffer->glNormal3f(x3, y3, z3);
			buffer->glVertex3f(x3, y3, z3);

			buffer->glNormal3f(x2, y2, z2);
			buffer->glVertex3f(x2, y2, z2);

			buffer->glNormal3f(x1, y1, z1);
			buffer->glVertex3f(x1, y1, z1);

			buffer->glNormal3f(x2, y2, z2);
			buffer->glVertex3f(x2, y2, z2);

			buffer->glNormal3f(x3, y3, z3);
			buffer->glVertex3f(x3, y3, z3);

			buffer->glNormal3f(x4, y4, z4);
			buffer->glVertex3f(x4, y4, z4);
		}
	}

	BaseModelIO createInfo;
	createInfo.SetVertexBuffer(buffer->GetVertexBuffer(), buffer->GetVertexBufferSize());
	createInfo.SetNormalBuffer(buffer->GetNormalBuffer(), buffer->GetNormalBufferSize());

	_meshRenderer = new GLMeshRenderer(&createInfo, PHONG_PER_PIXEL_SHADER);

	delete buffer;
}

void Sphere::rot(int axis, float angleInDegrees, float x, float y, float z, float* newX, float* newY, float* newZ)
{
	float angleInRadians = angleInDegrees * DEG_RAD;

	float cosVal = cosf(angleInRadians);
	float sinVal = sinf(angleInRadians);

	if (axis == 3)
	{
		newX[0] = x * cosVal - y * sinVal;
		newY[0] = x * sinVal + y * cosVal;
		newZ[0] = z;
	}
	else if (axis == 1)
	{
		newY[0] = y * cosVal - z * sinVal;
		newZ[0] = y * sinVal + z * cosVal;
		newX[0] = x;
	}
	else if (axis == 2)
	{
		newZ[0] = z * cosVal - x * sinVal;
		newX[0] = z * sinVal + x * cosVal;
		newY[0] = y;
	}
}

Sphere::~Sphere()
{
	if (_meshRenderer)
	{
		delete _meshRenderer;
		_meshRenderer = NULL;
	}
}

//// Create light components
//GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
//GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0, 1.0f };
//GLfloat specularLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };

//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientLight);
//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseLight);
//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100);

//GLfloat white[] = {0.8f, 0.8f, 0.8f, 1.0f};
//GLfloat cyan[] = {1.0f, 1.0f, 1.0f, 1.f};
//GLfloat shininess[] = {50};

//glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);
//glMaterialfv(GL_FRONT, GL_SPECULAR, white);
//glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

//glUniform4f(glGetUniformLocation(_shaderProgram->ProgramID(), "ambient"), 0.5f, 0.0f, 0.0f, 1.0f);
//glUniform4f(glGetUniformLocation(_shaderProgram->ProgramID(), "diffuse"), 0.4f, 0.4f, 0.5f, 1.0f);
//glUniform4f(glGetUniformLocation(_shaderProgram->ProgramID(), "specular"), 0.8f, 0.8f, 0.0f, 1.0f);
//glUniform1f(glGetUniformLocation(_shaderProgram->ProgramID(), "shininess"), 0.5f);