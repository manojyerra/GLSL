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

	float _sw;
	float _sh;

	Cam2D();

public:
	GLMat projMat;
	GLMat viewMat;
	
	static Cam2D* GetInstance();
	static void DeleteInstance();

	void Init(int screenW, int screenH);
	void SetScreenSize(int sw, int sh);
	
	glm::mat4 GetMVP(float* modelMat);
	glm::mat4 GetModelView(float* modelMat);
	
	void SetProjection();
	bool UpdateCamera();
};

#endif
