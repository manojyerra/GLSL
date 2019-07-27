#ifndef GLMat_H
#define GLMat_H

#include "glm/vec3.hpp"

class GLMat
{
private:
	void SetIdentityMatrix(float* m);
	void GetXRotArray(float angle, float* result);
	void GetYRotArray(float angle, float* result);
	void GetZRotArray(float angle, float* result);

public:
	float m[16];

	GLMat();
	GLMat(float* arr);
	~GLMat();

	void frustum(float left, float right, float bottom, float top, float zNear, float zFar);
	void ortho(float l, float r, float b, float t, float n, float f);
	void loadIdentity();
	void translatef(float x, float y, float z, bool reverseOrder = false);
	void rotatef(float angleInDegrees, int isX, int isY, int isZ, bool reverseOrder = false);
	void scalef(float x, float y, float z, bool reverseOrder = false);
	void multMatrixf(float* mat);

	float* Get();
	void Copy(float* mat);
	void CopyRotation(float* mat);

	glm::vec3 GetEulerXYZRot_In_Degrees();
	glm::vec3 GetEulerXYZRot_In_Radians();
	glm::vec3 GetScale();
	void RemoveScale();
	void SetScale(glm::vec3 scale);

	void SetPos(float x, float y, float z);
	void SetPos(glm::vec3 pos);
	glm::vec3 GetPos();
	glm::vec3 GetRotation();
	void SetRotation(glm::vec3 rot);

	void AddTransInWorld(float x, float y, float z);
	void AddTransInLocal(char axis, float move);
	void AddRotateInWorld(char axis, float angle);
	void AddRotateInLocal(char axis, float angle);

	static glm::vec3 MultVector(float* a, glm::vec3 vec);
	static void MultMat(float* a, float* b, float* result);
	static int InvertMatrix(const float src[16], float inverse[16]);
	static void GetGluLookAtParameters(float* m, float* gluLookAtParams);
};

#endif
