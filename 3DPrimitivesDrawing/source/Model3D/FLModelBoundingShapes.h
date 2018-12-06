#ifndef FLModelBoundingShapes_H
#define FLModelBoundingShapes_H

#include "DefinesAndIncludes.h"
#include "GLUtil/GLUtil.h"
#include "Math/GLMat.h"
#include "Math/Vector3.h"
#include "Shapes3D/Shape.h"
#include "GLUtil/GL2DState.h"
#include "Shapes2D/Rect2D.h"
#include "Shapes3D/Box.h"
#include "Shapes3D/Cylinder.h"
#include "Shapes3D/Cone.h"
#include "Shapes3D/Sphere.h"

#include <vector>
#include <string>
using namespace std;


class FLModelBoundingShapes
{
private:
	vector<Shape*> _boundingShapes;
	
public:
	FLModelBoundingShapes();
	~FLModelBoundingShapes();

	void AddBoundingShape(Shape* shape);
	void AddBoundingBox(float w, float h, float d, float* mat);
	void AddBoundingCone(float r, float h, float* mat);
	void AddBoudningCylinder(float r, float h, float* mat);
	void AddBoundingSphere(float r, float* mat);

	Shape* AddBestBoudingShapeByVerticesOnRect(Rect2D* rect, float* verArr, int numVertex, float* mat);
	Shape* AddBoudingShapeByVerticesOnRect(Rect2D* rect, int boundingShapeID, float* verArr, int numVertex, float* mat);

	unsigned int Size();
	Shape* Get(unsigned int index);
	vector<Shape*> GetBoudingShapes();
	void DeleteBoundingShape(Shape* shape);
	void DrawForSelection();

	void Draw();
};

#endif
