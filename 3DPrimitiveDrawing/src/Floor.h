#ifndef Floor_H
#define Floor_H

#include "GLMat.h"
#include "GLMeshRenderer.h"

class Floor
{
private:
	bool _visible;
	bool _axisVisible;
	bool _gridVisible;
	bool _gridLinesVisible;
	
	GLMat _modelMat;

	GLMeshRenderer* _axisRenderer;
	GLMeshRenderer* _gridRenderer;
	GLMeshRenderer* _gridLinesRenderer;

public:
	Floor();
	~Floor();

	bool IsVisible();
	bool IsAxisVisible();
	bool IsGridVisible();
	bool IsGridLinesVisible();

	void SetVisible(bool visible);
	void SetAxisVisible(bool axisVisible);
	void SetGridVisible(bool floorVisible);
	void SetGridLinesVisible(bool linesVisible);

	void Draw();
};

#endif
