#ifndef GLUtil_H
#define GLUtil_H

#include "Macros.h"
#include "GLMat.h"
#include "glm/vec3.hpp"

#include <vector>
using namespace std;

class GLUtil
{
private:
	static float SW;
	static float SH;

public:
	static void Init(int screenW, int screenH);
	static int GetWindowWidth();
	static int GetWindowHeight();

	static GLMat GetModelViewMatrix();
	static GLMat GetProjectionMatrix();
	static void SetModelViewMatrix(GLMat mat);
	static void SetProjectionMatrix(GLMat mat);
	static void GLReadPixelsFromTopLeft(int x, int y, int width, int height, GLenum format, GLenum type, GLvoid *pixels);

	static void GetProjectionValues(float* l, float* r, float* b, float* t, float* n, float* f);
	static void GetProjectionValues(float* projMat, float* l, float* r, float* b, float* t, float* n, float* f);

	static void Get2DPosOnScreenFrom3DPos(float* pos3D, float* pos2D, float* modelMatrix);
	static void Get2DPosOnScreenFrom3DPos(float* pos3D, float* pos2D, float* modelMatrix, float* projMatrix);
	static vector<glm::vec3> Get2DPosOnScreenFrom3DPos(vector<glm::vec3>* pos3DVec, float* modelMatrix);
	static vector<float> GetVerticesOnRect(float* verArr, int numVertex, float* objMat, float x, float y, float w, float h);
};

#endif
