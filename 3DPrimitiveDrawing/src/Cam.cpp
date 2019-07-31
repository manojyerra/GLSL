#include "Cam.h"
#include "Input.h"

Cam* Cam::_ref = NULL;

Cam::Cam()
{
}

Cam* Cam::GetInstance()
{
	if(_ref == NULL)
	{
		_ref = new Cam();
	}

	return _ref;
}

void Cam::DeleteInstance()
{
	if(_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}

void Cam::Init(float screenW, float screenH, float zNear, float zFar, float zNearPlaneW)
{
	_sw = screenW;
	_sh = screenH;

	_zNear  = zNear;
	_zFar = zFar;
	_zNearPlaneHalfW = zNearPlaneW/2.0f;

	_pivot = glm::vec3(0.0f, 0.0f, 0.0f);
	_trans = glm::vec3(0.0f, -2.046f, -12.0f);
	_angle = glm::vec3(37.49f, -32.56f, 0);

	_viewType = 5;
	_isOrtho = false;
	_camUpdated = false;

	SetProjection();
	SetViewMatrix();
}

void Cam::SetScreenSize(float sw, float sh)
{
	_sw = sw;
	_sh = sh;

	SetProjection();
	SetViewMatrix();
}

void Cam::SetProjection()
{
	if(_isOrtho)
	{
		SetOrthoProjection();
	}
	else
	{
		SetPerspectiveProjection();
	}
}

void Cam::SetPerspectiveProjection()
{
	_isOrtho = false;

	_left = -_zNearPlaneHalfW;
	_right = _zNearPlaneHalfW;
	_bottom = -_zNearPlaneHalfW * _sh / _sw;
	_top = _zNearPlaneHalfW * _sh / _sw;

	projMat.loadIdentity();
	projMat.frustum(_left, _right, _bottom, _top, _zNear, _zFar);
}

void Cam::SetOrthoProjection()
{
	_isOrtho = true;

	_left = -(-_trans.z *_zNearPlaneHalfW) / _zNear;
	_right = (-_trans.z *_zNearPlaneHalfW) / _zNear;
	_bottom = -((-_trans.z *_zNearPlaneHalfW) / _zNear) * (_sh / _sw);
	_top = ((-_trans.z *_zNearPlaneHalfW) / _zNear) * (_sh / _sw);

	projMat.loadIdentity();
	projMat.ortho(_left, _right, _bottom, _top, _zNear, _zFar);
}

bool Cam::IsOrthoProjection()
{
	return _isOrtho;
}

void Cam::SetViewMatrix()
{
	viewMat.loadIdentity();
	viewMat.translatef(_trans.x, _trans.y, _trans.z);
	viewMat.rotatef(_angle.x, 1, 0, 0);
	viewMat.rotatef(_angle.y, 0, 1, 0);
	//viewMat.rotatef(-90, 1, 0, 0);
	viewMat.translatef(-_pivot.x, -_pivot.y, -_pivot.z);
}

glm::mat4 Cam::GetMVP(float* modelMat)
{
	glm::mat4 projMatrix = glm::make_mat4(projMat.m);
	glm::mat4 viewMatrix = glm::make_mat4(viewMat.m);
	glm::mat4 modelMatrix = glm::make_mat4(modelMat);
	return glm::mat4(projMatrix * viewMatrix * modelMatrix);
}

glm::mat4 Cam::GetModelViewMat(float* modelMat)
{
	glm::mat4 viewMatrix = glm::make_mat4(viewMat.m);
	glm::mat4 modelMatrix = glm::make_mat4(modelMat);
	return glm::mat4(viewMatrix * modelMatrix);
}

glm::mat3 Cam::GetNormalMat(float* modelMat)
{
	glm::mat4 viewMatrix = glm::make_mat4(viewMat.m);
	glm::mat4 modelMatrix = glm::make_mat4(modelMat);
	return glm::mat3(glm::transpose(glm::inverse(viewMatrix * modelMatrix)));
}

glm::mat4 Cam::GetProjMat()
{
	glm::mat4 projMatrix = glm::make_mat4(projMat.m);
	return glm::mat4(projMatrix);
}

glm::mat4 Cam::GetViewMat()
{
	glm::mat4 viewMatrix = glm::make_mat4(viewMat.m);
	return glm::mat4(viewMatrix);
}

bool Cam::UpdateCamera()
{
	_camUpdated = false;

	if( (Input::IsKeyPressed(Input::KEY_LEFT_SHIFT) || Input::IsKeyPressed(Input::KEY_RIGHT_SHIFT)) && Input::IsMiddleMousePressed())
	{
		if(Input::MX != Input::PrevMX || Input::MY != Input::PrevMY)
		{
			float dx = (float)(Input::MX - Input::PrevMX);
			float dy = (float)(Input::MY - Input::PrevMY);

			float z = _trans.z;
			if(z < 0)
				z = -z;

			z /= 3000;

			if (z < 0.003)
				z = 0.003;

			_trans.x += dx*z / (_zNearPlaneHalfW * 4.0f);
			_trans.y += -dy*z / (_zNearPlaneHalfW * 4.0f);
		}

		_camUpdated = true;
	}
	else if((Input::IsKeyPressed(Input::KEY_LEFT_CONTROL) || Input::IsKeyPressed(Input::KEY_RIGHT_CONTROL)) && Input::IsMiddleMousePressed())
	{
		if(Input::MY != Input::PrevMY)
		{
			float dy = (float)(Input::MY - Input::PrevMY);

			float z = _trans.z;
			if (z < 0)
				z = -z;

			z /= 300;

			if (z < 0.02)
				z = 0.02;

			_trans.z += -dy * z;
		}

		_camUpdated = true;
	}
	else if(Input::IsMiddleMousePressed())
	{
		float dx = (float)(Input::MX - Input::PrevMX);
		float dy = (float)(Input::MY - Input::PrevMY);

		_angle.y += dx * 0.75f * 180.0f / (_sw*0.5f);
		_angle.x += dy * 0.75f * 180.0f / (_sh*0.5f);

		_camUpdated = true;
	}
	//else if(Input::IsScrollDown())
	//{
	//	float dy = 30.0f;

	//	float z = _trans.z;
	//	if (z < 0)
	//		z = -z;

	//	z /= 300;

	//	_trans.z -= dy * z;

	//	_camUpdated = true;
	//}
	//else if(Input::IsScrollUp())
	//{
	//	float dy = 30.0f;

	//	float z = _trans.z;
	//	if (z < 0)
	//		z = -z;

	//	z /= 300;

	//	_trans.z += dy * z;

	//	_camUpdated = true;
	//}

	return _camUpdated;
}

bool Cam::IsCameraUpdated()
{
	return _camUpdated;
}

void Cam::SetPivot(glm::vec3 pivotPoint)
{
	_pivot = pivotPoint;
}

void Cam::SetTrans(glm::vec3 trans)
{
	_trans = trans;
}

void Cam::SetRot(glm::vec3 rot)
{
	_angle = rot;
}

glm::vec3 Cam::GetTrans()
{
	return _trans;
}

void Cam::SetFrontView()
{
	_angle = glm::vec3(0,0,0);
	_viewType = 0;
}

void Cam::SetBackView()
{
	_angle = glm::vec3(0,180,0);
	_viewType = 1;
}

void Cam::SetLeftView()
{
	_angle = glm::vec3(0,90,0);
	_viewType = 2;
}

void Cam::SetRightView()
{
	_angle = glm::vec3(0,-90,0);
	_viewType = 3;
}

void Cam::SetTopView()
{
	_angle = glm::vec3(90,0,0);
	_viewType = 4;
}

void Cam::SetBottomView()
{
	_angle = glm::vec3(-90,0,0);
	_viewType = 5;
}

void Cam::ChangeView()
{
	_viewType++;

	if(_viewType >= 6)
		_viewType = 0;

	if(_viewType == 0) SetFrontView();
	else if(_viewType == 1) SetBackView();
	else if(_viewType == 2) SetLeftView();
	else if(_viewType == 3) SetRightView();
	else if(_viewType == 4) SetTopView();
	else if(_viewType == 5) SetBottomView();
}
