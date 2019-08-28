#ifndef Cam_H
#define Cam_H

#include "GLMat.h"
#include "glm/ext.hpp"

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

	glm::vec3 _pivot;
	glm::vec3 _angle;
	glm::vec3 _trans;
	bool _zUpRot;

	bool _isOrtho;

	float _sw;
	float _sh;

	int _viewType;
	bool _camUpdated;

	Cam();

	void SetProjection();

public:
	GLMat projMat;
	GLMat viewMat;
	
	static Cam* GetInstance();
	static void DeleteInstance();

	void Init(float screenW, float screenH, float zNear, float zFar, float zNearPlaneW);
	void SetScreenSize(float sw, float sh);

	void SetPerspectiveProjection();
	void SetOrthoProjection();
	void SetViewMatrix();
	bool IsOrthoProjection();
	
	glm::mat4 GetMVP(float* modelMat);
	glm::mat4 GetModelViewMat(float* modelMat);
	glm::mat3 GetNormalMat(float* modelMat);
	glm::mat4 GetProjMat();
	glm::mat4 GetViewMat();

	bool UpdateCamera();
	bool IsCameraUpdated();

	void SetPivot(glm::vec3 pivotPoint);
	void SetTrans(glm::vec3 trans);
	void SetRot(glm::vec3 rot);
	void SetEnableZUpRot(bool zUpRot);
	glm::vec3 GetTrans();

	void SetFrontView();
	void SetBackView();
	void SetLeftView();
	void SetRightView();
	void SetTopView();
	void SetBottomView();
	void ChangeView();
	void ResetToInitialPos();
};

#endif
