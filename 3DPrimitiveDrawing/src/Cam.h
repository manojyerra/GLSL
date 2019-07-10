#ifndef Cam_H
#define Cam_H

#include "Macros.h"
#include "GLMat.h"
#include "Vector3.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "glm/ext.hpp"

#include <vector>
using namespace std;

class Cam
{
private:
	static Cam* _ref;

	float _left;
	float _right;
	float _bottom;
	float _top;

	float _zNear;
	float _zFar;
	float _zNearPlaneHalfW;

	CVector3 _pivot;
	CVector3 _angle;
	CVector3 _trans;

	bool _isOrtho;

	float _sw;
	float _sh;

	int _viewType;

	Cam();

	void SetProjection();

public:
	GLMat projMat;
	GLMat viewMat;
	
	static Cam* GetInstance();
	static void DeleteInstance();

	void Init(int screenW, int screenH, float zNear, float zFar, float zNearPlaneW);
	void SetScreenSize(int sw, int sh);

	void SetPerspectiveProjection();
	void SetOrthoProjection();
	void SetViewMatrix();
	bool IsOrthoProjection();
	
	glm::mat4 GetMVP(float* modelMat);
	glm::mat4 GetModelView(float* modelMat);
	glm::mat3 GetNormalMat(float* modelMat);
	
	bool UpdateCamera();

	void SetPivot(CVector3 pivotPoint);
	void SetTrans(CVector3 trans);
	void SetRot(CVector3 rot);

	void SetFrontView();
	void SetBackView();
	void SetLeftView();
	void SetRightView();
	void SetTopView();
	void SetBottomView();
	void ChangeView();
};

#endif
