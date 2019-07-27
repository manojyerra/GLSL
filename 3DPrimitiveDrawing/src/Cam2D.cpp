#include "Cam2D.h"
#include "Input.h"
#include <math.h>


Cam2D* Cam2D::_ref = NULL;

Cam2D::Cam2D()
{
}

Cam2D* Cam2D::GetInstance()
{
	if(_ref == NULL)
	{
		_ref = new Cam2D();
	}

	return _ref;
}

void Cam2D::DeleteInstance()
{
	if(_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}

void Cam2D::Init(float screenW, float screenH)
{
	_sw = screenW;
	_sh = screenH;

	SetScreenSize(_sw, _sh);

	SetProjection();
}

void Cam2D::SetScreenSize(float sw, float sh)
{
	_sw = sw;
	_sh = sh;

	_left = 0.0f;
	_right = _sw;
	_bottom = _sh;
	_top = 0.0f;
	_zNear = 0.0f;
	_zFar = 1.0f;
}

void Cam2D::SetProjection()
{
	projMat.loadIdentity();
	projMat.ortho(_left, _right, _bottom, _top, _zNear, _zFar);
}

glm::mat4 Cam2D::GetMVP(float* modelMat)
{
	glm::mat4 projMatrix = glm::make_mat4(projMat.m);
	glm::mat4 viewMatrix = glm::make_mat4(viewMat.m);
	glm::mat4 modelMatrix = glm::make_mat4(modelMat);
	return glm::mat4(projMatrix * viewMatrix * modelMatrix);
}

glm::mat4 Cam2D::GetModelView(float* modelMat)
{
	glm::mat4 viewMatrix = glm::make_mat4(viewMat.m);
	glm::mat4 modelMatrix = glm::make_mat4(modelMat);
	return glm::mat4(viewMatrix * modelMatrix);
}

glm::mat4 Cam2D::GetProjMat()
{
	glm::mat4 projMatrix = glm::make_mat4(projMat.m);
	return glm::mat4(projMatrix);
}
