#ifndef Cam2D_H
#define Cam2D_H

#include "Macros.h"
#include "GLMat.h"
#include "glm/ext.hpp"

#include <vector>
using namespace std;

class Cam2D
{
private:
	static Cam2D* _ref;

	float _left;
	float _right;
	float _bottom;
	float _top;

	float _zNear;
	float _zFar;
	float _zNearPlaneHalfW;

	glm::vec3 _pivot;
	glm::vec3 _angle;
	glm::vec3 _trans;

	bool _isOrtho;

	float _sw;
	float _sh;

	int _viewType;

	Cam2D();

	void SetProjection();

public:
	GLMat projMat;
	GLMat viewMat;
	
	static Cam2D* GetInstance();
	static void DeleteInstance();

	void Init(int screenW, int screenH, float zNear, float zFar, float zNearPlaneW);
	void SetScreenSize(int sw, int sh);

	void SetOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar);
	
	glm::mat4 GetMVP(float* modelMat);
	glm::mat4 GetModelView(float* modelMat);
	glm::mat3 GetNormalMat(float* modelMat);
	
	bool UpdateCamera();
};

#endif
