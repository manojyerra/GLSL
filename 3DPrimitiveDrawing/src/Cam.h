#ifndef Cam_H
#define Cam_H

#include "Macros.h"
#include "GLMat.h"
#include "Vector3.h"

#include <vector>
using namespace std;

class Cam
{
private:
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

	float SW;
	float SH;

	int _viewType;
	float normalMat[9];

	Cam();
	static Cam* _ref;

public:
	GLMat projMat;
	GLMat viewMat;
	
	static Cam* GetInstance();
	static void DeleteInstance();

	void Init(int screenW, int screenH, float zNear, float zFar, float zNearPlaneW);

	void SetProjection();
	void SetPerspectiveView();
	//void SetOrthoView();
	bool IsOrthoView();

	void SetModelViewMatrix();
	
	const float* GetMVP(float* modelMat);
	const float* GetModelViewMatrix(float* modelMat);
	const float* GetNormalMat(float* modelMat);
	

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
