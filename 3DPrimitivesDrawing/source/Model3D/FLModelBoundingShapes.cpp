#include "FLModelBoundingShapes.h"
#include "CFileReader.h"
#include "ImageBuffer.h"
#include "GLUtil/GLUtil.h"
#include "Cam.h"

FLModelBoundingShapes::FLModelBoundingShapes()
{
	_boundingShapes.clear();
}

FLModelBoundingShapes::~FLModelBoundingShapes()
{
	for(unsigned int i=0; i<_boundingShapes.size(); i++)
	{
		if(_boundingShapes[i])
		{
			delete _boundingShapes[i];
			_boundingShapes[i] = NULL;
		}
	}

	_boundingShapes.clear();
}

void FLModelBoundingShapes::AddBoundingShape(Shape* shape)
{
	_boundingShapes.push_back(shape);
}

void FLModelBoundingShapes::AddBoundingBox(float w, float h, float d, float* mat)
{
	Box* shape = new Box( mat, CVector3(w,h,d) );
	_boundingShapes.push_back(shape);
}

void FLModelBoundingShapes::AddBoundingCone(float r, float h, float* mat)
{
	Cone* shape = new Cone( mat, r, h );
	_boundingShapes.push_back(shape);
}

void FLModelBoundingShapes::AddBoudningCylinder(float r, float h, float* mat)
{
	Cylinder* shape = new Cylinder( mat, r, h );
	_boundingShapes.push_back(shape);
}

void FLModelBoundingShapes::AddBoundingSphere(float r, float* mat)
{
	Sphere* shape = new Sphere( mat, r );
	_boundingShapes.push_back(shape);
}

Shape* FLModelBoundingShapes::AddBestBoudingShapeByVerticesOnRect(Rect2D* rect, float* verArr, int numVertex, float* mat)
{
	float x = rect->x;
	float y = rect->y;
	float w = rect->w;
	float h = rect->h;

	vector<float> verVec = GLUtil::GetVerticesOnRect(verArr, numVertex, mat, x, y, w, h);
	
	Shape* bShape = NULL;

	if(verVec.size() > 0)
	{
		bShape = Shape::GetBestFitBoundingShape(&verVec[0], verVec.size());

		_boundingShapes.push_back(bShape);
	}

	return bShape;
}

Shape* FLModelBoundingShapes::AddBoudingShapeByVerticesOnRect(Rect2D* rect, int boundingShapeID, float* verArr, int numVertex, float* mat)
{
	float x = rect->x;
	float y = rect->y;
	float w = rect->w;
	float h = rect->h;

	vector<float> verVec = GLUtil::GetVerticesOnRect(verArr, numVertex, mat, x, y, w, h);
	
	Shape* bShape = NULL;

	if(verVec.size() > 0)
	{
		bShape = Shape::GetBoundingShape(&verVec[0], verVec.size(), boundingShapeID);

		if(bShape)
			_boundingShapes.push_back(bShape);
	}

	return bShape;
}

vector<Shape*> FLModelBoundingShapes::GetBoudingShapes()
{
	return _boundingShapes;
}

void FLModelBoundingShapes::DeleteBoundingShape(Shape* shape)
{
	for(int i=0; i<_boundingShapes.size(); i++)
	{
		if(_boundingShapes[i] == shape)
		{
			delete _boundingShapes[i];
			_boundingShapes.erase(_boundingShapes.begin() + i);
			break;
		}
	}
}

void FLModelBoundingShapes::DrawForSelection()
{
	GLboolean light = GLUtil::GLEnable(GL_LIGHTING, false);
	GLboolean blend = GLUtil::GLEnable(GL_BLEND, false);
	GLboolean depthTest = GLUtil::GLEnable(GL_DEPTH_TEST, true);
	unsigned int prevColor = GLUtil::GLColor(0x000000ff);

	for(int i=0; i<_boundingShapes.size(); i++)
	{
		Shape* shape = _boundingShapes[i];

		bool isUsingRandomColors = shape->IsUsingRandomColors();
		shape->SetUseRandomColors(false);

		shape->SetColor(i);
		shape->Draw();

		shape->SetUseRandomColors(isUsingRandomColors);
	}

	GLUtil::GLEnable(GL_LIGHTING, light);
	GLUtil::GLEnable(GL_BLEND, blend);
	GLUtil::GLEnable(GL_DEPTH_TEST, depthTest);
	GLUtil::GLColor(prevColor);
}

Shape* FLModelBoundingShapes::Get(unsigned int index)
{
	return _boundingShapes[index];
}

unsigned int FLModelBoundingShapes::Size()
{
	return _boundingShapes.size();
}

void FLModelBoundingShapes::Draw()
{
	for(unsigned int i=0; i<_boundingShapes.size(); i++)
		_boundingShapes[i]->Draw();
}

