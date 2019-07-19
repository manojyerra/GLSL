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
	bool _smallGridLinesVisible;
	bool _bigGridLinesVisible;
	
	GLMat _modelMat;

	GLMeshRenderer* _gridRenderer;
	GLMeshRenderer* _gridRectRenderer;
	GLMeshRenderer* _smallGridLinesRenderer;
	GLMeshRenderer* _gridLinesRenderer;
	GLMeshRenderer* _bigGridLinesRenderer;
	GLMeshRenderer* _axisRenderer;

	void GenerateGrid(float start, float end, float gap);
	void GenerateGridRect(float start, float end);
	void GenerateSmallGridLines(float start, float end, float gap);
	void GenerateGridLines(float start, float end, float gap);
	void GenerateBigGridLines(float start, float end, float gap);
	void GenerateAxis(float start, float end);

	int _gridType;

public:
	enum
	{
		GRID_SINGLE_RECT,
		GRID_MULTI_RECT
	};

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
	void SetGridType(int gridType);

	void Draw();
};

#endif
