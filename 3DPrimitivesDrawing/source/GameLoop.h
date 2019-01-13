#ifndef GameLoop_H
#define GameLoop_H

#include "Cam.h"
#include "Shapes2D/Triangle.h"
#include "Shapes2D/Rect2D.h"
#include "Floor.h"
#include "Cam.h"
#include "Input.h"
#include "Shapes3D/Box.h"
#include "Shapes3D/Cylinder.h"
#include "Shapes3D/Cone.h"
#include "Shapes3D/Sphere.h"
#include "Model3D/FLModel.h"
#include "ShadersManager.h"
#include "GLUtil/GLBatch.h"


class GameLoop 
{
private:
	int _sw;
	int _sh;
	float _zNear;
	float _zFar;
	float _zNearPlaneW;

	//Cam* _cam;

	//Floor* _floor;
	//Triangle* _triangle;
	//Box* _box;
	//Cylinder* _cylinder;
	//Cone* _cone;
	//Sphere* _sphere;

	void GLSettings();
	void DemoLight();

public:
	GameLoop(int sw, int sh);
	~GameLoop();

	void Update(float deltaTime);
	void Draw();
};

#endif
